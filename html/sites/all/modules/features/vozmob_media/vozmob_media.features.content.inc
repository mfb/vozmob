<?php

/**
 * Implementation of hook_content_default_fields().
 */
function vozmob_media_content_default_fields() {
  $fields = array();

  // Exported field: field_image
  $fields['blog-field_image'] = array(
    'field_name' => 'field_image',
    'type_name' => 'blog',
    'display_settings' => array(
      'weight' => '31',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => '160px_linked',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => '320px_imagelink',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => '160px_linked',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => '160px_linked',
        'exclude' => 0,
      ),
      '0' => array(
        'format' => 'above',
        'exclude' => 0,
      ),
      '1' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '1',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'widget' => array(
      'file_extensions' => 'jpg jpeg png gif',
      'file_path' => 'image/[uid]',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'file_widgets' => NULL,
      'max_resolution' => '0',
      'min_resolution' => '0',
      'alt' => '',
      'custom_alt' => 1,
      'title' => '',
      'custom_title' => 1,
      'progress_indicator' => 'bar',
      'title_type' => 'textfield',
      'use_default_image' => 0,
      'rotate' => 1,
      'label' => 'Image',
      'weight' => '-1',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Image');

  return $fields;
}
