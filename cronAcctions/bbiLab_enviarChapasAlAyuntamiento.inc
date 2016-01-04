<?php

function bbiLab_setChapaToSegeco() {
    //Vamos a ir cogiendo los viales y enviarlos a su ayuntamiento
    $aytos = BbiConector::getAyuntamientosActivos(); 
    foreach ($aytos as $ayto) {
    $queue = DrupalQueue::get('BBIEnviarChapaA' . $ayto);  
        $remote = new CodeServer('conector' . $ayto);
        $remote->config->request_timeout = 5;//Tiempo de espera de lectura en segundos, especificado por un float (p.ej. 10.5).Por omisión se utiliza el valor del ajuste default_socket_timeout de php.ini.
        $time = microtime();
        $max = (int)BbiConector::getMaxTimeExecuteAQueue($ayto); 
        while (($item = $queue->claimItem(7400)) && ((microtime() - $time) < $max)) {
            $chapa = $item->data;
            $result = $remote->conectorSegeco_setChapa($chapa);      

            $titulo = $chapa['titulo'];
            if($result) {
                //Si todo a ido bien le metemos la fecha de envío.
                $n = node_load($chapa['nid']);
                //Le metemos la fecha de envío
                $n->field_chapa_fecha_envio['und'][0] = $chapa['fechaEnvio']; 
                node_save($n);

                //Eliminamos el item de la queue
                $queue->deleteItem($item);
                drupal_set_message("Chapa $titulo enviada correctamente, se envió la chapa al ayuntamiento de $ayuntamiento.", 'status');
            } else {
                //Lo metemos en la cola de errores.
                $queue = DrupalQueue::get('BBIenviarChapaA' . $ayto . '_error');
                $queue->createItem($chapa);
                $mensaje = "Error durante la sincronización, no se envió la chapa $titulo a $ayuntamiento.";
                drupal_set_message($mensaje, 'error');
//                bbiLab_sendEmail('error', $mensaje);
            }
        }
    }
}



