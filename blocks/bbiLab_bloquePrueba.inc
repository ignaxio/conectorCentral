<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function bbiLab_block_configure($delta='') {
  $form = array();
  
  switch($delta) {
    case 'estadisticas' :
      // Text field form element
      $form['text_body'] = array(
        '#type' => 'text_format',
        '#title' => t('Enter your text here in WYSIWYG format'),
        '#default_value' => variable_get('text_variable', ''),
      );

      // File selection form element
      $form['file'] = array(
        '#name' => 'block_image',
        '#type' => 'managed_file',
        '#title' => t('Choose an Image File'),
        '#description' => t('Select an Image for the custom block.  Only *.gif, *.png, *.jpg, and *.jpeg images allowed.'),
        '#default_value' => variable_get('block_image_fid', ''),
        '#upload_location' => 'public://block_image/',
        '#upload_validators' => array(
          'file_validate_extensions' => array('gif png jpg jpeg'),
        ),
      );
      break;
  }
  return $form;
}


/**
 * Implements hook_block_save().
 */
function bbiLab_block_save($delta = '', $edit = array()) {
  switch($delta) {
    case 'estadisticas' :
      // Saving the WYSIWYG text      
      variable_set('text_variable', $edit['text_body']['value']);

      // Saving the file, setting it to a permanent state, setting a FID variable
      $file = file_load($edit['file']);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      $block = block_load('bbiLab', $delta);
      file_usage_add($file, 'bbiLab', 'block', $block->bid);
      variable_set('block_image_fid', $file->fid);
      break;
  }
}

function bbiLab_block_view($delta='') {
  $block = array();
  
  switch($delta) {
    case 'estadisticas' :
      $block['content'] = bbiLab_view();
      break;
  }
  
  return $block;
}

function bbiLab_view() {
  $block = array();

  // Capture the image file path and form into HTML with attributes
  $image_file = file_load(variable_get('block_image_fid', ''));
  $image_path = '';

  if (isset($image_file->uri)) {
    $image_path = $image_file->uri;
  }

  $image = theme_image(array(
    'path' => ($image_path),
    'alt' => t('Image description here.'),
    'title' => t('This is our block image.'),
    'attributes' => array('class' => 'class_name'),
  ));

  // Capture WYSIWYG text from the variable
  $text = variable_get('text_variable', '');

  // Block output in HTML with div wrapper
  $block = array(
    'image' => array(
      '#prefix' => '',
      '#type' => 'markup',
      '#markup' => $image,
    ),
    'message' => array(
      '#type' => 'markup',
      '#markup' => $text,
      '#suffix' => '',
    ),
  );

  return $block;
}