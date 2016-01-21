<?php
require_once 'blocks/bbiLab_bloquePrueba.inc';

function bbiLab_block_info() {
  $blocks = array();
  $blocks['estadisticas'] = array(
    'info' => t('Bloque de estadísticas'),
  );
  
  return $blocks;
}