<?php

/**
 * Implementation of hook_content_default_fields().
 */
function vozmob_og_content_default_fields() {
  $fields = array();

  // Exported field: field_group_logo
  $fields['group-field_group_logo'] = array(
    'field_name' => 'field_group_logo',
    'type_name' => 'group',
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'group_logo_small_linked',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'group_logo_large_default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '5' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '0',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'widget' => array(
      'file_extensions' => 'png gif jpg jpeg',
      'file_path' => '',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'max_resolution' => '0',
      'min_resolution' => '0',
      'alt' => '',
      'custom_alt' => 0,
      'title' => '',
      'custom_title' => 0,
      'title_type' => 'textfield',
      'default_image' => NULL,
      'use_default_image' => 0,
      'rotate' => 0,
      'label' => 'Group logo',
      'weight' => '-4',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_yeta
  $fields['group-field_yeta'] = array(
    'field_name' => 'field_yeta',
    'type_name' => 'group',
    'display_settings' => array(
      'weight' => '14',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '5' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'mail2og_emailfield',
    'required' => '0',
    'multiple' => '0',
    'module' => 'mail2og',
    'active' => '1',
    'mail2og_emailfield_process' => '',
    'widget' => array(
      'size' => NULL,
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_yeta][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'E-mail address',
      'weight' => '14',
      'description' => '',
      'type' => 'mail2og_emailfield',
      'module' => 'mail2og',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('E-mail address');
  t('Group logo');

  return $fields;
}
