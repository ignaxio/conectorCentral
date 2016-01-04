<?php

/**
 * He creado esta página para probar las funciones y ver los valores devueltos
 */
function bbiLab_menu() {
    $items['BBI/pagina-de-pruebas'] = array(
        'title'             => 'Página para probar código',
        'page callback'     => 'bbiLab_pruebas',
        'access callback'   => 'user_access',
        'access arguments'  => array('access content'),
        'type'              => MENU_CALLBACK,
    );
    $items['BBI/creadorDeViales'] = array(
        'title'             => 'Cargador de viales masivo',
        'description'       => 'Página que crea viales y cajas',
        'page callback'     => 'drupal_get_form',
        'access callback'   => 'user_access',
        'page arguments'    => array('bbiLab_creadorDeViales_form'),
        'access arguments'  => array('access content'),
        'type'              => MENU_CALLBACK,
        'file'              => 'pages/bbiLab_creadorDeViales.inc'
    );
    $items['BBI/creadorDeChapas'] = array(
        'title'             => 'Creador de chapas',
        'description'       => 'Página que crea chapas a tope',
        'page callback'     => 'drupal_get_form',
        'access callback'   => 'user_access',
        'page arguments'    => array('bbiLab_creadorDeChapas_form'),
        'access arguments'  => array('access content'),
        'type'              => MENU_CALLBACK,
        'file'              => 'pages/bbiLab_creadorDeChapas.inc'
    );
    $items['admin/config/services/web_server_config'] = array(
        'title'             => t('Web servers'),
        'description'       => t('Configure web servers'),
        'page callback'     => 'bbiLab_web_servers_admin',
        'access arguments'  => array('access content'),
        'file'              => 'admin/bbiLab_web_servers.inc'
    );
    $items['admin/config/services/web_server_config/html'] = array(
        'title' => t('web servers'),
        'weight' => 1,
        'type' => MENU_DEFAULT_LOCAL_TASK,
    );
    $items['admin/config/services/web_server_config/new'] = array(
        'title' => t('New web server'),
        'description' => t('Crear un nuevo servidor'),
        'page callback' => 'drupal_get_form',
        'page arguments' => array('bbiLab_new_server_admin'),
        'access arguments'  => array('access content'),
        'weight' => 2,
        'type' => MENU_LOCAL_TASK,
        'file' => 'admin/bbiLab_new_server.inc',
    );
    $items['admin/config/services/web_server_config/delete'] = array(
        'title' => t('Drop a web server'),
        'description' => t('Eliminar un servidor de la configuración'),
        'page callback' => 'drupal_get_form',
        'page arguments' => array('bbiLab_drop_server_admin'),
        'access arguments'  => array('access content'),
        'weight' => 3,
        'type' => MENU_LOCAL_TASK,
        'file' => 'admin/bbiLab_drop_server.inc',
    );
    //Hay que crear tantos servidores como tenga el cliente
    $codeserver_configs = variable_get('codeserver_configs');
    $contador = 4;
    foreach ($codeserver_configs as $nombreServidor => $configuraciones) {
        $items['admin/config/services/web_server_config/' . $nombreServidor] = array(
            'title'             => t('Configurar ' . $nombreServidor),
            'description'       => t('Configuración del cliente ' . $nombreServidor),
            'page callback'     => 'drupal_get_form',
            'page arguments'    => array('bbiLab_server_config_admin', $nombreServidor), 
            'access arguments'  => array('access content'),
            'weight'            => $contador,
            'type'              => MENU_CALLBACK,
            'file'              => 'admin/bbiLab_server_config.inc'
        );
        $contador++;
    }
    return $items;
}

//Funcion para imprimir variables 
function bbiLab_pruebas() {
    print_r(BbiCaja::getIdCaja('ca-aaa8848'));
    
    
    
    
    
//    $serversNames = BbiConector::getServerNameActivos();
//    foreach ($serversNames as $uid => $serverName) {
//        $remote = new CodeServer($serverName);
//        $remote->config->request_timeout = 2; //Tiempo de espera de lectura en segundos, especificado por un float (p.ej. 10.5).Por omisión se utiliza el valor del ajuste default_socket_timeout de php.ini.
//        $time = microtime();
//        $max = (int) BbiConector::getMaxTimeExecution(bbiLab_getUserById($uid));
//
//        if(($item = $remote->node_load(5376)) && ((microtime() - $time) < $max)) {
//            if($serverName == 'conectorSegcan') {
//                print_r($item);
//            }
//        }
//    }
    
//    $tipoSegeco = BbiConector::getTipoDeSegeco('conectorSegcan');
//    print_r($tipoSegeco);
    
    
    
    
////    $viales = bbiLab_getVialesDeUnaCaja(5278);// GC-AAA0039
//    $viales = bbiLab_getVialesDeUnaCaja(5279);// GC-AAA0040
//    
//    foreach ($viales as $vial) {
//        $v['titulo'] = $vial;     
//        $v['veterinario'] = 'Zamfer';
//        print_r($v);
//        
//        $remote = new CodeServer('conectorLebrija');
//        $remote->config->request_timeout = 2;
//        //Aquí llamamos a función remota, si devuelve false hay que mandarlo otra vez a la cola
//        $remote->conectorSegeco_setVeterinario($v);  
//    }
    
    
    
//    
//    $item_id = 76052;
//    $item = new stdClass();
//    $item->item_id = $item_id;
//    print_r($item);
//    
//    $queue = DrupalQueue::get('prueba');
//    $queue->releaseItem($item);
//    
    
//    $data = array('casa' , 'ota cosa');
//    $queue = DrupalQueue::get('prueba');
//    $queue->createItem($data);
//    $item = $queue->claimItem();
//    print_r($item);
    
    
//    print_r(actions_do());
   
//    $viales = bbiLab_getVialesDeUnaCaja(1290, 'vacios');
//    print_r($viales);
//    
//    $r = bbiLab_cerrarYRecogerCaja_eliminarVialDeCajaCerrada($viales, 122);
//    print_r($r);
    
    
//    global $item;
//    var_dump($item); 
    
//    print_r(accionesDelCron_cronCheckVialesLlenos());
    
//    BBIQueue::pasarItemAlaColaDeErrores(22222, 'BBIEnviarVialAlAyntamiento');
//    BBIQueue::pasarTodosLosErroresAlaColaDeQueue();
    
    
//    //A ver , vamos a probar las queue, 
//    $dato = array('nombre' => 'Odfgdfg', 'edad' => '23');
//    
//    
//    
//    $queue = new BBIQueue('BBIEnviarVialAlAyntamiento_error', $dato, 33333);  
////    $num = $queue->numberOfItems();
//    print_r($queue);
////    $items = BBIQueue::getItems('accionEnviarVialAlAyntamiento_error');
////    $item = $items[0];
////    
////    BBIQueue::pasarItemFromErrorToQueue($item->item_id);
//    
////    $item = $queue->createItem($dato);
//    
////    $item = $queue->claimItem();
////    $queue->deleteItem($item);
////      print_r($items);
}
