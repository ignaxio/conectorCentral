<?php

function bbiLab_new_server_admin() { 
    $servidores = variable_get('codeserver_configs');
    $tiposDeSegeco =  BbiConector::getTiposDeSegeco();
    $nombreServidores = array();
    foreach ($servidores as $nombre => $datos) {
        $nombreServidores[$nombre] = $nombre;
    }    
   
    $form['bbiLab_titulo2'] = array(
        '#markup'       => '<h3>' . t('Crea un nuevo servidor.') . '</h3>'
    );
    $form['bbiLab_name'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Nombre del Ayuntamiento'),
        '#required'         => TRUE,
    );
    $form['bbiLab_serverUrl'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Dirección del servidor'),
        '#required'         => TRUE,
    );
    $form['bbiLab_tipoSegeco'] = array(
        '#type' => 'select', 
        '#title' => 'Selecciona el tipo de segeco',
        '#options' => $tiposDeSegeco,
        '#required' => TRUE,
    );
    $form['bbiLab_encrypt_key'] = array(
        '#type'             => 'textfield',
        '#title'            => t('La clave de encriptación'),
        '#required'         => TRUE,
        '#default_value'    => substr( md5(microtime()), 1, 16)
    );
    $form['bbiLab_encrypt_iv'] = array(
        '#type'             => 'textfield',
        '#title'            => t('La clave de enctiptación iv'),
        '#required'         => TRUE,
        '#default_value'    => substr( md5(microtime()), 1, 16)
    );
    $form['bbiLab_activo'] = array(
        '#type'             => 'checkbox',
        '#title'            => t('¿Ayuntamiento activo?'),
        '#required'         => TRUE,
        '#default_value'    => FALSE,
    );
    return system_settings_form($form);
}

function bbiLab_new_server_admin_validate($form, &$form_state) {
    $configs                = variable_get('codeserver_configs');
    $nombreDelAyuntamiento  = $form_state['values']['bbiLab_name'];
    $aytoActivo             = $form_state['values']['bbiLab_activo'];     
    $DireccionDelServidor   = $form_state['values']['bbiLab_serverUrl'];
    $tipoSegeco             = $form_state['values']['bbiLab_tipoSegeco'];
    $encryptKey             = $form_state['values']['bbiLab_encrypt_key'];
    $encryptIv              = $form_state['values']['bbiLab_encrypt_iv']; 
        
    //Creamos el nombre del conector
    $nombreDelServidor = 'conector' . $nombreDelAyuntamiento;
    //Vamos a construit el array de configuración del servidor.
    $configs[$nombreDelServidor] = array(
        'aytoName'      => $nombreDelAyuntamiento,
        'aytoActivo'    => $aytoActivo,
        'server'        => $DireccionDelServidor,
        'tipoSegeco'    => $tipoSegeco,
        'encrypt_key'   => $encryptKey,
        'encrypt_iv'    => $encryptIv,
    );
    
    // Save the updated configs.    
    variable_set('codeserver_configs', $configs);
    drupal_flush_all_caches();
    drupal_set_message(t('Configuración guardada correctamente para el ' . $nombreDelServidor));    
}
