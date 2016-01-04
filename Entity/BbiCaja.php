<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of BbiCaja
 *
 * @author ifarre
 */
class BbiCaja {

    public static function update($datos) {
        if ($idCaja = BbiCaja::getIdCaja($datos['titulo'])) {
            if ($caja = node_load($idCaja)) {
                //Le metemos la fecha, si no hay ninguan todavía...
                if (!isset($caja->field_caja_fec_primer_vial_lleno['und'][0]['value'])) {
                    $caja->field_caja_fec_primer_vial_lleno['und'][0] = bbiLab_getFecha($datos['fechaVialLleno']);
                }

                //Le sumamos 1 a viales llenos si no tienen ya fecha de extracción, 
                //quiere decir que ya se metió en la tienda antes.
                $vial = node_load(bbiLab_getIdNodeByTitle($datos['titulo']));
                if (!isset($vial->field_vial_fecha_de_extracci_n['und'][0])) {
                    $caja->field_caja_viales_llenos['und'][0]['value'] += 1;
                }
                node_save($caja);
                return TRUE;
            }
        }
        return FALSE;
    }

    /**
     * Funcion que devuelve el id de la ultima caj de un vial.
     * Pasandole el título de un vial
     */
    public static function getIdCaja($vialTitulo) {
        $vial = node_load(bbiLab_getIdNodeByTitle($vialTitulo));
        //cojemos el última coleccion de caja del vial
        $idCollection = end($vial->field_vial_caja['und']);
        //La cargamos 
        $collection = field_collection_item_load($idCollection['value']);
        //Buscamos la caja fisica de esa coleccion
        $idCaja = $collection->field_caja_fisica['und'][0]['target_id'];
        return $idCaja;
    }

    public static function getViales($idCaja, $estado = 'todos') {
        //dpm(get_defined_vars());
        $viales = array();
        $vialesLlenos = array();
        $vialesVacios = array();

        //Todos los viales de la caja
        if (!$viales = $this->getTodosLosViales($idCaja))
            return false;

        // vamos a buscar los viales llenos y los vacios
        if ($estado != 'todos') {
            $vialesLlenos = BbiVial::getVialesLlenos($viales);
            $vialesVacios = $this->quitarVialesLLenosDelArray($viales, $vialesLlenos);
        }

        //Si quieren todos los viales de la caja se los doy
        if ($estado == 'todos')
            return $viales;
        elseif ($estado == 'llenos')
            return $vialesLlenos;
        elseif ($estado == 'vacios')
            return $vialesVacios;
    }

    private function quitarVialesLLenosDelArray($viales, $vialesLlenos) {
        $vialesVacios = $viales;
        //Vamos a quitar los viales que ya están llenos para dejar los vacios.
        foreach ($vialesLlenos as $idVialLleno => $tituloVialLleno) {
            foreach ($vialesVacios as $idVial => $tituloVialVacio) {
                if ($idVial == $idVialLleno) {
                    unset($vialesVacios[$idVial]);
                }
            }
        }
        return $vialesVacios;
    }

    private function getTodosLosViales($idCaja) {
        $viales = array();

        //Cojo todos los viales de la caja
        $query = db_select('node', 'n');
        $query->join('field_data_field_caja_fisica', 'fcf', 'n.nid = fcf.field_caja_fisica_target_id');
        $query->join('field_data_field_vial_caja', 'fvc', 'fcf.entity_id = fvc.field_vial_caja_revision_id');
        $result = $query
                ->fields('fvc', array('entity_id'))//Vial
                ->condition('nid', $idCaja, '=')
                ->execute();
        if ($result) {
            while ($record = $result->fetchAssoc()) {
                $viales[$record['entity_id']] = bbiLab_getTitleById(
                        $record['entity_id']);
            }
        } else {
            //Si no tiene viales 
            return FALSE;
        }
        return $viales;
    }

    public static function setViales($tituloCaja, $primerNumeroVial, $ultimoNumeroVial) {
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

            $posicion++;
        }
    }

}
