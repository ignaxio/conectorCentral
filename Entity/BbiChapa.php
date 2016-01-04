<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
class BbiChapa {
    
    public static function insert($datos) {
        if(node_load(bbiLab_getIdNodeByTitle($datos['tituloChapa']))) {//        
            return FALSE;
        }
        
        //Creamos la chapa
        $values = array(
            'type'      => 'chapa',
            'uid'       => 1,
            'status'    => 1,
            'comment'   => 0,
            'promote'   => 0,
        );
        $entity = entity_create('node', $values);
        $ewrapper = entity_metadata_wrapper('node', $entity);  
        $ewrapper->title->set($datos['tituloChapa']);
        $ewrapper->field_chapa_tipo_chapa->set((int)$datos['tipoChapa']);    
        if($datos['digitoControl']) $ewrapper->field_chapa_digito_control->set($datos['digitoControl']);  
        $ewrapper->field_ayuntamiento->set((int)bbiLab_getUserByName($datos['ayuntamiento'])); 
        $ewrapper->field_chapa_vial->set((int)bbiLab_getIdNodeByTitle($datos['titulo']));

        $ewrapper->save();    
        if(node_load($ewrapper->getIdentifier())) return true;
    }
    
   
}
