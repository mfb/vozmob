<?php
// $Id: filemime.admin.inc,v 1.1.2.4 2008/08/15 09:24:32 mfb Exp $

/**
 * Form builder function for admin settings.
 */
function filemime_settings() {
  $form['filemime_file'] = array(
    '#type' => 'textfield',
    '#title' => t('Local mime.types file path'),
    '#default_value' => variable_get('filemime_file', ''),
    '#description' => t('If a mime.types file exists and is readable, it will be parsed to extract MIME extension mappings. Example: <em>/usr/local/etc/apache22/mime.types</em>'),
  );
  $form['filemime_types'] = array(
    '#type' => 'textarea',
    '#title' => t('Additional MIME extension mappings'),
    '#default_value' => variable_get('filemime_types', ''),
    '#description' => t('Additional types provided here will be merged with the mime.types file. Specify the mappings using the mime.types file format. Example:<br /><em>audio/mpeg mp3 m4a<br />audio/ogg ogg</em>'),
  );
  $form['#submit'][] = 'filemime_settings_submit';
  return system_settings_form($form);
}

/**
 * Set the mime_extension_mapping array based on the mime.types file and/or string.
 */
function filemime_settings_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Reset to defaults')) {
    variable_del('mime_extension_mapping');
  }
  else {
    $file = $form_state['values']['filemime_file'];
    $mappings = array_merge(
      is_readable($file) ? file($file) : array(),
      preg_split('/[\n\r]+/', $form_state['values']['filemime_types'], -1, PREG_SPLIT_NO_EMPTY)
    );
    foreach ($mappings as $mapping) {
      $types = preg_split('/[\s]+/', $mapping, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($types as $index => $type) {
        if (substr($type, 0, 1) == '#') {
          break;
        }
        if ($index) {
          $mime_types[$type] = $types[0];
        }
      }
    }    
    if (isset($mime_types)) {
      variable_set('mime_extension_mapping', $mime_types);
      drupal_set_message(t('Detected @count.  Overriding default mapping.', array('@count' => format_plural(count($mime_types), '1 custom mapping', '@count custom mappings'))));
    }
    else {
      variable_del('mime_extension_mapping');
      drupal_set_message(t('No custom mappings detected. Using default mapping.'));
    }
  }
}
