<?php

/**
 * Implementation of hook_content_default_fields().
 */
function vozmob_og_content_default_fields() {
  $fields = array();

  // Exported field: field_yeta
  $fields['group-field_yeta'] = array(
    'field_name' => 'field_yeta',
    'type_name' => 'group',
    'display_settings' => array(
      'label' => array(
        'format' => 'above',
        'exclude' => 0,
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
      'weight' => '33',
      'description' => '',
      'type' => 'mail2og_emailfield',
      'module' => 'mail2og',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('E-mail address');

  return $fields;
}
