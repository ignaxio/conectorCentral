<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Description of BbiVial
 *
 * @author ignacio
 */
class BbiVial {

    public static function update($paquete) {
        if ($vial = node_load(bbiLab_getIdNodeByTitle($paquete->getTituloVial()))) {
            
            $ewrapperVial = entity_metadata_wrapper('node', $vial); 
            
            if ($paquete->getEstado())
                $ewrapperVial->field_vial_estado->set((int)$paquete->getEstado());
            if ($paquete->getAyuntamiento())
                $ewrapperVial->field_ayuntamiento->set((int)bbiLab_getUserByName($paquete->getAyuntamiento()));
            if ($paquete->getVeterinario())
                $ewrapperVial->field_veterinario->set((int)bbiLab_getUserByName($paquete->getVeterinario()));
            if ($paquete->getTipoVial())
                $ewrapperVial->field_vial_tipo_de_vial->set((int)$paquete->getTipoVial());
            if ($paquete->getLocalizacion())
                $ewrapperVial->field_vial_localizacion->set((int)$paquete->getLocalizacion()); 
            if ($paquete->getFechaVialLleno())
                $ewrapperVial->field_vial_fecha_de_extracci_n->set($paquete->getFechaVialLleno());
            if ($paquete->getFechaInforme())
                $ewrapperVial->field_vial_fecha_fin_analisis->set($paquete->getFechaInforme());
            
            
            $ewrapperVial->save();
            return TRUE;
        }
        return FALSE;
    }

    public static function getChapa($idVial) {
        $chapaId = db_query(
                "SELECT ch.nid "
                . "FROM node ch, field_data_field_chapa_vial vial "
                . "WHERE ch.nid = vial.entity_id AND vial.field_chapa_vial_target_id"
                . " = $idVial"
                )->fetchCol();

        if (isset($chapaId[0]))
            return $chapaId[0];
        return FALSE;
    }

    /**
     * Funcion que se le pasa un array de viales especial y te devuelve solo los llenos
     * @param array $viales
     * @return boolean
     */
    public static function getVialesLlenos($viales) {
        $vialesLlenos = array();
        //Si quieren solo los viales llenos
        foreach ($viales as $idVial => $title) {
            //Vamos a buscar los viales que tienen fecha de extracción
            $query = db_select('node', 'n');
            $query->join('field_data_field_vial_fecha_de_extracci_n', 'fvfe', 'n.nid = fvfe.entity_id');
            $query->join('field_data_field_vial_tipo_de_vial', 'tipoVial', 'n.nid = tipoVial.entity_id');
            $query->join('field_data_field_ayuntamiento', 'ayun', 'n.nid = ayun.entity_id');
            $query->leftJoin('field_data_field_veterinario', 'veter', 'n.nid = veter.entity_id');
            $result = $query
                    ->fields('n', array('nid'))
                    ->fields('tipoVial', array('field_vial_tipo_de_vial_value'))
                    ->fields('ayun', array('field_ayuntamiento_target_id'))
                    ->fields('veter', array('field_veterinario_target_id'))
                    ->condition('nid', $idVial, '=')
                    ->execute();
            if ($result) {
                while ($record = $result->fetchAssoc()) {
                    $vialesLlenos[$record['nid']]['titulo'] = $title;
                    $vialesLlenos[$record['nid']]['tipoDeVial'] = $record['field_vial_tipo_de_vial_value'];
                    $vialesLlenos[$record['nid']]['ayuntamiento'] = bbiLab_getUserById($record['field_ayuntamiento_target_id']);
                    $vialesLlenos[$record['nid']]['veterinario'] = bbiLab_getUserById($record['field_veterinario_target_id']);
                }
            } else {
                //no tiene viales llenos
                return FALSE;
            }
        }
        return $vialesLlenos;
    }

    public static function checkHaveCaja($idVial) {
        $vial = node_load($idVial);
        if ($vial->field_vial_pistoleteado['und'][0]['value'] == 1) {
            return true;
        }
        return false;
    }

    public static function createViales($primerNumeroVial, $ultimoNumeroVial) {
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

}
