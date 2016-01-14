<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
class BbiChapa {
    
    public static function insert($paquete) {        
        //Creamos la chapa
        $values = array(
            'type'      => 'chapa',
            'uid'       => 1,
            'status'    => 1,
            'comment'   => 0,
            'promote'   => 0,
        );
        $entity = entity_create('node', $values);
        $ewrapperChapa = entity_metadata_wrapper('node', $entity);  
        $ewrapperChapa->title->set($paquete->getTituloChapa());
        $ewrapperChapa->field_chapa_tipo_chapa->set((int)$paquete->getTipoChapa());    
        if($paquete->getDigitoControl()) $ewrapperChapa->field_chapa_digito_control->set($paquete->getDigitoControl());  
        $ewrapperChapa->field_ayuntamiento->set((int)bbiLab_getUserByName($paquete->getAyuntamiento())); 
        $ewrapperChapa->field_chapa_vial->set((int)bbiLab_getIdNodeByTitle($paquete->getTituloVial()));

        $ewrapperChapa->save();    
        if(node_load($ewrapperChapa->getIdentifier())) return true;
    }
    
   public static function update($paquete, $idChapa) {
        $chapa = node_load($idChapa);
        $ewrapperChapa = entity_metadata_wrapper('node', $chapa);  
         
        $ewrapperChapa->title->set($paquete->getTituloChapa());
        $ewrapperChapa->field_chapa_tipo_chapa->set((int)$paquete->getTipoChapa());    
        if($paquete->getDigitoControl()) $ewrapperChapa->field_chapa_digito_control->set($paquete->getDigitoControl());  
        if($paquete->getMatching()) {
            $ewrapperChapa->field_chapa_finalizado_ok->set(TRUE);
        } else if ($paquete->getResultado() == 3) {
            $ewrapperChapa->field_chapa_finalizado_ok->set(FALSE);
        }
        $ewrapperChapa->save();    
        return TRUE;
   }
}
