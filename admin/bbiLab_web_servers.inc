<?php

function bbiLab_web_servers_admin() {
    $header = array(t('Server name'), t('Segeco type'), t('Max time execute a queue'), t('Ayto name'), t('Server path'), t('Activo'), t('Edit'));
    $rows = array();
    $config = variable_get('codeserver_configs');

    foreach ($config as $nombre => $server) {
        //Creamos un enlace para poder editar el servidor.
        $enlaceEditar = l(t('Edit'), 'admin/config/services/web_server_config/' . $nombre);
        //Creamos un enlace al segeco.
        $enlaceSegeco = l($server['server'], $server['server']);
        //Si el servidor es activo mostramos true en vez de 1
        $server['aytoActivo'] = $server['aytoActivo'] ? 'TRUE' : 'FALSE';
        //Vamos a traducir los tipos de segeco
        $rows[] = array($nombre, $server['tipoSegeco'], $server['max_time_queue'], $server['aytoName'], $enlaceSegeco, $server['aytoActivo'], $enlaceEditar);
    }
  return theme('table', array('header' => $header, 'rows' => $rows));
}

