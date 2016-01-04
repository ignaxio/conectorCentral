<?php


function bbiLab_enviarChapa($context) {
    
    $chapa[$context['nid']]['nid']              
            = $context['nid'];
    $chapa[$context['nid']]['titulo']           
            = $context['title'];
    $chapa[$context['nid']]['tipo']             
            = $context['field_chapa_tipo_chapa_value'];    
    $chapa[$context['nid']]['idAyuntamiento']   
            = $context['field_ayuntamiento_target_id'];             
    $chapa[$context['nid']]['fechaEnvio']       
            = bbiLab_getFecha(); 
    
    //Creo un objeto y le meto el array en el atributo chapa, 
    //así cuando llege al segeco sabra que es una chapa
    $datos = new stdClass();
    $datos->chapa = $chapa;
    $datos->operaciones = array(22, 32); //Le pasamos las operaciones de insertar y update chapa para que haga la que tenga que hacer
    //Creamos el item para que el cron lo lance el solo
    $queue = DrupalQueue::get('bufferSalidaconector' . bbiLab_getUserById($context['field_ayuntamiento_target_id']));
    $respuesta = $queue->createItem($datos);
    //Le metemos la fecha de envío.
    $context->field_chapa_fecha_envio['und'][0] = bbiLab_getFecha();
}













