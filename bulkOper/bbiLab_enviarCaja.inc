<?php

/**
 * Funcion que crea un form intermedio en el bulk operation donde le ponemos 
 * que seleccione el veterinario y ayuntamiento que quiere enviar la caja
 */
function bbiLab_enviarCaja_form($settings, &$form_state) {
    //Voy a coger los ayuntamientos que tenemos en la tienda
    $ayuntamientos = BbiConector::getAyuntamientos();
    $veterinarios = BbiConector::getVeterinarios();
    //Añado la opción ninguno a los veterinarios por si la caja es de heces 
    //y va ha el usuario ayto
    $veterinarios[0] = 'Ninguno, heces';
    $form = array();
    $form['ayuntamiento'] = array(
        '#type' => 'select', 
        '#title' => 'Selecciona el ayuntamiento',
        '#options' => $ayuntamientos,
        '#required' => TRUE,
        '#default_value' => isset($settings['settings']['ayuntamiento']) ? 
        $settings['settings']['ayuntamiento'] : '',
    );
    $form['veterinario'] = array(
        '#type' => 'select', 
        '#title' => 'Selecciona un veterinario',
        '#options' => $veterinarios,
        '#required' => TRUE,
        '#default_value' => isset($settings['settings']['veterinario']) ? 
        $settings['settings']['veterinario'] : '',
    );
    $form['trackingEnvio'] = array(
        '#type' => 'textfield', 
        '#title' => 'Inserta el tracking de envío',
        '#required' => TRUE
    );
    return $form;
}


/**
 * Funcion que recoge los datos del formulario intermedio enviar caja.
 * Aquí se verifica que el veterinario pertenezca a un ayuntamiento
 * llamamos a la función que se encarga de todas las acciones que hay que 
 * realizar al enviar una caja
 */
function bbiLab_enviarCaja_submit($form, $form_state) {    
    //Aquí podemos checkear si el veterinario existe en ese ayuntamiento...    
    //$result = bbiLab_checkVeterinarioIsOnAyuntamiento($form_state['values']
    //['veterinario'], $form_state['values']['ayuntamiento'])
    
    //Aquí hacemos las comprovaciones.....
    
    //Enviamos los datos seleccionados en el form
    $context                        = array(); 
    $context['ayuntamiento']        = $form_state['values']['ayuntamiento'];
    $context['veterinario']         = $form_state['values']['veterinario'];
    $context['trackingEnvio']       = $form_state['values']['trackingEnvio'];
    return $context; //Note, return value here must be an array.
}

/**
 * Mensaje final al enviar una caja
 */
function bbiLab_enviarCaja(&$entity, $context = array()) {
    $idVeterinario      = $context['veterinario'];
    $idAyuntamiento     = $context['ayuntamiento'];
    $trackingEnvio      = $context['trackingEnvio'];
     
    
    //Modificamos los campos de la caja 
    bbiLab_enviarCaja_modificarCamposDeCaja($entity, $idVeterinario, 
            $idAyuntamiento, $trackingEnvio); 
    //Vamos a buscar los viales que hay en esta caja
    $viales = BbiCaja::getViales($entity->nid);
    //Ahora modificamos los campos de los viales de la tienda
    foreach ($viales as $idVial => $titleVial) {
        $vial = bbiLab_enviarCaja_modificarCamposDeVial($idVial, $idVeterinario, 
                $idAyuntamiento); 
        bbiLab_enviarCaja_llenarPaquete($vial); 
    }      
    
    //Ya esta, con esto se ha hecho todo para enviar las cajas, solo esperar 
    //que se ejecute el cron    
    
    
    //dpm($entity);
    $message = t('La caja ha sido enviada al veterinario  %veterinario de la '
            . 'localidad de  %ayuntamiento', array(
    '%veterinario' => bbiLab_getUserById($idVeterinario),
    '%ayuntamiento' => bbiLab_getUserById($idAyuntamiento),
  ));
  drupal_set_message($message);
}

function bbiLab_enviarCaja_llenarPaquete($vial)  {
    $paquete = new BbiPaquete();
    $paquete->setTituloVial($vial->title);
    $paquete->setTipoVial($vial->field_vial_tipo_de_vial['und'][0]['value']);
    $paquete->setAyuntamiento(bbiLab_getUserById($vial->field_ayuntamiento['und'][0]['target_id']));
    $paquete->setVeterinario(isset($vial->field_veterinario['und'][0]['target_id']) ? bbiLab_getUserById($vial->field_veterinario['und'][0]['target_id']) : NULL);
    
    
    //Creo un objeto y le meto el array en el atributo vial, 
    //así cuando llege al segeco sabra que es un vial
    $datos = new stdClass();
    $datos->paquete = $paquete;
    $datos->operaciones = array(1);
    //Creamos el item para que el cron lo lance el solo
    BbiBuffer::insert_item_into_buffer($datos, $paquete->getAyuntamiento());
}

/**
 * Funcion que modifica los campos del vial cunado se envia a un veterinario.
 * Se mete el veterinario y el ayuntamiento
 * Se marca como no sincronizado
 * y se marca en localizacion veterinario
 */
function bbiLab_enviarCaja_modificarCamposDeVial($idVial, $idVeterinario, 
        $idAyuntamiento) {   
    $vial = node_load($idVial);
    
    //Le metemos el ayuntamiento
    $vial->field_ayuntamiento['und'][0]['target_id'] = $idAyuntamiento;
    //Lo marcamos que está en veterinario
    $vial->field_vial_localizacion['und'][0]['value'] = 3;
    //si el idVeterinario es 0 no tiene veterinario
    if ($idVeterinario) {
        $vial->field_veterinario['und'][0]['target_id'] = (int)$idVeterinario;  
    }else {
        $vial->field_veterinario = array(); 
    }
    
    node_save($vial); 
    
    return $vial;
}

/**
 * Funcion que modifica campos de una caja
 * Pone localización en veterinario
 * Le mete el veterinario y el ayuntamiento
 * Le pone fecha de envio
 */
function bbiLab_enviarCaja_modificarCamposDeCaja($entity, $idVeterinario, 
        $idAyuntamiento, $trackingNumber) {
    //Marcamos la caja en localización veterinario
    $entity->field_caja_localizacion['und'][0]['value'] = 3;  
    //Metemos el veterinario y el ayuntamiento
    $entity->field_ayuntamiento['und'][0]['target_id'] = $idAyuntamiento;  
    //si el idVeterinario es 0 no tiene veterinario
    if ((int)$idVeterinario) $entity->field_veterinario['und'][0]['target_id'] 
        = $idVeterinario;  
    else $entity->field_veterinario = array(); 
    //Metemos la fecha de envío.
    $entity->field_caja_fecha_envio['und'][0] = bbiLab_getFecha();
    //Metemos el número de tracking
    $entity->field_caja_tracking_de_envio['und'][0]['value'] = $trackingNumber;  
}
