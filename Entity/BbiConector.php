<?php

/**
 * Clase que trabaja con la configuración del conector
 *
 * @author ignacio
 */
class BbiConector {

    /**
     * Función que devuelve todos los ayuntamientos activos
     * @return array()
     */
    public static function getAyuntamientosActivos() {
        $ayuntamientos = array();
        //Cogemos el configurador...
        $configs = variable_get('codeserver_configs');
        foreach ($configs as $conectorName => $variablesDelConector) {
            //Si el conector esta activo o no es segcan lo añadimos al array
            if ($variablesDelConector['aytoActivo'] && $conectorName != 'conectorSegcan') {
                $ayuntamientos[bbiLab_getUserByName($variablesDelConector['aytoName'])] = $variablesDelConector['aytoName'];
            }
        }
        return $ayuntamientos;
    }

    /**
     * Devuelve los nombre de los servidores activos
     * @return array()
     */
    public static function getServerNameActivos() {
        $ayuntamientos = array();
        //Cogemos el configurador...
        $configs = variable_get('codeserver_configs');
        foreach ($configs as $conectorName => $variablesDelConector) {
            //Si el conector esta activo o no es segcan lo añadimos al array
            if ($variablesDelConector['aytoActivo']) {
                $ayuntamientos[bbiLab_getUserByName($variablesDelConector['aytoName'])] = $conectorName;
            }
        }
        return $ayuntamientos;
    }

    /**
     * Devuleve los tipos de segeco que hay dados de alta en un array()
     * @return array()
     */
    public static function getTiposDeSegeco() {
        $tiposDeSegeco = array();
        $config = variable_get('codeserver_configs');
        foreach ($config as $server) {
            $tiposDeSegeco[$server['tipoSegeco']] = $server['tipoSegeco'];
        }
        return $tiposDeSegeco;
    }
    
    /**
     * Devuleve el tipo del conector pasandole en conector name
     * @return array()
     */
    public static function getTipoDeSegeco($conectorName1) {
        
        $config = variable_get('codeserver_configs');
        foreach ($config as $conectorName2 => $server) {
            if($conectorName2 == $conectorName1) {
                return $server['tipoSegeco'];
            } 
        }
        return FALSE;
    }

    /**
     * Devuleve el tiempo máximo de ejecución "en milisegundos" asignado a un conector
     * Se le pasa el nombre del ayuntamiento
     * @param String $ayto
     * @return type
     */
    public static function getMaxTimeExecution($ayto) {
        //Cogemos el configurador...
        $configs = variable_get('codeserver_configs');
        foreach ($configs as $conectorName => $variablesDelConector) {
            //Si el el ayto es el mismo devolvemos el tiempo máximo de ejecución en milisegundos
            if ($variablesDelConector['aytoName'] == $ayto) {
                return $variablesDelConector['max_time_queue'] . 000;
            }
        }
    }

}
