<?php


function bbiLab_darDeBajaUnVial($context) {
    if(isset($context->field_vial_fecha_de_extracci_n['und'][0]['value'])) {
        drupal_set_message('El vial tiene fecha de extracción, no puede ser dado de baja.', 'status');
        return;
    }
    $vial = entity_metadata_wrapper('node', $context); 
    //Vamos a poner a null todo
    $vial->field_vial_estado->set(9);
    $vial->field_ayuntamiento->set(NULL);
    $vial->field_veterinario->set(NULL);
    $vial->field_vial_localizacion->set(5);
    $vial->field_vial_fecha_de_extracci_n->set(NULL);
    $vial->field_vial_fecha_fin_analisis->set(NULL);
    $vial->save();
    //enviar a segcan y eliminar...
    
    $paquete = new BbiPaquete();
    $paquete->setEstado(9);//Baja
    
    //Creo un objeto y le meto el array en el atributo vial, 
    //así cuando llege al segeco sabra que es un vial
    $datos = new stdClass();
    $datos->paquete = $paquete;
    $datos->operaciones = array(2);//Actualizar
    //Creamos el item para que el cron lo lance el solo
    BbiBuffer::insert_item_into_buffer($datos, 'segcan');
    
    
}
