<?php




function bbiLab_retornarCajasDelLaboratorio($context) {
    //Hay que coger los viales vacios de la caja.
    $viales = bbiLab_getVialesDeUnaCaja($context->nid, 'vacios');
    
    //Ahora hay que modificar los campos de cada vial.
    foreach ($viales as $nid => $titulo) {
        bbiLab_retornarCajasDelLaboratorio_modificarCamposDelVial($nid);        
    }    
    //Ya podemos marcar la caja como desechada.
    bbiLab_retornarCajasDelLaboratorio_modificarCamposDeLaCaja($context->nid);
    //Ahora limpiamos la cache...
    drupal_flush_all_caches();
    //Mandamos el mensaje a Lourdes...
    $respuesta = 'Se han liberado los viales de la caja ' . 
            bbiLab_getTitleById($context->nid) . ', ya puedes meterlos '
            . 'en otra caja nueva.';
    drupal_set_message($respuesta, 'status');
}

function bbiLab_retornarCajasDelLaboratorio_modificarCamposDeLaCaja($cajaId) {
    $caja = node_load($cajaId);
    $caja->field_caja_localizacion['und'][0]['value'] = 4;
    node_save($caja);    
}

/**
 * Vamos a modificar los campos de los vials directamente a la base de datos 
 * sin tener que cargar el vial con node_load.
   Para ello vamos a crear un array de campos para modificar con su valor.
 * @param int $nid
 */
function bbiLab_retornarCajasDelLaboratorio_modificarCamposDelVial($nid) {    
    //Cogemos los campos a modificar
    $campos = bbiLab_retornarCajasDelLaboratorio_camposParaModificar();

    foreach ($campos as $campo => $value) {
        //Si tiene dato es que hay que hacer update, si no lo tiene es que 
        //hay que eliminar.
        if($value !== NULL) {
            db_update('field_data_' . $campo)
                    ->fields(array($campo . '_value' => $value,))
                    ->condition('entity_id', $nid, '=')
                    ->execute();            
        } else {
            db_delete('field_data_' . $campo)
                    ->condition('entity_id', $nid, '=')
                    ->execute();
        }
    }    
}

/**
 * Función que retorna los campos que hay que modificar del vial.
 * Poner NULL como valor si se quiere eliminar el campo.
 * @return array
 */
function bbiLab_retornarCajasDelLaboratorio_camposParaModificar() {
    return array(
        'field_vial_sincronizado'           => 1,
        'field_vial_localizacion'           => 1,
        'field_vial_pistoleteado'           => 0,
        'field_ayuntamiento'                => NULL,
        'field_veterinario'                 => NULL,
        'field_vial_fecha_de_extracci_n'    => NULL,
        'field_vial_fecha_fin_analisis'     => NULL,
        'field_vial_estado'                 => NULL,
    );
}