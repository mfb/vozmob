<?php 

// $Id: ffmpeg_converter.admin.inc,v 1.3 2009/03/04 19:47:19 zoo33 Exp $

/**
 * @file
 * Include file for FFmpeg Converter's settings pages.
 */

/**
 * Page callback: presets settings form.
 */
function ffmpeg_converter_admin_presets() {
  $form = array();
  $form['ffmpeg_converter_presets'] = array(
    '#value' => t('Use this page to create FFmpeg configuration presets which you can use to process media files automatically.'),
    '#tree' => true,
  );
  if (module_exists('filefield')) {
    $form['ffmpeg_converter_presets']['#value'] .= ' ' . t('Your presets will appear on file field configurations pages (see the <a href="@content_types">Content types</a> page), which allows you to have all media files that are uploaded through a certain file field processed automatically.', array('@content_types', url('admin/content/types')));
  }
  else {
    $form['ffmpeg_converter_presets']['#value'] .= ' ' . t('If you install and enable the FileField module, your presets will appear on file field configurations pages, which allows you to have all media files that are uploaded through a certain file field processed automatically.');
  }
  $form['ffmpeg_converter_presets']['#value'] .= ' ' . t('Third party modules may make this functionality available in their own specific contexts.');
  
  $presets = ffmpeg_converter_options();
  
  // Add a separate fieldset for each configuration.
  foreach ($presets as $key => $preset) {
    $form['ffmpeg_converter_presets'][$key] = array(
      '#type' => 'fieldset',
      '#collapsible' => false,
      '#tree' => true
    );
    $form['ffmpeg_converter_presets'][$key]['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Preset name'),
      '#description' => t('Use this field to rename the preset.'),
      '#default_value' => $preset['name'],
      '#size' => 20,
    );
    
    // Add FFmpeg Wrapper's configuration form.
    $form['ffmpeg_converter_presets'][$key] += ffmpeg_wrapper_configuration_form($preset['ffmpeg_wrapper'], "edit-ffmpeg-converter-presets-$key-ffmpeg-wrapper-");
    $form['ffmpeg_converter_presets'][$key]['ffmpeg_wrapper']['#collapsible'] = true;
    $form['ffmpeg_converter_presets'][$key]['ffmpeg_wrapper']['#collapsed'] = true;
    $form['ffmpeg_converter_presets'][$key]['ffmpeg_wrapper']['#tree'] = true;
    
    $form['ffmpeg_converter_presets'][$key]['process_same_format'] = array(
      '#type' => 'checkbox',
      '#title' => t('Process files already in destination format'),
      '#description' => t('Choose this if you want to process files even if they are already in the chosen destination format.'),
      '#default_value' => $preset['process_same_format'],
    );
    $form['ffmpeg_converter_presets'][$key]['delete_preset'] = array(
      '#type' => 'submit',
      '#value' => t('Delete this preset') . ' (TODO)',
      '#submit' => '',
    );
    if (count($presets) < 2) {
      $form['ffmpeg_converter_presets'][$key]['delete_preset']['#disabled'] = true;
    }
    // TODO: delete this line once this button is implemented.
    $form['ffmpeg_converter_presets'][$key]['delete_preset']['#disabled'] = true;
  }
  
  $form['add_preset'] = array(
    '#type' => 'submit',
    '#value' => t('Add new preset') . ' (TODO)',
    '#submit' => '',
  );
  // TODO: delete this line once this button is implemented.
  $form['add_preset']['#disabled'] = true;
  
  return system_settings_form($form);
}


/**
 * Page callback: debug settings form.
 */
function ffmpeg_converter_admin_debug() {
  $form = array();
  
  $form['ffmpeg_converter_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Save debug messages to the log'),
    '#description' => t('If you enable this, the options used when invoking ffmpeg will be logged each time a file is converted.'),
    '#default_value' => variable_get('ffmpeg_converter_debug', 0),
  );
  
  return system_settings_form($form);
}
