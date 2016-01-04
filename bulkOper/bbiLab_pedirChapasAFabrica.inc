<?php

/* 
 * Acción que pide las chapas seleccionadas a fábrica para que se hagan
 */

/**
 * Funcion que coge las chapas que se seleccionan en el bulk operation 
 * y le mete la fecha de petición.
 * @param object $chapa
 */
function bbiLab_pedirChapasAFabrica($chapa) {
    //Vamos a poner la fecha de petición a las chapas selecionadas
    $chapa->field_chapa_fecha_peticion['und'][0] = bbiLab_getFecha();
    node_save($chapa);
}