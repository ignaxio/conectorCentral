<?php


function bbiLab_setVialToSegeco() {
    //Vamos a ir cogiendo los viales y enviarlos a su ayuntamiento
    
    $aytos = BbiConector::getAyuntamientosActivos(); 
    foreach ($aytos as $ayto) {
        $queue = DrupalQueue::get('setVialTo' . $ayto);
        $remote = new CodeServer('conector' . $ayto);
        $remote->config->request_timeout = 2;//Tiempo de espera de lectura en segundos, especificado por un float (p.ej. 10.5).Por omisión se utiliza el valor del ajuste default_socket_timeout de php.ini.
        $time = microtime();
        $max = (int)BbiConector::getMaxTimeExecuteAQueue($ayto); 
        while (($item = $queue->claimItem(7400)) && ((microtime() - $time) < $max)) {
            //Aquí llamamos a función remota, si devuelve false hay que mandarlo otra vez a la cola
            $result = $remote->conectorSegeco_setVial($item->data);  

            if($result) {
                //Eliminamos el item de la queue
                $queue->deleteItem($item);
                //Si lo ha enviado le mandamos un mensaje a Lourdes...
                $veterinario = $vial['veterinario'] ? $vial['veterinario'] : 'sin veterinario';
                drupal_set_message("Vial enviado correctamente, se envió el vial al ayuntamiento de $ayuntamiento en el veterinario $veterinario", 'status');
            } else { 
                //Si no ha podido conectar, mandamos email de error y volvemos a encolar el vial pa la próxima.
                $mensaje = 'Error durante la sincronización, no se envió el vial ' . $vial['nid'] . ' a ' . $ayuntamiento . '. <br>'
                        . 'El vial contenía : '
                        . '<li><ul>' 
                        . 'Título: '        . $vial['titulo']
                        . '</ul><ul>'
                        . 'Ayntamiento: '   . $vial['idAyuntamiento']
                        . '</ul><ul>'
                        . 'Veterinario: '   . $vial['veterinario']
                        . '</ul><ul>'
                        . 'Tipo de vial: '  . $vial['tipoDeVial']
                        . '</ul></li>';
                drupal_set_message($mensaje, 'error');
                //Lo pasamos a la cola de errores.
                $queue = DrupalQueue::get('BBIEnviarVialA' . $ayto . '_error');
                $queue->createItem($vial);
        //        //Mandamos un email de error pa avisar.
        //        bbiLab_sendEmail('error', $mensaje);
            }
        }
    }
}





