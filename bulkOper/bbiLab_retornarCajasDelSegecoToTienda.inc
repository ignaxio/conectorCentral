<?php




function bbiLab_retornarCajasDelSegecoToTienda($context) {
    $caja = node_load($context->nid);    
    //Modificamos los campos
    $caja->field_caja_localizacion['und'][0]['value']   = 1;//en bbi
    $caja->field_caja_fecha_envio                       = array(); 
    $caja->field_ayuntamiento                           = array(); 
    $caja->field_veterinario                            = array(); 
    $caja->field_caja_tracking_de_envio                 = array(); 
    $caja->field_caja_tracking_de_recogida              = array(); 
    $caja->field_caja_fecha_cierre                      = array(); 
    node_save($caja);
    
    //Ahora sus viales
    $viales = bbiLab_getTodosLosVialesDeUnaCaja($context->nid);
    foreach ($viales as $idVial => $tituloVial) {
        $vial = node_load($idVial);
        $vial->field_ayuntamiento                           = array(); 
        $vial->field_veterinario                            = array(); 
        $vial->field_vial_localizacion['und'][0]['value']   = 1;//en bbi
        node_save($vial);
    }
    
    //Eliminar viales del segeco??????????????????????????????????
    
    
}
