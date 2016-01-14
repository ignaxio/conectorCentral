<?php

function bbiLab_getVialesSegcan() {
    $aytos = BbiConector::getAyuntamientosActivos();       
    $remote = new CodeServer('conectorSegcan');
    $remote->config->request_timeout = 5;//Tiempo de espera de lectura en segundos, especificado por un float (p.ej. 10.5).Por omisión se utiliza el valor del ajuste default_socket_timeout de php.ini.
    $time = microtime();
    $max = (int)BbiConector::getMaxTimeExecuteAQueue('Segcan'); 
    while (($item = $remote->conectorSegcan_getVialesSinSincronizar($aytos)) && ((microtime() - $time) < $max)) {
        $vial = $item->data; 
        //Primero hay que marcar en la tienda el resultado (estado) de la muestra
        $result = bbiLab_getVialesSegcan_setEstadoEnTienda($vial); 

        //Ahora hay que mandar el nuevo estado al segeco de su ayuntamiento
        if($result) $result = bbiLab_getVialesSegcan_setEstadoEnSegeco($vial);    
        
        //si todo fue bien hay que meterlos en la cola de la tienda y informar a segcan
        if($result) {
            //Vamos a meterlo en la queue de la tienda
            $queue = DrupalQueue::get('vialesFromSegcanDe' . $vial['ayuntamiento']);  
            $queue->createItem($vial);   
            
            //Ahora hay que conectar a segcan y eliminar el item de la queue para que no vuelva a venir
            $remote->conectorSegcan_eliminarItemDeLaQueue($item->item_id);
            drupal_set_message ("El vial " . $vial['titulo'] . 
                    " se ha sincronizado en " . $vial['ayuntamiento'] . ".", "status");
        }
    }       
}


function bbiLab_procesarItemsSegcan() {
    //Aquí vamos a ir cogiendo los items que se metieron que venían de segcan y 
    //vamos a actualizar los datos.
    //vamos a ver los ayuntamientos activos para construir el nombre de las queue
    $aytos = BbiConector::getAyuntamientosActivos(); 
    foreach ($aytos as $ayto) {
        $queue = DrupalQueue::get('vialesFromSegcanDe' . $ayto);  
        $time = microtime();
        $max = (int)BbiConector::getMaxTimeExecuteAQueue($ayto); 
        while (($item = $queue->claimItem(7400)) && ((microtime() - $time) < $max)) {
            $vial = $item->data;     
            
            //Ahora hay que mandar el nuevo estado al segeco de su ayuntamiento
            $result = bbiLab_getVialesSegcan_setEstadoEnSegeco($vial);    

            //Si ha podido conectar con el segeco eliminamos el item de la tienda.
            if($result) {
                $queue->deleteItem($item);
            }
        }
    }    
}
    
function bbiLab_getVialesSegcan_setEstadoEnSegeco($vial) {
    $result = false;
    //Creamos el nombre del conector
    $conector = 'conector' . $vial['ayuntamiento'];
    
    //Vamos a transformar la fecha para que drupal la lea bien si la tiene
    $vial['fechaInforme'] = isset($vial['fechaInforme']) ? 
            bbiLab_getFecha($vial['fechaInforme']) : NULL;
    try {
        $remote = new CodeServer($conector);
        $remote->config->request_timeout = 10;
        //Aquí llamamos a función remota, si devuleve true se marca el vial sincronizado.
        $result = $remote->conectorSegeco_setEstadoVial($vial);
    } catch (Exception $e) {
        $message = 'Error al conectar en el segeco de ' . $vial['ayuntamiento'] .
                ' para sincronizar el vial ' . $vial['titulo'] . '  traidos de segcan. ';
        watchdog_exception('muestras Segcan', $message . $e, NULL, WATCHDOG_ERROR);
    }
    return $result;
}

function bbiLab_getVialesSegcan_setEstadoEnTienda($vial) {
    $result = TRUE;
    $n;
    if(!$n = node_load(bbiLab_getIdNodeByTitle($vial['titulo']))) {
        $result = FALSE;
        $message = 'error con el vial traido de segcan, el vial' . $vial['titulo'] . ' no existe en la tienda';
        watchdog('Viales de segcan', $message, NULL, WATCHDOG_ERROR);
//        bbiLab_sendEmail('Error', $message);
    }
    
    $n->field_vial_estado['und'][0]['value'] = (int)$vial['resultado'];
    if(isset($vial['fechaInforme'])) {
        $n->field_vial_fecha_fin_analisis['und'][0] = bbiLab_getFecha($vial['fechaInforme']);
    }                               
    node_save($n);
    
    //Marcamos en la chapa el finalizado OK
    bbiLab_getVialesSegcan_setEstadoEnChapa($vial['resultado'], $n->nid);
    return $result;
}

/**
 * Función que marca en la chapa el finalizado ok
 * @param int $reslutado
 * @param int $nid
 */
function bbiLab_getVialesSegcan_setEstadoEnChapa($reslutado, $nid) {    
    //Si el estado es 3, finalizado ok lo tenemos que marcar en la chapa    
    if((int)$reslutado === 3) {
        //Vamos a meterle a la chapa que tenga asociado este vial el finalizado ok
        if($chapaId = BbiVial::getChapa((int)$nid)) {
            $chapa = node_load($chapaId);
            $chapa->field_chapa_finalizado_ok['und'][0]['value'] = TRUE;
            node_save($chapa);
        }
    }
}
