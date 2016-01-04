<?php

function bbiLab_creadorDeViales_form($form, &$form_state) {
    $form['descripcion'] = array(
        '#markup'       => bbiLab_creadorDeViales_setTextoDescripcion()
    );
    $form['pre'] = array(
        '#type'         => 'textfield',
        '#title'        => 'Prefijo (CA-AAA)',
        '#size'         => 15,
        '#maxlength'    => 6,
        '#required'     => TRUE,
    );
    $form['num1'] = array(
        '#type'         => 'textfield',
        '#title'        => 'Primer vial',
        '#size'         => 15,
        '#maxlength'    => 4,
        '#required'     => TRUE,
    );
    $form['num2'] = array(
        '#type'         => 'textfield',
        '#title'        => 'Último vial',
        '#size'         => 15,
        '#maxlength'    => 4,
        '#required'     => TRUE,
    );
    $form['tipoVial'] = array(
        '#title'        => t('Selecciona el tipo de vial'),
        '#type'         => 'select',
        '#default_value' => 'CA',
        '#options'      => array(
            'CA' => 'Sangre',
            'HB' => 'Isopos',
            'HC' => 'Heces'
        ),
        '#required'     => TRUE,
    );
    $form['tipoDeCaja'] = array(
        '#title'        => t('Selecciona el tipo de caja'),
        '#type'         => 'select',
        '#default_value' => 0,
        '#options'      => array(
            0   => 'Sin caja',
            100 => 'Caja de sangre de 100',
            50  => 'Caja de sangre de 50',
            12  => 'Caja de heces de 12'
        ),
        '#required'     => TRUE,
    );
    
    $form['caja'] = array(
        '#type'         => 'textfield',
        '#title'        => 'Caja',
        '#size'         => 15,
        '#maxlength'    => 10,
    );

    $form['submit'] = array(
        '#type'         => 'submit',
        '#value'        => 'Insertar',
        '#attributes' => array('class' => array('btn')),
    );
    return $form;
}

function bbiLab_creadorDeViales_form_validate($form, &$form_state) {    
    $pre                        = $form_state['values']['pre'];
    $num1                       = $form_state['values']['num1'];
    $num2                       = $form_state['values']['num2'];
    $tipoVial                   = $form_state['values']['tipoVial'];
    $caja                       = $form_state['values']['caja'];
    $vialesQueCabenEnLaCaja     = $form_state['values']['tipoDeCaja'];
    $numeroViales               = ($num2 - $num1) + 1;
    $tieneCaja                  = ($caja == '') ? FALSE : TRUE;
        
    $form_state['values']['numeroViales']   = $numeroViales;
    $form_state['values']['tieneCaja']      = $tieneCaja;
    
    if (!is_numeric($num1)){
        form_set_error('num1', 'El número de vial tiene que ser un número');
    }
    if (!is_numeric($num2)){
        form_set_error('num2', 'El número de vial tiene que ser un número');
    }    
    $pre1 = substr($pre, 0, -4);  
    if($pre1 != $tipoVial) {
        form_set_error('pre', 'Checkea el tipo de vial...');
    }
    if($num1 > $num2) {
        form_set_error('num1', 'el primer vial no puede ser menos que el segundo');
        form_set_error('num2', 'el primer vial no puede ser menos que el segundo');
    }     
    //Vamos a verificar si existe algún vial con ese título
    for ($i = $num1; $i <= $num2; $i++) {  
        $titulo = $pre . $i;
        if(bbiLab_getIdNodeByTitle($titulo)) {
            form_set_error('num1', "El vial $titulo ya existe en la base de datos");
        }
    }       
    //si tiene caja hacemos las comprobaciones.
    if ($tieneCaja) {        
        //Vamos a verificar si existe la caja
        if(bbiLab_getIdNodeByTitle($caja)) {
            form_set_error('caja', "La caja $caja ya existe en la base de datos");
        }
        //aseguramos que el número de viales que se quieren meter 
        //no sea superior a los que admite la caja
        if($numeroViales > $vialesQueCabenEnLaCaja) {
            form_set_error('num1', "El número de viales es mayor de "
                    . "$vialesQueCabenEnLaCaja, no cabe en la caja");
            form_set_error('num2', "El número de viales es mayor de "
                    . "$vialesQueCabenEnLaCaja, no cabe en la caja");
        }
    }
    
    
}

function bbiLab_creadorDeViales_form_submit($form, &$form_state) {
    $pre            = $form_state['values']['pre'];
    $num1           = $form_state['values']['num1'];
    $num2           = $form_state['values']['num2'];
    $tipoVial       = $form_state['values']['tipoVial'];
    $tituloCaja     = $form_state['values']['caja'];
    $numeroViales   = $form_state['values']['numeroViales'];
    $tieneCaja      = $form_state['values']['tieneCaja'];
    
    if($tieneCaja) {
        //Creamos la caja
        bbiLab_creadorDeViales_CrearCaja($tituloCaja, $numeroViales);        
    }
    // Creamos los viales
    $viales = bbiLab_creadorDeViales_CrearViales($num1, $num2, $pre, 
            $tipoVial);
    //Si hay que meterlos en caja lo hacemos....
    if($tieneCaja) {
        bbiLab_creadorDeViales_meterVialesEnCaja($viales, 
                bbiLab_getIdNodeByTitle($tituloCaja));        
    }
}

function bbiLab_creadorDeViales_meterVialesEnCaja($viales, $idCaja) {
    $contador = 1;
    
    foreach ($viales as $key => $tituloVial) {
        $vial = node_load($key);
        $fc_item = entity_create('field_collection_item', array(
            'field_name' => 'field_vial_caja'));
        $fc_item->setHostEntity('node', $vial);

        $collection = entity_metadata_wrapper('field_collection_item', $fc_item);
        $collection->field_caja_fisica->set($idCaja);
        $collection->field_vial_caja_posicion->set($contador);
        $contador ++;
        $collection->save();
        
    }
    $tituloCaja = bbiLab_getTitleById($idCaja);
    drupal_set_message("Se han metido los viales en la caja $tituloCaja", 
            'status');
    
}


function bbiLab_creadorDeViales_CrearViales($num1, $num2, $pre, 
        $tipoVial) {        
    $viales = array();
    $primerVial = '';        
    $segundoVial = '';        
    $cont = 0;
    for ($i = $num1; $i <= $num2; $i++) {  
        
        $titulo = $pre . bbiLab_creadorDeViales_setCeros($i);
        if($i == $num1) $primerVial = $titulo;
        if($i == $num2) $segundoVial = $titulo;
        $values = array(
                'type'      => 'vial',
                'uid'       => 1,
                'status'    => 1,
                'comment'   => 0,
                'promote'   => 0,
        );
        $vial = entity_create('node', $values);        
        $ewrapper = entity_metadata_wrapper('node', $vial);
        $ewrapper->field_vial_tipo_de_vial
                ->set(bbiLab_creadorDeViales_getTipoVial($tipoVial));
        $ewrapper->field_vial_pistoleteado->set(1);
        $ewrapper->title->set($titulo); 
        //cargamos un array para devolverselo
        $ewrapper->save();
        $viales[$ewrapper->getIdentifier()] = $ewrapper->label();
        $cont++;
    }
    
    drupal_set_message("Se han creado $cont, los vials desde $primerVial hasta "
            . "del vial $segundoVial", 'status');
    return $viales;
}



function bbiLab_creadorDeViales_CrearCaja($tituloCaja, $numeroViales) {
    $values = array(
            'type'      => 'caja',
            'uid'       => 1,
            'status'    => 1,
            'comment'   => 0,
            'promote'   => 0,
    );
    $entity = entity_create('node', $values);
    //$entity->field_caja_numero_de_viales['und'][0]['value'] = $numeroViales;
    $ewrapper = entity_metadata_wrapper('node', $entity);    
    $ewrapper->field_caja_numero_de_viales->set($numeroViales);     
    $ewrapper->title->set($tituloCaja);     
    $ewrapper->save();
    drupal_set_message("Se ha creado la caja $tituloCaja .", 'status'); 
}

function bbiLab_creadorDeViales_getTipoVial($tipoVial) {
    $tipos = array(
        1   => 'CA',
        2   => 'HB',
        3   => 'HC'
    );    
    foreach ($tipos as $key => $tipo) {
        if($tipoVial == $tipo) return $key;
    }
}

function bbiLab_creadorDeViales_setCeros($numero) {
    //miramos los dígitos que tiene y le agrgamos los necesarios hasta 4.
    $digitos = strlen($numero);
    $numeroConCeros = $numero;
    switch ($digitos) {
        case 1:
            $numeroConCeros = '000' . "$numero";
            break;
        case 2:
            $numeroConCeros = '00' . "$numero";
            break;
        case 3:
            $numeroConCeros = '0' . "$numero";
            break;
    }
    return $numeroConCeros;
}

function bbiLab_creadorDeViales_setTextoDescripcion() {
    $texto = ""
            . "<div class='jumbotron'>"
                . "<h2>Importante !!!!!!</h2>"
                . "<p>Para insertar viales has de introducir el codigo del tipo "
            . "de vial. eje:</p>"
                . "<ol>"
                    . "<li>Sangre = CA-</li>"
                    . "<li>Isopos = HB-</li>"
                    . "<li>Heces = HC-</li>"
                . "</ol>"
                . "<p>Seguidamente hay que introducir el prefijo que consta de "
            . "3 letras en mayusculas.</p>"
                . "<p>El número de vial consta de 4 dígitos insertando el "
            . "primero y el último incluisves.</p>"
                . "<p>La caja es un campo opcional, si quieres insertar solo "
            . "viales omites este último. La caja que insertes va ha ser creada, "
            . "no puede existir.</p>"
            . "</div>"
            . "";
    return $texto;
}