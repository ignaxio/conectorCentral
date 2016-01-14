<?php

//Repasar este código, no creo que se acceda a los datos como un array, tiene que ser como un objeto.
//$context->nid    ----   no $context['nid]



function bbiLab_enviarChapa($context) {
    $paquete = bbiLab_enviarChapa_llenarPaquete($context);
    $datos = new stdClass();
    $datos->paquete = $paquete;
    //Vamos a ver el tipo de segeco que es y le damos las operaciones para cada uno.
    $tipoSegeco = BbiConector::getTipoDeSegeco('conector' . $paquete->getAyuntamiento());
    switch ($tipoSegeco) {
        case 'Segeco1'://Lebrija
            $datos->operaciones = array(4); //actualizar

            break;
        case 'Segeco2'://Xativa
            $datos->operaciones = array(3); //insertar

            break;
    }
    //lo metemos en el buffer de salida
    BbiBuffer::insert_item_into_buffer($datos, $paquete->getAyuntamiento());
    //Le metemos la fecha de envío en la tienda
    $context->field_chapa_fecha_envio['und'][0] = bbiLab_getFecha();
    drupal_set_message('Se ha enviado la chapa ' . $paquete->getTituloChapa(), 'status');
}

function bbiLab_enviarChapa_llenarPaquete($context) {
    $ahora = new DateTime(date('Y-m-d H:i:s'));
    $ahoraU = $ahora->format('U');
    
    $paquete = new BbiPaquete();
    $paquete->setTituloVial(bbiLab_getTitleById($context->field_chapa_vial['und'][0]['target_id']));
    $paquete->setTituloChapa($context->title);
    $paquete->setTipoChapa($context->field_chapa_tipo_chapa['und'][0]['value']);
    $paquete->setAyuntamiento(bbiLab_getUserById($context->field_ayuntamiento['und'][0]['target_id']));
    $paquete->setFechaEnvioChapa($ahoraU);
    return $paquete;
}
