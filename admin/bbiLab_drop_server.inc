<?php

function bbiLab_drop_server_admin() { 
    $servidores      = codeserver_configs();
    $nombreServidores = array();
    foreach ($servidores as $nombre => $datos) {
        $nombreServidores[$nombre] = $nombre;
    }
    
    $form['bbiLab_titulo1'] = array(
        '#markup'       => '<h3>' . t('Elimina un servidor.') . '</h3>'
    );
          
    $form['bbiLab_servidores_activos'] = array(
        '#type'             => 'checkboxes',
        '#options'          => drupal_map_assoc($nombreServidores),
        '#title'            => t('¿Selecciona el servidor que quieras eliminar?'),
    );
    return system_settings_form($form);
}

function bbiLab_drop_server_admin_validate($form, &$form_state) {
    $configs                = array();
   
    $servidoresParaeliminar = $form_state['values']['bbiLab_servidores_activos'];    
    $opcionEliminar         = false;
    //print_r($servidoresParaeliminar);
    
    //Vamos a comprobar si se quiere eliminar algún servidor.
    foreach ($servidoresParaeliminar as $nombreServidor => $servidorParaeliminar) {
        
        if($servidorParaeliminar) {
            $configs = codeserver_configs();
            if (isset($configs[$nombreServidor])) {
                unset($configs[$nombreServidor]);
                codeserver_configs($configs);
                drupal_flush_all_caches();
                $servidoresEliminados .= ', ' . $nombreServidor;
                drupal_set_message(t('Se han eliminado correctamente los servidores ' . $servidoresEliminados));
            }
        }
    }
}
