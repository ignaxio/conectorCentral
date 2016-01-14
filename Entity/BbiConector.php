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
    public static function getTipoDeSegeco($conectorName) {

        $config = variable_get('codeserver_configs');
        foreach ($config as $conectorName2 => $server) {
            if ($conectorName2 == $conectorName) {
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
                return $variablesDelConector['max_time_queue'];
            }
        }
    }

    public static function getAyuntamientos() {
        //Vamos a buscar los uid con rol 4 que son los ayuntamientos
        $idAyuntamientos = array();
        $ayuntamientos = array();
        $idRolAyntamiento = 4;
        $query = db_select('users_roles', 'u');
        $result = $query
                ->fields('u', array('uid'))
                ->condition('rid', $idRolAyntamiento, '=')
                ->execute();
        while ($record = $result->fetchAssoc()) {
            $idAyuntamientos[$record['uid']] = '';
        }
        //Ahora ya tenemos los id vamos a buscar los nombres    
        foreach ($idAyuntamientos as $idAyuntamiento => $valor) {
            $query = db_select('users', 'u');
            $result = $query
                    ->fields('u', array('name'))
                    ->condition('uid', $idAyuntamiento, '=')
                    ->execute();
            while ($record = $result->fetchAssoc()) {
                $ayuntamientos[$idAyuntamiento] = $record['name'];
            }
        }
        return $ayuntamientos;
    }

    public static function getVeterinarios() {
        //Vamos a buscar los uid con rol 5 que son los veterinarios
        $idVeterinarios = array();
        $veterinarios = array();
        $idRolVeterinario = 5;
        $query = db_select('users_roles', 'u');
        $result = $query
                ->fields('u', array('uid'))
                ->condition('rid', $idRolVeterinario, '=')
                ->execute();
        while ($record = $result->fetchAssoc()) {
            $idVeterinarios[$record['uid']] = '';
        }
        //Ahora ya tenemos los id vamos a buscar los nombres    
        foreach ($idVeterinarios as $idVeterinario => $valor) {
            $query = db_select('users', 'u');
            $result = $query
                    ->fields('u', array('name'))
                    ->condition('uid', $idVeterinario, '=')
                    ->execute();
            while ($record = $result->fetchAssoc()) {
                $veterinarios[$idVeterinario] = $record['name'];
            }
        }
        return $veterinarios;
    }

}
