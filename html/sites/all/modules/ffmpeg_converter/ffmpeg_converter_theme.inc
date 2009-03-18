<?php

// $Id: ffmpeg_converter_theme.inc,v 1.1 2009/01/26 23:40:30 zoo33 Exp $

/**
 * @file
 * Theme functions for FFmpeg Converter.
 */


/**
 * Theme the form elements of file fields that are used by ffmpeg_converter.
 * 
 * @param $element
 *   The form element array.
 */
function theme_ffmpeg_converter_widget($element) {
  
  drupal_add_css(drupal_get_path('module', 'ffmpeg_converter'). '/ffmpeg_converter.css', 'module');
  
  $field_name = $element['#field_name'];
  $field = content_fields($field_name);
  
  $output = '';
  $render_fields = array();
  $file_fields = array(
    'empty' => array(),
    'original' => array(),
  );
  
  // Examine field instances.
  foreach (element_children($element) as $key) {
    
    if (is_int($key)) {
      if (!empty($element[$key]['#default_value']['fid'])) {
        if (empty($element[$key]['#default_value']['data']['ffmpeg_converter_file'])) {
          $file_fields['original'][] = $key;
        }
      }
      else {
        $file_fields['empty'][] = $key;
      }
      // Hide weight field.
      $element[$key]['_weight']['#type'] = 'value';
    }
    else {
      $render_fields[] = $key;
    }
  }
  
  // Determine which field instance to render.
  $file_fields = array_merge($file_fields['original'], $file_fields['empty']);
  $output .= drupal_render($element[array_shift($file_fields)]);
  
  // Render other form fields.
  foreach ($render_fields as $key) {
    $output .= drupal_render($element[$key]);
  }
  
  return $output;
}
