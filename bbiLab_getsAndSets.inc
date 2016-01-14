<?php

function bbiLab_sendEmail($titulo, $mensaje) {
    simple_mail_queue('ifarre@bbi.bio', 'ifarrebbi@gmail.com', 'ADN Perros ' .
            $titulo, $mensaje);
}

/**
 * Función que busca el nid de la chapa que tiene asociado el vial que recive 
 * por parametro
 * @param int $idVial
 * @return int
 */
//function bbiLab_getChapaFromVial($idVial) {
//    $chapaId = db_query(
//            "SELECT ch.nid "
//            . "FROM node ch, field_data_field_chapa_vial vial "
//            . "WHERE ch.nid = vial.entity_id AND vial.field_chapa_vial_target_id"
//            . " = $idVial"
//            )->fetchCol();
//
//    if ($chapaId[0])
//        return $chapaId[0];
//
//    // metemos el error en el watch log de drupal si no hay chapa
//    watchdog_exception('SQL', 'No existe la chapa del vial ' . $idVial . '. ' . $e);
//    return FALSE;
//}

/**
 * funcion que devuelve los viales que hay en una caja 
 * El estado puede ser.. si se omite te devuelve todos los viales de la caja
 * $estado = 'vacios' te devuelve los viales vacios de la caja
 * $estado = 'llenos' te devuelve los viales llenos de la caja
 * @param int $idCaja
 * @param string $estado
 * @return array
 */
//function bbiLab_getVialesDeUnaCaja($idCaja, $estado = 'todos') {
////dpm(get_defined_vars());
//    $viales = array();
//    $vialesLlenos = array();
//    $vialesVacios = array();
//
////Todos los viales de la caja
//    if (!$viales = bbiLab_getTodosLosVialesDeUnaCaja($idCaja))
//        return false;
//
//// vamos a buscar los viales llenos y los vacios
//    if ($estado != 'todos') {
//        $vialesLlenos = bbiLab_getVialesLlenos($viales);
//        $vialesVacios = bbiLab_quitarVialesLLenosDelArray($viales, $vialesLlenos);
//    }
//
////Si quieren todos los viales de la caja se los doy
//    if ($estado == 'todos')
//        return $viales;
//    elseif ($estado == 'llenos')
//        return $vialesLlenos;
//    elseif ($estado == 'vacios')
//        return $vialesVacios;
//}
//
//function bbiLab_quitarVialesLLenosDelArray($viales, $vialesLlenos) {
//    $vialesVacios = $viales;
////Vamos a quitar los viales que ya están llenos para dejar los vacios.
//    foreach ($vialesLlenos as $idVialLleno => $tituloVialLleno) {
//        foreach ($vialesVacios as $idVial => $tituloVialVacio) {
//            if ($idVial == $idVialLleno) {
//                unset($vialesVacios[$idVial]);
//            }
//        }
//    }
//    return $vialesVacios;
//}
//
//function bbiLab_getVialesLlenos($viales) {
//    $vialesLlenos = array();
////Si quieren solo los viales llenos
//    foreach ($viales as $idVial => $title) {
////Vamos a buscar los viales que tienen fecha de extracción
//        $query = db_select('node', 'n');
//        $query->join('field_data_field_vial_fecha_de_extracci_n', 'fvfe', 'n.nid = fvfe.entity_id');
//        $query->join('field_data_field_vial_tipo_de_vial', 'tipoVial', 'n.nid = tipoVial.entity_id');
//        $query->join('field_data_field_ayuntamiento', 'ayun', 'n.nid = ayun.entity_id');
//        $query->leftJoin('field_data_field_veterinario', 'veter', 'n.nid = veter.entity_id');
//        $result = $query
//                ->fields('n', array('nid'))
//                ->fields('tipoVial', array('field_vial_tipo_de_vial_value'))
//                ->fields('ayun', array('field_ayuntamiento_target_id'))
//                ->fields('veter', array('field_veterinario_target_id'))
//                ->condition('nid', $idVial, '=')
//                ->execute();
//        if ($result) {
//            while ($record = $result->fetchAssoc()) {
//                $vialesLlenos[$record['nid']]['titulo'] = $title;
//                $vialesLlenos[$record['nid']]['tipoDeVial'] = $record['field_vial_tipo_de_vial_value'];
//                $vialesLlenos[$record['nid']]['ayuntamiento'] = bbiLab_getUserById($record['field_ayuntamiento_target_id']);
//                $vialesLlenos[$record['nid']]['veterinario'] = bbiLab_getUserById($record['field_veterinario_target_id']);
//            }
//        } else {
////no tiene viales llenos
//            return FALSE;
//        }
//    }
//    return $vialesLlenos;
//}
//
//function bbiLab_getTodosLosVialesDeUnaCaja($idCaja) {
//    $viales = array();
//
////Cojo todos los viales de la caja
//    $query = db_select('node', 'n');
//    $query->join('field_data_field_caja_fisica', 'fcf', 'n.nid = fcf.field_caja_fisica_target_id');
//    $query->join('field_data_field_vial_caja', 'fvc', 'fcf.entity_id = fvc.field_vial_caja_revision_id');
//    $result = $query
//            ->fields('fvc', array('entity_id'))//Vial
//            ->condition('nid', $idCaja, '=')
//            ->execute();
//    if ($result) {
//        while ($record = $result->fetchAssoc()) {
//            $viales[$record['entity_id']] = bbiLab_getTitleById(
//                    $record['entity_id']);
//        }
//    } else {
////Si no tiene viales 
//        return FALSE;
//    }
//    return $viales;
//}

/**
 * Función que devuelve todos los veterinarios de la tienda en un 
 * array [uid] = nombre
 */
//function bbiLab_getVeterinarios() {
////Vamos a buscar los uid con rol 5 que son los veterinarios
//    $idVeterinarios = array();
//    $veterinarios = array();
//    $idRolVeterinario = 5;
//    $query = db_select('users_roles', 'u');
//    $result = $query
//            ->fields('u', array('uid'))
//            ->condition('rid', $idRolVeterinario, '=')
//            ->execute();
//    while ($record = $result->fetchAssoc()) {
//        $idVeterinarios[$record['uid']] = '';
//    }
////Ahora ya tenemos los id vamos a buscar los nombres    
//    foreach ($idVeterinarios as $idVeterinario => $valor) {
//        $query = db_select('users', 'u');
//        $result = $query
//                ->fields('u', array('name'))
//                ->condition('uid', $idVeterinario, '=')
//                ->execute();
//        while ($record = $result->fetchAssoc()) {
//            $veterinarios[$idVeterinario] = $record['name'];
//        }
//    }
//    return $veterinarios;
////}
//
///**
// * Función que devuelve todos los ayuntamientos de la tienda en un 
// * array [uid] = nombre
// */
//function bbiLab_getAyuntamientos() {
////Vamos a buscar los uid con rol 4 que son los ayuntamientos
//    $idAyuntamientos = array();
//    $ayuntamientos = array();
//    $idRolAyntamiento = 4;
//    $query = db_select('users_roles', 'u');
//    $result = $query
//            ->fields('u', array('uid'))
//            ->condition('rid', $idRolAyntamiento, '=')
//            ->execute();
//    while ($record = $result->fetchAssoc()) {
//        $idAyuntamientos[$record['uid']] = '';
//    }
////Ahora ya tenemos los id vamos a buscar los nombres    
//    foreach ($idAyuntamientos as $idAyuntamiento => $valor) {
//        $query = db_select('users', 'u');
//        $result = $query
//                ->fields('u', array('name'))
//                ->condition('uid', $idAyuntamiento, '=')
//                ->execute();
//        while ($record = $result->fetchAssoc()) {
//            $ayuntamientos[$idAyuntamiento] = $record['name'];
//        }
//    }
//    return $ayuntamientos;
//}

/**
 * Función que mira si el vial está en alguna caja
 */
//function bbiLab_vialTieneCaja($idVial) {
//    $vial = node_load($idVial);
//    if ($vial->field_vial_pistoleteado['und'][0]['value'] == 1) {
//        return true;
//    }
//    return false;
//}

/**
 * función que devuelve el id del usuario pasandole el nombre
 */
function bbiLab_getUserByName($name) {
    $query = db_select('users', 'u');
    $result = $query
            ->fields('u', array('uid'))
            ->condition('name', $name, '=')
            ->execute();
    while ($record = $result->fetchAssoc()) {
        return $record['uid'];
    }
}

/**
 *  * función que devuelve el nombre del usuario pasandole el id
 */
function bbiLab_getUserById($id) {
    try {
        $query = db_select('users', 'u');
        $result = $query
                ->fields('u', array('name'))
                ->condition('uid', $id, '=')
                ->execute();
        while ($record = $result->fetchAssoc()) {
            return $record['name'];
        }
        return 0;
    } catch (Exception $e) {
// metemos el error en el watch log de drupal.
        watchdog_exception('SQL', 'Error en la función bbiLab_getUserById(' . $id . ') ' . $e);
        return 0;
    }
}

/**
 * función que devuelve un nodo pasandole el título
 */
function bbiLab_getIdNodeByTitle($title) {
    $query = db_select('node', 'n');
    $result = $query
            ->fields('n', array('nid'))
            ->condition('title', $title, '=')
            ->execute();
    while ($record = $result->fetchAssoc()) {
        return $record['nid'];
    }
    return 0;
}

/**
 * función que devuelve un nodo pasandole el título,
 */
function bbiLab_getTitleById($id) {
    $query = db_select('node', 'n');
    $result = $query
            ->fields('n', array('title'))
            ->condition('nid', $id, '=')
            ->execute();
    while ($record = $result->fetchAssoc()) {
        return $record['title'];
    }
    return 0;
}

/**
 * Función que crea un array para meter una fecha estilo
 * @return array[] fecha
 */
function bbiLab_getFecha($strFecha = FALSE) {
    $fecha = array();
//Si viene fecha le metemos la que viene si no, le metemos la de ahora.
    $date = $strFecha ? date_create($strFecha) : date_now('UTC');
    $fecha['value'] = date_format($date, "Y-m-d H:i:s");
    $fecha['timezone'] = "Europe/Madrid";
    $fecha['timezone_db'] = "Europe/Madrid";
    $fecha['date_type'] = "datetime";
    return $fecha;
}

/**
 * función que cambia valores de los viales de la tienda. 
 * Coje el vial y modifica algun dato
 */
function bbiLab_setdatosMasivo() {
//Voy a marcar todos los viales que empiecen por HC como tipo de vial heces
    $viales = array();

    $query = db_select('node', 'n');
    $query->join('field_data_field_vial_estado', 'estado', 'n.nid = estado.entity_id');
//$query->join('field_data_field_vial_tipo_de_vial', 'tipo', 
//'n.nid = tipo.entity_id');
    $result = $query
            ->fields('n', array('nid', 'title'))
            ->fields('estado', array('field_vial_estado_value'))
            ->condition('type', 'vial', '=')
//->condition('title', '%' . db_like('HC') . '%', 'LIKE')
//->condition('tipo.field_vial_tipo_de_vial_value', 'Eces', '=')
            ->execute();
    while ($record = $result->fetchAssoc()) {
        $viales[] = $record;
    }


    foreach ($viales as $key => $vial) {


        $estado = $vial['field_vial_estado_value'];
//Vamos a cambiarle los estados
        switch ($estado) {
            case 11:
                $n = node_load($vial['nid']);
                $n->field_vial_estado['und'][0]['value'] = 2;
                node_save($n);
                echo "vial $n->title modificado<br>";
                break;
            case 12:
                $n = node_load($vial['nid']);
                $n->field_vial_estado['und'][0]['value'] = 3;
                node_save($n);
                echo "vial $n->title modificado<br>";
                break;
            case 13:
                $n = node_load($vial['nid']);
                $n->field_vial_estado['und'][0]['value'] = 4;
                node_save($n);
                echo "vial $n->title modificado<br>";
                break;
            case 14:
                $n = node_load($vial['nid']);
                $n->field_vial_estado['und'][0]['value'] = 9;
                node_save($n);
                echo "vial $n->title modificado<br>";
                break;
        }
    }
}

/**
 * Función hecha para meter los viales de lebrija que ya se habían enviado, 
 * la dejo para futuras consultas de manejo de coleccion de campos
 * Función que mete 50 viales en una caja
 * Funcion que borra todas las colecciones de campos lo marca como pistoleteado 
 * y le crea una nueva coleccion de campo
 */
function bbiLab_meterVialesEnCaja($tituloCaja, $primerNumeroVial, $ultimoNumeroVial) {

    $posicion = 1;
    $idCaja = bbiLab_getIdNodeByTitle($tituloCaja);
    for ($i = $primerNumeroVial; $i <= $ultimoNumeroVial; $i++) {

        $titulo = 'CA-AAA';
        $titulo = $titulo . $i;

        $vial = node_load(bbiLab_getIdNodeByTitle($titulo));

//Eliminamos todas las colecciones de campo que tenia de antes       
        foreach ($vial->field_vial_caja['und'] as $idCollection) {
//Cargamos el nodo....
            $collection = field_collection_item_load($idCollection['value']);
//Checkeamos que sea correcto
            if ($collection->item_id) {
                entity_delete_multiple('field_collection_item', array($idCollection));
            }
        }
//Le metemos que está pistoleteado
        if ($vial->type == 'vial') {
            $vial->field_vial_pistoleteado['und'][0]['value'] = 1;
//borramos todas las collecciones vacias
            $vial->field_vial_caja['und'] = null;
            $vial->body = '';
            node_save($vial);
        }

//Aquí creamos la colección de campos y la guardamos
        $values['field_name'] = 'field_vial_caja';
        $values["field_vial_caja_posicion"] = array(LANGUAGE_NONE =>
            array(array('value' => $posicion)));
        $values["field_caja_fisica"] = array(LANGUAGE_NONE =>
            array(array('target_id' => $idCaja)));
//print_r($values);
        $field_collection_item = entity_create('field_collection_item', $values);
        $field_collection_item->setHostEntity('node', $vial);
        $field_collection_item->save();
//Ahora la marcamos como pistoleteado

        $posicion++;
    }
}

function bbiLab_creadorDeViales($primerNumeroVial, $ultimoNumeroVial) {
    for ($i = $primerNumeroVial; $i <= $ultimoNumeroVial; $i++) {
        $titulo = 'CA-AAA';
        $titulo = $titulo . $i;
        $values = array(
            'type' => 'vial',
            'uid' => 1,
            'status' => 1,
            'comment' => 0,
            'promote' => 0,
        );
        $entity = entity_create('node', $values);
        $ewrapper = entity_metadata_wrapper('node', $entity);
        $ewrapper->title->set($titulo);
        $ewrapper->save();
//print_r($entity);
        return true;
    }
}

function fincionesBBI_setChapa($titulo, $fecha_envio, $fecha_peticion, $ayuntamiento) {
    $ayuntamiento = bbiLab_getUserByName($ayuntamiento);
    $existe = false;
//Primero comrobamos que no exista en la base de datos. 
    $query = db_select('node', 'n');
    $result = $query
            ->fields('n', array('nid'))
            ->condition('title', $titulo, '=')
            ->execute();
    if ($result) {
        while ($record = $result->fetchAssoc()) {
            $existe = true;
        }
    }
    if (!$existe) {
        $values = array(
            'type' => 'chapa',
            'uid' => 1,
            'status' => 1,
            'comment' => 0,
            'promote' => 0,
        );
        $entity = entity_create('node', $values);
        $ewrapper = entity_metadata_wrapper('node', $entity);
        $ewrapper->title->set($titulo);
        $ewrapper->field_chapa_fecha_envio->set($fecha_envio);
        $ewrapper->field_chapa_fecha_peticion->set($fecha_peticion);
//$ewrapper->field_chapa_finalizado_ok->set(true); 
        $ewrapper->field_ayuntamiento->set($ayuntamiento);
        $ewrapper->save();
        if (node_load($ewrapper->getIdentifier()))
            return true;
    }
}

function fincionesBBI_getChapas($ayuntamiento) {
    $remote = new CodeServer('conectorXativa');
    $remote->config->request_timeout = 10;
//Aquí llamamos a función remota, si devuleve true se marca 
//el vial sincronizado.
    $chapas = $remote->conectorSegeco_getAllChapas();


    foreach ($chapas as $chapa) {
        $existe = false;
        $query = db_select('node', 'n');
        $result = $query
                ->fields('n', array('nid'))
                ->condition('title', $chapa['titulo'], '=')
                ->execute();
        if ($result) {
            while ($record = $result->fetchAssoc()) {
                $existe = true;
            }
        }
        if (!$existe) {

            $fecha_envio = $chapa['fecha_envio'] ? strtotime(date(
                                    str_replace("T", " ", $chapa['fecha_envio']))) : NULL;
            $fecha_peticion = $chapa['fecha_peticion'] ? strtotime(date(
                                    str_replace("T", " ", $chapa['fecha_peticion']))) : NULL;
            $values = array(
                'type' => 'chapa',
                'uid' => 1,
                'status' => 1,
                'comment' => 0,
                'promote' => 0,
            );
            $entity = entity_create('node', $values);
            $ewrapper = entity_metadata_wrapper('node', $entity);
            $ewrapper->title->set($chapa['titulo']);
            if ($fecha_envio)
                $ewrapper->field_chapa_fecha_envio
                        ->set($fecha_envio);
            if ($fecha_peticion)
                $ewrapper->field_chapa_fecha_peticion
                        ->set($fecha_peticion);
            $ewrapper->field_chapa_finalizado_ok->set(true);
            $ewrapper->field_ayuntamiento->set(140);
            $ewrapper->save();
            echo 'Chapa insertada ' . $chapa['titulo'] . '<br>';
        }
    }
}

function bbiLab_CreadorDeVIalesChulo() {
    $titulos = array('CA-AAA3353', 'CA-AAA3445', 'CA-AAA3272', 'CA-AAA3444',
        'CA-AAA3251', 'CA-AAA2736', 'CA-AAA3276', 'CA-AAA3249', 'CA-AAA3270',
        'CA-AAA3380', 'CA-AAA2740', 'CA-AAA3269', 'CA-AAA3376', 'CA-AAA3267',
        'CA-AAA2742', 'CA-AAA2966');
    $posicion = 1;
    $caja3 = 4399;
    $idCaja = $caja1;
    foreach ($titulos as $titulo) {

        $values = array(
            'type' => 'vial',
            'uid' => 1,
            'status' => 1,
            'comment' => 0,
            'promote' => 0,
        );
        $entity = entity_create('node', $values);
        $ewrapper = entity_metadata_wrapper('node', $entity);
        $ewrapper->title->set($titulo);
        $ewrapper->field_vial_tipo_de_vial->set(2); //1 sangre, 2 hisopo
        $ewrapper->save();

        $vial = node_load($ewrapper->getIdentifier());
        $vial->field_vial_pistoleteado['und'][0]['value'] = 1;

//Aquí creamos la colección de campos y la guardamos
        $values['field_name'] = 'field_vial_caja';
        $values["field_vial_caja_posicion"] = array(LANGUAGE_NONE => array(
                array('value' => $posicion)));
        $values["field_caja_fisica"] = array(LANGUAGE_NONE => array(
                array('target_id' => $caja3)));
//print_r($values);
        $field_collection_item = entity_create('field_collection_item', $values);
        $field_collection_item->setHostEntity('node', $vial);
        $field_collection_item->save();
        $posicion++;
    }
}
