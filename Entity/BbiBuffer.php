<?php

/**
 * Operaciones disponibles
 *  1 - Insertar vial en tienda
 *  2 - Insertar chapa en tienda
 *  11 - update vial en tienda
 *  12 - update chapa en tienda
 *  21 - insertar vial en segeco
 *  22 - insertar chapa en segeco
 *  31 - update vial en segeco
 *  32 - updat chapa en segeco
 *  50 - eliminar viales vacios de una caja
 * 100 - insertar inti buffer en tienda
 */

/**
 * Description of BBIBuffer
 *
 * @author ignacio
 */
class BbiBuffer {

    public static function check_buffer_entrada() {
        //Vamos a recorrer los conectores activos
        $serversNames = BbiConector::getServerNameActivos();

        foreach ($serversNames as $uid => $serverName) {
            $queue = DrupalQueue::get('bufferSalida' . $serverName);
            $remote = new CodeServer($serverName);
            $remote->config->request_timeout = 10;
            $time = microtime(TRUE);
            $max = (int) BbiConector::getMaxTimeExecution(bbiLab_getUserById($uid));

            while (($item = $remote->get_item_from_buffer()) && ((microtime(TRUE) - $time) < $max)) {
                if (BbiBuffer::process_item($item, $serverName)) {
                    $remote->remove_item_from_buffer($item->item_id);
                } else {
                    $remote->insert_item_error_buffer($item);
                }
            }
        }
    }

    public static function process_item($item, $serverName) {
        $resultado = FALSE;
        $data = $item->data;
        $paquete = $data->paquete;

        //Si no existe el vial se meterá en la cola de errores.
        if ($vial = node_load(bbiLab_getIdNodeByTitle($paquete->getTituloVial()))) {
            //Vamos a ver antes de guardar los datos si el vial que viene tiene fecha de extracción, 
            //si no la tiene es que en segcan no devería existir y hay que crearla nueva, 
            //si tiene fecha de extracción se envia para actualizar datos.
            if (isset($vial->field_vial_fecha_de_extracci_n['und'][0]['value'])) {
                $operacionParaSegcan = 2; //Lo actualizamos
            } else {
                $operacionParaSegcan = 1; //Lo creamos
            }
            foreach ($data->operaciones as $operacion) {
                switch ($operacion) {
                    case 1://Crear????? nada de momento

                        break;
                    case 2://actualizar
                        //Antes de guardar el vial vamos a actualizar la caja si viene de un segeco.
                        //Miramos le fecha de vial lleno para saberlo.
                        if ($paquete->getFechaVialLleno()) {
                            BbiCaja::update($paquete);
                        }
                        $resultado = BbiVial::update($paquete);

                        //Si viene el titulo de la chapa la creamos o la editamos si ya está en tienda
                        if ($paquete->getTituloChapa()) {
                            //Mirar si existe la chapa en el vial y se actualiza los datos
                            if ($idChapa = BbiVial::getChapa(bbiLab_getIdNodeByTitle($paquete->getTituloVial()))) {
                                $resultado = BbiChapa::update($paquete, $idChapa);
                            } else {
                                //Si no existe se crea la chapa
                                $resultado = BbiChapa::insert($paquete);
                            }
                        } 
                        //No tiene fecha extraccion... y viene de algún segeco
                        // hay que eliminar la chapa, si la hubiese
                        //Esto quiere decir que se ha editado la solicitud y se ha cambiado de vial.
                        if (!$paquete->getFechaVialLleno() && $serverName != 'conectorSegcan') {
                            
                            if($vial = node_load(bbiLab_getIdNodeByTitle($paquete->getTituloVial()))) {
                                if($idChapa = BbiVial::getChapa($vial->nid)) {
                                    watchdog('prueba vial editado', 'chapa que se va a eliminar ' . $idChapa);
                                    //Si teien chapa la eliminamos
                                    node_delete($idChapa);
                                    
                                    watchdog('prueba vial editado', 'chapa eliminada ' . $idChapa);
                                    //ahora limpiamos el vial
                                    $vial->field_vial_fecha_de_extracci_n['und'] = NULL;                                    
                                    $vial->field_vial_estado['und'] = NULL;
                                    $vial->field_vial_localizacion['und'][0]['value'] = 3;
                                    node_save($vial);
                                    
                                    watchdog('prueba vial editado', 'vial se ha quitado la fecha de extracción ' . $vial->title);
                                    //Le restamos 1 a la caja.
                                    $caja = node_load(BbiCaja::getIdCaja($paquete->getTituloVial()));
                                    watchdog('prueba vial editado', 'se le va a restar uno a la caja ' . $caja->field_caja_viales_llenos['und'][0]['value']);
                                    
                                    
                                    $caja->field_caja_viales_llenos['und'][0]['value'] -= 1;
                                    node_save($caja);
                                    
                                    watchdog('prueba vial editado', 'se le ha restado una a la caja ' . $caja->field_caja_viales_llenos['und'][0]['value']);
                                    
                                }
                            }
                        }
                        //Si viene de segcan habrá que mirar de que ayntamiento es y mandarlo para allí
                        //Si viene de algún ayntamiento hay que mandarlo a segcan
                        $tipoSegeco = BbiConector::getTipoDeSegeco($serverName);
                        if ($tipoSegeco == 'Segcan') {
                            if ($ayto = $paquete->getAyuntamiento()) {
                                //Operaciones para los segecos, 
                                $data->operaciones = array(2, 4); //Update vial y chapa si la hubiese
                                BbiBuffer::insert_item_into_buffer($data, $ayto);
                                return TRUE;
                            } else {
                                return FALSE;
                            }
                        } else {
                            //Hay que mandar si es actualizar el vial o es insertar un vial nuevo.
                            //Si viene de algún ayto lo mandamos a segcan, cuando haya mas segcan ya veremos
                            $data->operaciones = array($operacionParaSegcan);
                            BbiBuffer::insert_item_into_buffer($data, 'segcan');
                            return TRUE;
                        }
                        break;
                }
            }
        }

        return $resultado;
    }

    public static function insert_item_into_buffer($datos, $ayto) {
        $queue = DrupalQueue::get('bufferSalidaconector' . $ayto);
        $queue->createItem($datos);
    }

    public static function get_item_from_buffer($queue) {
        return $queue->claimItem(7200);
    }

    public static function remove_item_from_buffer($queue, $item) {
        $queue->deleteItem($item);
    }

    public static function insert_item_error_buffer($serverName, $data) {
        $queue = DrupalQueue::get('bufferSalida' . $serverName . 'Error');
        $queue->createItem($data);
    }

    public static function send_buffer_salida() {
        //Vamos a recorrer los conectores activos
        $serversNames = BbiConector::getServerNameActivos();
        foreach ($serversNames as $uid => $serverName) {
            $queue = DrupalQueue::get('bufferSalida' . $serverName);
            $remote = new CodeServer($serverName);
            $remote->config->request_timeout = 20; //Tiempo de espera de lectura en segundos, especificado por un float (p.ej. 10.5).Por omisión se utiliza el valor del ajuste default_socket_timeout de php.ini.
            $time = microtime(TRUE);
            watchdog('pruebatiempo', 'inicio total ' . $time);
            $max = (int) BbiConector::getMaxTimeExecution(bbiLab_getUserById($uid));
            while (($item = BbiBuffer::get_item_from_buffer($queue)) && ((microtime(TRUE) - $time) < $max)) {
            watchdog('pruebatiempo', 'inicio una prueba ' .  (microtime(TRUE) - $time));
                if ($item) {
                    if ($remote->write_element_from_buffer($item)) {
                        BbiBuffer::remove_item_from_buffer($queue, $item); //Lo ha metido, lo borramos del buffer de salida
                    } else {
                        $item = $item ? $item : 'no hay elemento';
                        BbiBuffer::insert_item_error_buffer($serverName, $item); // Ha fallado lo metemos en el buffer de errores de salida específico.
                    }
                }
            }
        }
    }

}
