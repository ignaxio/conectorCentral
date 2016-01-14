<?php

function bbiLab_creadorDeChapas_form($form, &$form_state) {
    $ayuntamientos = BbiConector::getAyuntamientosActivos();
    
    $form['descripcion'] = array(
        '#markup'       => bbiLab_creadorDeChapas_setTextoDescripcion()
    );
    $form['ayuntamiento'] = array(
        '#type' => 'select', 
        '#title' => 'Selecciona el ayuntamiento',
        '#options' => $ayuntamientos,
        '#required' => TRUE,
    );
    $form['tipo'] = array(
        '#type' => 'select',
        '#title' => 'Tipo de chapa',
        '#options' => array(1 => 'Chapa grande', 2 => 'Chapa pequeña'),
        '#required' => TRUE,
    );
    $form['cantidad'] = array(
        '#type'         => 'textfield',
        '#title'        => 'Cantidad de chapas',
        '#size'         => 20,
        '#maxlength'    => 3,
        '#required'     => TRUE,
    );
    $form['submit'] = array(
        '#type'         => 'submit',
        '#value'        => 'Insertar',
        '#attributes' => array('class' => array('btn')),
    );
    return $form;
}

function bbiLab_creadorDeChapas_form_validate($form, &$form_state) {    
    $cantidad                   = $form_state['values']['cantidad'];        
    
    if (!is_numeric($cantidad)){
        form_set_error('cantidad', 'La cantidad tiene que ser un número');
    }
}

function bbiLab_creadorDeChapas_form_submit($form, &$form_state) {
    $ayuntamiento               = (int)$form_state['values']['ayuntamiento'];
    $tipo                       = (int)$form_state['values']['tipo'];
    $cantidad                   = (int)$form_state['values']['cantidad']; 
    
    //Vamos a ver de que ayuntamiento son
    switch ($ayuntamiento) {
        case 140://Xativa
            $prefijo = 'Xat - ';
            //Cogemos el contador que toca
            $contador = bbiLab_creadorDeChapas_getContadorSiguiente(
                    $ayuntamiento);
            //Ahora ya poodemos crear las chapas...            
            bbiLab_creadorDeChapas_crearChapas($cantidad, $tipo, $prefijo, 
                    $contador, $ayuntamiento);
            break;
    }
}


function bbiLab_creadorDeChapas_crearChapas($cantidad, $tipo, $prefijo, 
        $contador, $ayuntamiento, $digitoControl = NULL, 
        $fechaPeticion = NULL, $fechaEnvio = NULL, $finalizadoOK = NULL, 
        $idVial = NULL) {
    //Se crean tantas chapas como cantidad haya marcado.
    for ($i = 0; $i < (int)$cantidad; $i++) {
        //Hay que aumentar el contador y que tenga 5 dígitos
        $contador = bbiLab_creadorDeChapas_transformTo5Digitos((int)$contador + 1); 
        $titulo     = $prefijo . $contador . $digitoControl;     
        
        $values = array(
                'type'      => 'chapa',
                'uid'       => 1,
                'status'    => 1,
                'comment'   => 0,
                'promote'   => 0,
        );
        $entity = entity_create('node', $values);
        $ewrapper = entity_metadata_wrapper('node', $entity);  
        $ewrapper->title->set($titulo);
        if($tipo != NULL)           $ewrapper->field_chapa_tipo_chapa
            ->set($tipo);   
        if($prefijo != NULL)        $ewrapper->field_chapa_prefijo
            ->set($prefijo);  
        if($contador != NULL)       $ewrapper->field_chapa_contador
            ->set($contador);   
        if($digitoControl != NULL)  $ewrapper->field_chapa_digito_control
            ->set($tipo);     
        if($fechaPeticion != NULL)  $ewrapper->field_chapa_fecha_peticion
            ->set($fechaPeticion); 
        if($fechaEnvio != NULL)     $ewrapper->field_chapa_fecha_envio
            ->set($fechaEnvio);    
        if($finalizadoOK != NULL)   $ewrapper->field_chapa_finalizado_ok
            ->set($finalizadoOK); 
        if($ayuntamiento != NULL)   $ewrapper->field_ayuntamiento
            ->set((int)$ayuntamiento);
        if($idVial != NULL)         $ewrapper->field_chapa_vial
            ->set((int)$idVial);
        $ewrapper->save();  
        
        if(node_load($ewrapper->getIdentifier())) {
            $ayto = bbiLab_getUserById((int)$ayuntamiento);
            drupal_set_message("Se ha creado la chapa $titulo del ayuntamiento "
                    . "$ayto.", 'status');
        } else {
            drupal_set_message("No se ha podido crear la chapa $titulo, avise a "
                    . "el programador...", 'error');
        }
    }
}

/**
 * 
 * @param int $ayuntamiento
 * @return string
 */
function bbiLab_creadorDeChapas_getContadorSiguiente($ayuntamiento) {
    $contadorMasAlto = db_query(
            "SELECT MAX(fcc.field_chapa_contador_value) as ultimoContador "
            . "FROM field_data_field_chapa_contador fcc, "
            . "node n, "
            . "field_data_field_ayuntamiento fa "
            . "WHERE n.nid = fcc.entity_id AND fa.entity_id = n.nid "
            . "AND fa.field_ayuntamiento_target_id = $ayuntamiento"
            )->fetchCol();
    //Si está vacio significa que es el primero del ayuntamiento, 
    //se le mete el 00001
    $contadorMasAlto =  ($contadorMasAlto[0] == NULL) ? 3500 : 
            (int)$contadorMasAlto[0];
    return $contadorMasAlto;
}

/**
 * Función que se le pasa un número y le mete los ceros necesarios 
 * delane hasta tener 5 dígitos.
 * @param string $stringNumero
 * @return string
 */
function bbiLab_creadorDeChapas_transformTo5Digitos($stringNumero) {
    $numeroFinal = '';
    $digitos = strlen($stringNumero);
    switch ($digitos) {
        case 1:
            $numeroFinal = '0000' . $stringNumero;
            break;
        case 2:
            $numeroFinal = '000' . $stringNumero;
            break;
        case 3:
            $numeroFinal = '00' . $stringNumero;
            break;
        case 4:
            $numeroFinal = '0' . $stringNumero;
            break;
        case 5:
            $numeroFinal = $stringNumero;
            break;       
    }
    return $numeroFinal;
}


function bbiLab_creadorDeChapas_setTextoDescripcion() {
    $texto = ""
            . "<div class='jumbotron'>"
                . "<h2>Página para crear chapas</h2>"
                . "<p>Solo hay que seleccionar el ayntamiento al que "
            . "pertenezacan y la cantidad.</p>"
            . "</div>"
            . "";
    return $texto;
}