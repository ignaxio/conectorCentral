<?php

/**
 * función que checkea en todos los ayuntamientos si hay algún vial lleno
 */
function bbiLab_getVialesLlenosDelSegeco() {
    $serversNames = BbiConector::getServerNameActivos();
    foreach ($serversNames as $uid => $serverName) {
        $queue = DrupalQueue::get('BBIVialLlenoDe' . bbiLab_getUserById($uid));
        $remote = new CodeServer($serverName);
        $remote->config->request_timeout = 10;
        $time = microtime();
        $max = (int)BbiConector::getMaxTimeExecuteAQueue(bbiLab_getUserById($uid)); 
        while (($item = $remote->conectorSegeco_getVialesLlenos()) && ((microtime() - $time) < $max)) {            
            $vialArray = $item->data;     
            
            $tituloVial         = $vialArray['titulo'];
            $fechaVialLleno     = $vialArray['fechaVialLleno'];
            //Cargamos el vial.
            $vial = node_load(bbiLab_getIdNodeByTitle($tituloVial)); 

            //Si el vial ya tiene fecha de extración quiere decir que ya se sincronizo antes, no hay que sumar nada en el contador de la caja
            $sincronizado = isset($vial->field_vial_fecha_de_extracci_n['und'][0]) ? TRUE : FALSE;
            //Le metemos la fecha de extracción y el estado
            bbiLab_getVialesLlenosDelSegeco_setDatosAlVial($vial, $fechaVialLleno);
            //Ahora marcamos su caja con la Fecha primer vial lleno y sumamos uno a viales llenos
            bbiLab_getVialesLlenosDelSegeco_setFechaPrimerVialLlenoDeLaCaja($vial, $fechaVialLleno, $sincronizado);
            //Ahora hay que crear la chapa si viene chapa.
            if($vialArray['crearChapa']) {
                bbiLab_getVialesLlenosDelSegeco_setChapa($vialArray, $vial->field_ayuntamiento['und'][0]['target_id']);
            }
            //Ahora lo metemos en la cola para que lo lleve a segcan
            $queue->createItem($item);
            
            //Si llega aquí es que todo fué bien, ahora hay que eliminar el vial del segeco
            $remote->conectorSegeco_eliminarItemDeLaQueue($item->item_id);
        }
    }
}

function bbiLab_procesarItemsSegeco() {
    //Vamos a coger los ayntamientos para poder montar los nombres de las queue 
    //y coger los items
    $aytos = BbiConector::getAyuntamientosActivos(); 
    foreach ($aytos as $ayto) {
        $queue = DrupalQueue::get('BBIVialLlenoDe' . $ayto);
        $time = microtime();
        $max = (int)BbiConector::getMaxTimeExecuteAQueue($ayto); 
        while (($item = $queue->claimItem(7400)) && ((microtime() - $time) < $max))  {            
            $itemSegeco = $item->data;            
            $vialArray = $itemSegeco->data;
            $tituloVial = $vialArray['titulo'];
            //Cargamos el vial.
            $vial = node_load(bbiLab_getIdNodeByTitle($tituloVial)); 
           
            //29-10-2015 Ahora hay que avisar al segcan y mandarle el vial
            $respuesta = bbiLab_getVialesLlenosDelSegeco_setvialToSegcan($vial);
            //Si logra conectar con segcan y devuelve true significa que todo ha ido bien y podemos dejar que la queue se borre.
            if(!$respuesta) {
                //Si no ha conectado con segcan mandamos la queue a la cola de errores.
                $queue = DrupalQueue::get('BBIVialLleno_error');
                $queue->createItem($vialArray);
            } else {
                //Si llega aquí es que todo fué bien, ahora hay que eliminar el vial de la queue
                $queue->deleteItem($item);
            }
        }
    }
    
}



function bbiLab_getVialesLlenosDelSegeco_setChapa($vialArray, $ayuntamiento) {
    $vialId             = bbiLab_getIdNodeByTitle($vialArray['titulo']);
    $tituloSolicitud    = isset($vialArray['tituloSolicitud']) ? 
            $vialArray['tituloSolicitud'] : NULL;
    $tipoChapa          = isset($vialArray['tipoChapa']) ? 
            $vialArray['tipoChapa'] : NULL;
    $nombrePerro        = isset($vialArray['nombrePerro']) ? 
            $vialArray['nombrePerro'] : NULL;
    $digitoControl      = isset($vialArray['digitoControl']) ? 
            $vialArray['digitoControl'] : NULL;
    //Si no tiene titulo chapa lo montamos, quiere decir que en xativa si me traigo el título, en lebrija hay que montarlo.
    $tituloChapa        = isset($vialArray['tituloChapa']) ? 
            $vialArray['tituloChapa'] : 
            $tituloSolicitud . ' ' . $nombrePerro . ' ' . $digitoControl;
    
    //Primero comprobamos que no exista ya, si existe no lo crea, manda un email de error
    if(node_load(bbiLab_getIdNodeByTitle($tituloChapa))) {//        
        return;
    }
    //Creamos la chapa
    $values = array(
        'type'      => 'chapa',
        'uid'       => 1,
        'status'    => 1,
        'comment'   => 0,
        'promote'   => 0,
    );
    $entity = entity_create('node', $values);
    $ewrapper = entity_metadata_wrapper('node', $entity);  
    $ewrapper->title->set($tituloChapa);
    $ewrapper->field_chapa_tipo_chapa->set((int)$tipoChapa);    
    if($digitoControl) $ewrapper->field_chapa_digito_control->set($digitoControl);  
    $ewrapper->field_ayuntamiento->set((int)$ayuntamiento); 
    $ewrapper->field_chapa_vial->set((int)$vialId);
     
    $ewrapper->save();    
    //if(node_load($ewrapper->getIdentifier())) return true;
}

function bbiLab_getVialesLlenosDelSegeco_setvialToSegcan($vial) {
    //Vamos a crear el array para mandarlo con los datos que necesitamos.
    $v['titulo'] = $vial->title;
    $v['tipoDeVial'] = $vial->field_vial_tipo_de_vial['und'][0]['value'];
    $v['ayuntamiento'] = bbiLab_getUserById($vial->field_ayuntamiento['und'][0]['target_id']);
    $v['veterinario'] = isset($vial->field_veterinario['und'][0]['target_id']) ? 
            bbiLab_getUserById($vial->field_veterinario['und'][0]['target_id']) : 
            false;
        
    $remote = new CodeServer('conectorSegcan');
    $remote->config->request_timeout = 10;
    $respuesta = $remote->conectorSegcan_setMuestra($v);
    drupal_set_message($respuesta, 'status');
    return $respuesta;
}

/**
 * Funcion que carga datos al vial la fecha de extracción y el estado 
 * 1|Pendiente de recepción al vial
 * @param type $vial
 */
function bbiLab_getVialesLlenosDelSegeco_setDatosAlVial($vial, $fechaVialLleno) {    
    //Primero le metemos la fecha de extracción al vial
    $vial->field_vial_fecha_de_extracci_n['und'][0] = 
            bbiLab_getFecha($fechaVialLleno);
    //Si no esta cargado ya el estado le metemos el estado 
    //1|Pendiente de recepción al vial, si ya tiene estado lo dejamos como está
    $vial->field_vial_estado['und'][0]['value'] = 
            isset($vial->field_vial_estado['und'][0]['value']) ? 
            $vial->field_vial_estado['und'][0]['value'] : 1;
    node_save($vial);
}

/**
 * Función que pone en la caja de un vial la fecha del primer vial lleno, 
 * si existe ya la fecha la deja como esta.
 * Le mete al campo viales llenos +1
 */
function bbiLab_getVialesLlenosDelSegeco_setFechaPrimerVialLlenoDeLaCaja(
        $vial, $fechaVialLleno, $sincronizado) {
    //Buscamos el id de la caja en la coleccion de campos del vial, 
    //cogemos la última colección de campos.
    $idCaja = bbiLab_getElIdDeLaCajaDeUnVial($vial);
    //Cargamos la caja
    $caja = node_load($idCaja);

    //Le metemos la fecha, si no hay ninguan todavía...
    $caja->field_caja_fec_primer_vial_lleno['und'][0] = 
            isset($caja->field_caja_fec_primer_vial_lleno['und'][0]['value']) ? 
            $caja->field_caja_fec_primer_vial_lleno['und'][0] : 
            bbiLab_getFecha($fechaVialLleno);
    
    //Le sumamos 1 a viales llenos si no esta sincronizado ya de antes, 
    //quiere decir que ya se metió en la tienda antes.
    if(!$sincronizado) {
         $caja->field_caja_viales_llenos['und'][0]['value'] += 1;
    }
    node_save($caja);
}

/**
 * función que marca como sincronizado un vial lleno en su ayuntamiento
 */
function bbiLab_getVialesLlenosDelSegeco_setVialSincronizadoEnAyuntamiento($item_id, $ayto) {
    //Hacemos la conexión y lo enviamos
    $remote = new CodeServer('conector' . $ayto);
    $remote->config->request_timeout = 10;
    $result = $remote->conectorSegeco_eliminarItemDeLaQueue($item_id);
    return $result;
    
    
    
    
    
    
    $result = false;
    //Creo el nombre del conector
    $conector = 'conector' . bbiLab_getUserById($vial->field_ayuntamiento['und'][0]['target_id']);
    
    //ahora ya no necesitamos el switch..
    $remote = new CodeServer($conector);
    $remote->config->request_timeout = 10;
    $result = $remote->conectorSegeco_marcarVialSincronizado($vial->title);
    
    if ($result) return true;
}

function bbiLab_getVialesLlenosDelSegeco_getVialesLlenosDeAyntamiento() {
    $arrayDeArrayDeViales = array();
    $viales = array();
    //Vamos a recoger los serverNames activos
    $serversNames = BbiConector::getServerNameActivos();
    //Vamos a recorrer los ayuntamientos
    foreach ($serversNames as $uid => $serverName) {
        $remote = new CodeServer($serverName);
        $remote->config->request_timeout = 10;
        if ($viales = $remote->conectorSegeco_getVialesLlenos()) {
            $arrayDeArrayDeViales[bbiLab_getUserById($uid)] = $viales;
        }        
    }
    return $arrayDeArrayDeViales;
}
