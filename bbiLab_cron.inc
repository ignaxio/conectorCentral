<?php

require_once 'cronAcctions/bbiLab_getVialesLlenosDelSegeco.inc';
require_once 'cronAcctions/bbiLab_getVialesSegcan.inc';
require_once 'cronAcctions/bbiLab_enviarVialesAlAynuntamiento.inc';
require_once 'cronAcctions/bbiLab_enviarChapasAlAyuntamiento.inc';

function bbiLab_cron() {     
    BbiBuffer::check_buffer_entrada();
    BbiBuffer::send_buffer_salida();
    
    
    
    
    
//    //Función que mira en todos los ayuntamientos activos si hay viales llenos en las colas suyas.
//    bbiLab_getVialesLlenosDelSegeco();    
//    //Vamos a ver si hay items en la cola "BBIVialLlenoDe[NombreConector]" para procesarlos.
//    bbiLab_procesarItemsSegeco();
//    
//    //vamos a coger los viales del laboratorio que ya han sido analizados, se ha creado un informe.
//    bbiLab_getVialesSegcan();    
//    //Vamos a ver si hay item en la cola "vialesFromSegcanDe[ayntamiento]" de segcan y los procesamos.
//    bbiLab_procesarItemsSegcan();
//    
//    //vamos a enviar los viales a los segecos
//    bbiLab_setVialToSegeco();
//    //Vamos a enviar las chapas a los segecos
//    bbiLab_setChapaToSegeco();
}
