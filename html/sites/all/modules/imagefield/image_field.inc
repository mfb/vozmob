<?php

/**
 * @file
 * Field Hooks and Callbacks for ImageField.
 */


/**
 * Implementation of hook_field_settings().
 */

function imagefield_image_field_settings_form($field) {
  $form = array();
  $form['default'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default image'),
    '#element_validate' => array('_imagefield_field_settings_default_validate'),
  );
      // Present a thumbnail of the current default image.
  $form['default']['use_default_image'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use default image'),
    '#default_value' =>  $field['use_default_image'],
    '#description' => t('Check here if you want to use a image as default.'),
  );
  if (!empty($field['default_image'])) {
    $form['default']['default_image_thumbnail'] = array(
      '#type' => 'markup',
      '#value' => theme('imagefield_image', $field['default_image'], '', '', array('width' => '150'), false),
    );
  }
  $form['default']['default_image_upload'] = array(
    '#type'  => 'file',
    '#title' => t('Upload image'),
    '#description' => t('Choose a image that will be used as default.'),
  );

  // We set this value on 'validate' so we can get cck to add it
  // as a standard field setting.
  $form['default_image'] = array(
    '#type' => 'value',
    '#value' => $field['default_image'],
  );
  return $form;
}

function imagefield_image_field_settings_save($field) {
  return array('default_image', 'use_default_image');
}

function imagefield_image_field_settings_database_columns($field) {
  return array(
    'title' => array('type' => 'varchar', 'length' => 255, 'not null' => true, 'default' => '', 'sortable' => true),
    'alt' => array('type' => 'varchar', 'length' => 255, 'not null' => true, 'default' => '', 'sortable' => true),
  );
}

function imagefield_image_field_settings_filters($field) {
  return array(
    'not null' => array(
      'operator' => array('=' => t('Has Image')),
      'list' => 'views_handler_operator_yesno',
      'list-type' => 'select',
      'handler' => 'imagefield_views_handler_filter_is_not_null',
    ),
 );
}

/**
 * Element specific validation for imagefield default value.
 *
 * Validated in a separate function from imagefield_field() to get access
 * to the $form_state variable.
 */
function _imagefield_field_settings_default_validate($element, &$form_state) {
  // Verify the destination exists
  $dst = file_directory_path() .'/imagefield_default_images';
  if (!field_file_check_directory($dst, FILE_CREATE_DIRECTORY)) {
    form_set_error('default_image', t("The default image could not be uploaded. The destination(%d) does not exist or is not writable by the webserver.", array('%d' => dirname($dst))));
    return;
  }

  $validators = array(
    'file_validate_is_image' => array(),
  );

  // We save the upload here because we can't know the correct path until the file is saved.
  if (!$file = file_save_upload('default_image_upload', $validators, $dst)) {
    // no upload to save we hope... or file_save_upload reported an error on its own.
    return;
  }

  // set new value.
  $form_state['values']['default_image'] = (array)$file;

  // remove old image & clean up database.
  if (file_delete(file_create_path($field['default_image']['filepath']))) {
    db_query('DELETE FROM {files} WHERE fid=%d', $field['default_image']['fid']);
  }
}

/**
 * Custom filter for imagefield NOT null.
 */
function imagefield_views_handler_filter_is_not_null($op, $filter, $filterinfo, &$query) {
  if ($op == 'handler') {
    $query->ensure_table($filterinfo['table']);
    if ($filter['value']) {
      $qs = '%s.%s > 0';
      $query->add_where($qs, $filterinfo['table'], $filterinfo['field']);
    }
    else {
      $qs = '%s.%s = 0 OR %s.%s IS null';
      $query->add_where($qs, $filterinfo['table'], $filterinfo['field'], $filterinfo['table'], $filterinfo['field']);
    }
  }

}
