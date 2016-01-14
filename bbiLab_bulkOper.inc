<?php
require_once 'bulkOper/bbiLab_eliminarVialDeLaCaja.inc';
require_once 'bulkOper/bbiLab_enviarCaja.inc';
require_once 'bulkOper/bbiLab_enviarChapa.inc';
require_once 'bulkOper/bbiLab_cerrarYRecogerCaja.inc';
require_once 'bulkOper/bbiLab_retornarCajasDelSegecoToTienda.inc';
require_once 'bulkOper/bbiLab_retornarCajasUsadasDelLaboratorio.inc';
require_once 'bulkOper/bbiLab_pedirChapasAFabrica.inc';
require_once 'bulkOper/bbiLab_darDeBajaUnVial.inc';



//Función para crear un actión para el módulo burk operations
function bbiLab_action_info() { 
    return array(
        'bbiLab_eliminarVialDeLaCaja' => array(
            'type' => 'entity',
            'label' => t('Eliminar vial de la caja'),
            'configurable' => FALSE,
            'pass rows' => TRUE
        ),
        'bbiLab_enviarCaja' => array(
            'type' => 'entity',
            'label' => t('Enviar caja'),
            'behavior' => array('changes_property'),
            'configurable' => FALSE,
            'vbo_configurable' => TRUE,
            'triggers' => array('any'),
            'pass rows' => TRUE
        ),
        'bbiLab_enviarChapa' => array(
            'type' => 'entity',
            'label' => t('Enviar chapa'),
            'configurable' => FALSE,
            'pass rows' => TRUE
        ),
        'bbiLab_cerrarYRecogerCaja' => array(
            'type' => 'entity',
            'label' => t('Cerrar y recoger'),
            'behavior' => array('changes_property'),
            'configurable' => FALSE,
            'vbo_configurable' => TRUE,
            'triggers' => array('any'),
            'pass rows' => TRUE
        ),
        'bbiLab_retornarCajasDelSegecoToTienda' => array(
            'type' => 'entity',
            'label' => t('Retornar caja entera'),
            'configurable' => FALSE,
            'pass rows' => TRUE
        ),
        'bbiLab_retornarCajasDelLaboratorio' => array(
            'type' => 'entity',
            'label' => t('Retornar cajas del laboratorio'),
            'configurable' => FALSE,
            'pass rows' => TRUE
        ),
        'bbiLab_pedirChapasAFabrica' => array(
            'type' => 'entity',
            'label' => t('Pedir chapas a fábrica'),
            'configurable' => FALSE,
            'pass rows' => TRUE
        ),
        'bbiLab_darDeBajaUnVial' => array(
            'type' => 'entity',
            'label' => t('Dar de baja un vial'),
            'configurable' => FALSE,
            'pass rows' => TRUE
        ),
    );
}