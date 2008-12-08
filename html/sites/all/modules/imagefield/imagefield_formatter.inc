<?php

/**
 * @file
 * ImageField formatter hooks and callbacks.
 */


function theme_imagefield_formatter_image_plain($element) {
  // Inside a View this function may be called with null data.  In that case,
  // just return.
  if (empty($element['#item'])) {
    return '';
  }

  $field = content_fields($element['#field_name']);
  $item = $element['#item'];

  if (empty($item['fid']) && $field['use_default_image']) $item = $field['default_image'];
  if (empty($item['filepath']))  $item = array_merge($item, field_file_load($item['fid']));

  $class = 'imagefield imagefield-'. $field['field_name'];
  return  theme('imagefield_image', $item, $item['data']['alt'], $item['data']['title'], array('class' => $class));
}

function theme_imagefield_formatter_image_nodelink($element) {
  // Inside a View this function may be called with null data.  In that case,
  // just return.
  if (empty($element['#item'])) {
    return '';
  }

  $node = $element['#node'];
  $imagetag = theme('imagefield_formatter_image_plain', $element);
  $class = 'imagefield imagefield-nodelink imagefield-'. $element['#field_name'];
  return l($imagetag, 'node/'. $node->nid, array('attributes' => array('class' => $class), 'html' => true));
}

function theme_imagefield_formatter_image_imagelink($element) {
  // Inside a View this function may be called with null data.  In that case,
  // just return.
  if (empty($element['#item'])) {
    return '';
  }

  $item = $element['#item'];
  $imagetag = theme('imagefield_formatter_image_plain', $element);
  $original_image_url = file_create_url($item['filepath']);
  $class = 'imagefield imagefield-imagelink imagefield-'. $element['#field_name'];
  return l($imagetag, $original_image_url, array('attributes' => array('class' => $class), 'html' => true));
}

function theme_imagefield_formatter_path_plain($element) {
  // Inside a View this function may be called with null data.  In that case,
  // just return.
  if (empty($element['#item'])) {
    return '';
  }

  $field = content_fields($element['#field_name']);
  $item = $element['#item'];
  if (empty($item['fid']) && $field['use_default_image'])  $item = $field['default_image'];
  // If there is no image on the database, use default.
  if (empty($item['filepath']))  $item = array_merge($item, field_file_load($item['fid']));

  $attributes['class'] .= ' imagefield-formatter-path';
  return '<span '. drupal_attributes($attributes) .'>'. file_create_path($item['filepath']) .'</span>';
}

function theme_imagefield_formatter_url_plain($element) {
  // Inside a View this function may be called with null data.  In that case,
  // just return.
  if (empty($element['#item'])) {
    return '';
  }

  $field = content_fields($element['#field_name']);
  $item = $element['#item'];
  if (empty($item['fid']) && $field['use_default_image'])  $item = $field['default_image'];
  // If there is no image on the database, use default.
  if (empty($item['filepath']))  $item = array_merge($item, field_file_load($item['fid']));

  $attributes['class'] .= ' imagefield-formatter-url';
  return '<span '. drupal_attributes($attributes) .'>'. file_create_url($item['filepath']) .'</span>';
}



