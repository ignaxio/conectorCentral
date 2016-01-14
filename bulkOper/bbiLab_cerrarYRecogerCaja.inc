<?php




function bbiLab_cerrarYRecogerCaja_form($settings, &$form_state) {
    $form = array();    
    $form['trackingRecogida'] = array(
        '#type' => 'textfield', 
        '#title' => 'Inserta el tracking de recogida',
        '#required' => TRUE
    );
    return $form;
}

function bbiLab_cerrarYRecogerCaja_submit($form, $form_state) {    
    
    $return = array(); 
    $return['trackingRecogida'] = $form_state['values']['trackingRecogida'];
    return $return; //Note, return value here must be an array.    
}


/**
 * Mensaje final al enviar una caja
 */
function bbiLab_cerrarYRecogerCaja(&$entity, $context = array()) {
    $trackingNumber = $context['trackingRecogida'];
    $ayuntamiento = $entity->field_ayuntamiento['und'][0]['target_id'];
    
    //Modificamos los campos de la caja
    bbiLab_cerrarYRecogerCaja_modificarCamposDeLaCaja($entity, $trackingNumber);
    //Vamos a buscar los viales que hay en esta caja
    $viales = BbiCaja::getViales($entity->nid);
    
    //Modificamos los campos del vial
    foreach ($viales as $idVial => $titleVial) {
        bbiLab_cerrarYRecogerCaja_modificarCamposDeVial($idVial);
        drupal_set_message("Se han modificado los campos del vial $titleVial", 
                'status');
    } 
    
    //Ahora hay que eliminar los viales vacios de esta caja que estan en su ayto
    $vialesVacios = BbiCaja::getViales($entity->nid, 'vacios');   
    bbiLab_cerrarYRecogerCaja_eliminarVialDeCajaCerrada($vialesVacios, 
            $ayuntamiento);
    
    $message = t('La caja se ha cerrado correctamente con el número de '
            . 'tracking %tracking', array('%tracking' => $trackingNumber));
  drupal_set_message($message);
}

/**
 * Función que realiza los cambios necesarios en los campos de la caja 
 * cuando se cierra y recoge
 * @param type $idCaja
 * @param type $trackingNumber
 */
function bbiLab_cerrarYRecogerCaja_modificarCamposDeLaCaja($caja, 
        $trackingNumber) {
    //Marcamos la caja en localización laboratorio
    $caja->field_caja_localizacion['und'][0]['value'] = 2; 
    //Metemos la fecha de cierre.
    $caja->field_caja_fecha_cierre['und'][0] = bbiLab_getFecha();
    //Metemos el número de tracking
    $caja->field_caja_tracking_de_recogida['und'][0]['value'] = $trackingNumber; 
    drupal_set_message("Se han modificado los campos de la caja $caja->title", 
            'status');
}

/**
 * Función que realiza los cambios necesarios en los campos de los viales 
 * cuando se cierra y recoge
 * @param type $idVial
 */
function bbiLab_cerrarYRecogerCaja_modificarCamposDeVial($idVial) {
    $vial = node_load($idVial);
    //Le metemos la localización en laboratorio
    $vial->field_vial_localizacion['und'][0]['value'] = 2;
    //Le metemos el estado pendiente de recepción si tiene fecha de extracción
    if($vial->field_vial_fecha_de_extracci_n['und'][0]['value']) {
        $vial->field_vial_estado['und'][0]['value'] = 1;            
    }
    node_save($vial);     
}

function bbiLab_cerrarYRecogerCaja_eliminarVialDeCajaCerrada($vialesVacios, 
        $codigoAyto) {
    $result = false;
    $segeco = bbiLab_getUserById($codigoAyto);
    //Lo enviamos al segeco
    $conector = 'conector' . $segeco;
    
    //Hacemos la conexión y lo enviamos
    $remote = new CodeServer($conector);
    $remote->config->request_timeout = 10;
    $respuesta = $remote->conectorSegeco_eliminarVialesVaciosDeUnaCaja(
            $vialesVacios);   
            
            
    //Si todo a ido bien mandamos el mensaje
    if($respuesta) {    
        drupal_set_message($respuesta, 'status');
    } else {   
        $strViales = '';
        $hoy = date('d-m-Y H:i');
        foreach ($vialesVacios as $vial) {
            $strViales .= $vial . ', ';
        }
        $mensaje = "No se han eliminado los viales vacios ( $strViales ) del "
                . "segeco $segeco cuando se cerro la caja el dia $hoy";
        watchdog('Cerrar y recoger', $mensaje, NULL, WATCHDOG_ERROR);
        bbiLab_sendEmail('error', $mensaje);
        drupal_set_message($mensaje, 'error');
    }   
    return $respuesta;
}
