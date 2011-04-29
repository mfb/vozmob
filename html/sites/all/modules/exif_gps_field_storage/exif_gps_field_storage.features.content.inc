<?php

/**
 * Implementation of hook_content_default_fields().
 */
function exif_gps_field_storage_content_default_fields() {
  $fields = array();

  // Exported field: field_gps_gpslatitude
  $fields['blog-field_gps_gpslatitude'] = array(
    'field_name' => 'field_gps_gpslatitude',
    'type_name' => 'blog',
    'display_settings' => array(
      'weight' => '29',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'number_decimal',
    'required' => '0',
    'multiple' => '0',
    'module' => 'number',
    'active' => '1',
    'prefix' => '',
    'suffix' => '',
    'min' => '-90',
    'max' => '90',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'precision' => '12',
    'scale' => '10',
    'decimal' => '.',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_gps_gpslatitude][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Latitude',
      'weight' => '29',
      'description' => '',
      'type' => 'number',
      'module' => 'number',
    ),
  );

  // Exported field: field_gps_gpslatituderef
  $fields['blog-field_gps_gpslatituderef'] = array(
    'field_name' => 'field_gps_gpslatituderef',
    'type_name' => 'blog',
    'display_settings' => array(
      'weight' => '31',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_gps_gpslatituderef][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Latitude ref',
      'weight' => '31',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_gps_gpslongitude
  $fields['blog-field_gps_gpslongitude'] = array(
    'field_name' => 'field_gps_gpslongitude',
    'type_name' => 'blog',
    'display_settings' => array(
      'weight' => '28',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'number_decimal',
    'required' => '0',
    'multiple' => '0',
    'module' => 'number',
    'active' => '1',
    'prefix' => '',
    'suffix' => '',
    'min' => '-180',
    'max' => '180',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'precision' => '13',
    'scale' => '10',
    'decimal' => '.',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_gps_gpslongitude][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Longitude',
      'weight' => '28',
      'description' => '',
      'type' => 'number',
      'module' => 'number',
    ),
  );

  // Exported field: field_gps_gpslongituderef
  $fields['blog-field_gps_gpslongituderef'] = array(
    'field_name' => 'field_gps_gpslongituderef',
    'type_name' => 'blog',
    'display_settings' => array(
      'weight' => '30',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_gps_gpslongituderef][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Longitude ref',
      'weight' => '30',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Latitude');
  t('Latitude ref');
  t('Longitude');
  t('Longitude ref');

  return $fields;
}
