<?php

/**
 * Función que salta cuando  se elimina una entidad...
 * @param type $entity
 * @param type $type
 */
function bbiLab_entity_delete($entity, $type) {    
    //Verificamos que la entidad sea de burk operatión
    if($type == 'field_collection_item' && $entity->field_name == 
            'field_vial_caja') {
        //Modificamos el vial marcandolo como no pistoleteado pasandole 
        //el id de la colección
        bbiLab_eliminarVialDeLaCaja_modificarVial($entity->item_id);
        
        //cargamos la caja actual
        $idCajaActual = $entity->field_caja_fisica['und'][0]['target_id'];
        //La modificamos metiendole un vial menos en el contador de viales
        bbiLab_eliminarVialDeLaCaja_modificarCaja($idCajaActual);        
    }
}

/**
 * Función que resta un vial a la caja
 * @param type $idCajaActual
 */
function  bbiLab_eliminarVialDeLaCaja_modificarCaja($idCajaActual){        
    //Cargamos la caja
    if($caja = node_load($idCajaActual)) {
        //Restamos un vial al contador de viales de la caja
        $caja->field_caja_numero_de_viales['und'][0]['value'] -= 1;
        node_save($caja);
    }
}

/**
 * función que busca el vial que tiene esta colección y le marca como 
 * no pistoleteado
 * @param type $idColeccion
 */
function bbiLab_eliminarVialDeLaCaja_modificarVial($idColeccion) {
    //Primero buscamos el vial y lo marcamos como que no se ha disparado
    $idVial = 0;
    $query = db_select('field_data_field_vial_caja', 'f');
    $result = $query
            ->fields('f', array('entity_id'))
            ->condition('field_vial_caja_revision_id', $idColeccion, '=')
            ->execute();
    while ($record = $result->fetchAssoc()) {
        $idVial = $record['entity_id'];
    }
    //cargamos el vial y le metemos el pistoleteado...
    $vial = node_load($idVial);
    if (isset($vial->uid)) {
        $vial->field_vial_pistoleteado['und'][0]['value'] = 0;
    }
    node_save($vial);
}

