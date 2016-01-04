<?php

function bbiLab_server_config_admin($form, &$form_state) {  
    $tiposDeSegeco      = BbiConector::getTiposDeSegeco();
    $nombreDelServidor  = $form_state['build_info']['args'][0];
    $funcionesActivas   = array();
    $ipActivas          = array();
    $form               = array();
    $configuracion      = variable_get('codeserver_configs');
    $configuracion      = $configuracion[$nombreDelServidor];            
    $enlaceVolver       = l(t('Volver'), 'admin/config/services/web_server_config');
    
    $form['descripcion'] = array(
        '#markup'       => $enlaceVolver . '<br><h3>' . t('Página de configuración del servidor.') . '</h3><p>' . t('Si quieres eliminar el servidor deja en blanco el nombre.') . '</p>'
    );
    $form['bbiLab_name'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Nombre del Ayuntamiento'),
        '#default_value'    => $configuracion['aytoName'], 
        '#required'         => FALSE,
    );
    $form['bbiLab_serverUrl'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Dirección del servidor'),
        '#default_value'    => $configuracion['server'], 
        '#required'         => FALSE,
    );
    $form['bbiLab_tipoSegeco'] = array(
        '#type'             => 'select', 
        '#title'            => 'Selecciona el tipo de segeco',
        '#options'          => $tiposDeSegeco,
        '#default_value'    => $configuracion['tipoSegeco'], 
        '#required'         => TRUE,
    );
    $form['bbiLab_encrypt_key'] = array(
        '#type'             => 'textfield',
        '#title'            => t('La clave de encriptación'),
        '#default_value'    => $configuracion['encrypt_key'], 
        '#required'         => FALSE,
    );
    $form['bbiLab_encrypt_iv'] = array(
        '#type'             => 'textfield',
        '#title'            => t('La clave de enctiptación iv'),
        '#default_value'    => $configuracion['encrypt_iv'], 
        '#required'         => FALSE,
    );    
    $form['bbiLab_activo'] = array(
        '#type'             => 'checkbox',
        '#title'            => t('¿Ayuntamiento activo?'),
        '#required'         => FALSE,
        '#default_value'    => $configuracion['aytoActivo'],
    );
    $form['bbiLab_max_time_queue'] = array(
        '#type'             => 'textfield',
        '#title'            => t('Tiempo máximo de ejecución de las colas en segundos'),
        '#default_value'    => $configuracion['max_time_queue'], 
        '#required'         => FALSE,
    ); 
    return system_settings_form($form);
}

function bbiLab_server_config_admin_validate($form, &$form_state) {
    $configs                = variable_get('codeserver_configs');
    $funcionesNuevas        = array();
    $nombreDelAyuntamiento  = $form_state['values']['bbiLab_name'];
    $aytoActivo             = $form_state['values']['bbiLab_activo']; 
    $DireccionDelServidor   = $form_state['values']['bbiLab_serverUrl'];
    $tipoSegeco             = $form_state['values']['bbiLab_tipoSegeco'];
    $encryptKey             = $form_state['values']['bbiLab_encrypt_key'];
    $encryptIv              = $form_state['values']['bbiLab_encrypt_iv'];
    $maxTimeQueue           = $form_state['values']['bbiLab_max_time_queue'];
    
    
    //Creamos el nombre del conector
    $nombreDelServidor = 'conector' . $nombreDelAyuntamiento;
    //Vamos a construit el array de configuración del servidor.
    $configs[$nombreDelServidor] = array(
        'aytoName'          => $nombreDelAyuntamiento,
        'aytoActivo'        => $aytoActivo,
        'server'            => $DireccionDelServidor,
        'tipoSegeco'        => $tipoSegeco,
        'encrypt_key'       => $encryptKey,
        'encrypt_iv'        => $encryptIv,
        'max_time_queue'    => $maxTimeQueue,
    );
    // Save the updated configs.    
    variable_set('codeserver_configs', $configs);
    drupal_set_message(t('Configuración guardada correctamente para el ' . $nombreDelServidor));
    
}
