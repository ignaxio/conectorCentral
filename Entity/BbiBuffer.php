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
            $remote->config->request_timeout = 2;
            $time = microtime();
            $max = (int) BbiConector::getMaxTimeExecution(bbiLab_getUserById($uid));

            while (($item = $remote->get_item_from_buffer()) && ((microtime() - $time) < $max)) {
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
        foreach ($data->operaciones as $operacion) {
            $resultado = FALSE;
            switch ($operacion) {
                case 1://Insertar vial tienda

                    break;
                case 2://Insertar chapa en tienda
                    $resultado = BbiChapa::insert($data->data);
                    break;
                case 11://Update vial en tienda
                    $resultado = BbiVial::update($data->data);

                    break;
                case 12://Update chapa en tienda

                    break;


                case 100://Insert into buffer
                    //Si hay que enviar los datos los metemos en el buffer de salida
                    $resultado = BbiBuffer::insert_item_into_buffer($item, $serverName);


                    break;
                default:
                    if (!$resultado)
                        return FALSE;
            }
        }
        return $resultado;
    }

    public static function insert_item_into_buffer($item, $serverName) {
        //Si viene de segcan habrá que mirar de que ayntamiento es y mandarlo para allí
        //Si viene de algún ayntamiento hay que mandarlo a segcan
        $tipoSegeco = BbiConector::getTipoDeSegeco($serverName);
        if ($tipoSegeco == 'Segcan') {
            //Vamos a ver de que ayto es
            $data = $item->data;
            $array = $data->data;
            if ($ayto = $array['ayuntamiento']) {
                $queue = DrupalQueue::get('bufferSalidaconector' . $ayto);
                $queue->createItem($item);
                return TRUE;
            } else {
                return FALSE;
            }
        } else {
            //Si viene de algún ayto lo mandamos a segcan, cuando haya mas segcan ya veremos
            $queue = DrupalQueue::get('bufferSalidaconectorSegcan');
            $queue->createItem($item);
            return TRUE;
        }
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
            $remote->config->request_timeout = 2; //Tiempo de espera de lectura en segundos, especificado por un float (p.ej. 10.5).Por omisión se utiliza el valor del ajuste default_socket_timeout de php.ini.
            $time = microtime();
            $max = (int) BbiConector::getMaxTimeExecution(bbiLab_getUserById($uid));
            while (($item = BbiBuffer::get_item_from_buffer($queue)) && ((microtime() - $time) < $max)) {
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
