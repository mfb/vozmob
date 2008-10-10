<?php
// $Id: filefield_widget.inc,v 1.39 2008/09/13 00:43:27 drewish Exp $
/**
 * @file
 * FileField: Defines a CCK file field type.
 *
 * Uses content.module to store the fid and field specific metadata,
 * and Drupal's {files} table to store the actual file data.
 *
 * This file contains CCK widget related functionality.
 */

/**
 * @file
 *
 * FileField Widget Settings Hooks.
 * @todo: move description property to filefield_widget widget callbacks
 *        (filefield_widget_widget_{$op}).
 */

function filefield_widget_settings_form($widget) {
  $form = array();
  $form['file_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Permitted upload file extensions'),
    '#default_value' => is_string($widget['file_extensions']) ? $widget['file_extensions'] : 'txt',
    '#size' => 64,
    '#description' => t('Extensions a user can upload to this field. Separate extensions with a space and do not include the leading dot. Leaving this blank will allow users to upload a file with any extension.'),
    '#weight' => 1,
  );
  $form['path_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Path settings'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 6,
  );

  $form['path_settings']['file_path'] = array(
    '#type' => 'textfield',
    '#title' => t('File path'),
    '#default_value' => is_string($widget['file_path']) ? $widget['file_path'] : '',
    '#description' => t('Optional subdirectory within the "%dir" directory where files will be stored. Do not include trailing slash.', array('%dir' => variable_get('file_directory_path', 'files'))),
    '#element_validate' => array('_filefield_widget_settings_file_path_validate'),
    '#suffix' => theme('token_help', 'user'),
  );

  $form['max_filesize'] = array(
    '#type' => 'fieldset',
    '#title' => t('File size restrictions'),
    '#description' => t('Limits for the size of files that a user can upload. Note that these settings only apply to newly uploaded files, whereas existing files are not affected.'),
    '#weight' => 6,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // upload validator. @todo: consider replacing with global
  // && node validate.
  $form['max_filesize']['max_filesize_per_file'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum upload size per file'),
    '#default_value' => is_string($widget['max_filesize_per_file'])
                        ? $widget['max_filesize_per_file']
                         : '',
    '#description' => t('Specify the size limit that applies to each file separately. Enter a value like "512" (bytes), "80K" (kilobytes) or "50M" (megabytes) in order to restrict the allowed file size. If you leave this this empty the file sizes will be limited only by PHP\'s maximum post and file upload sizes.'),
    '#element_validate' => array('_filefield_widget_settings_max_filesize_per_file_validate'),
  );

  // node validate.
  $form['max_filesize']['max_filesize_per_node'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum upload size per node'),
    '#default_value' => is_string($widget['max_filesize_per_node'])
                        ? $widget['max_filesize_per_node']
                        : '',
    '#description' => t('Specify the total size limit for all files in field on a given node. Enter a value like "512" (bytes), "80K" (kilobytes) or "50M" (megabytes) in order to restrict the total size of a node. Leave this empty if there should be no size restriction.'),
    '#element_validate' => array('_filefield_widget_settings_max_filesize_per_node_validate'),
  );
  return $form;
}

function filefield_widget_settings_save($widget) {
  return array(
    'file_extensions', 'file_path', 'max_filesize_per_file',
    'max_filesize_per_node', 'file_widgets'
  );
}

function _filefield_widget_settings_file_path_validate($element, &$form_state) {
  // Strip slashes from the beginning and end of $widget['file_path']
  $form_state['values']['file_path'] = trim($form_state['values']['file_path'], '\\/');
}

function _filefield_widget_settings_max_filesize_per_file_validate($element, &$form_state) {
  if (empty($form_state['values']['max_filesize_per_file'])) {
    return; // Empty means no size restrictions, so don't throw an error.
  }
  else if (!is_numeric(parse_size($form_state['values']['max_filesize_per_file']))) {
    form_error($element, t('The "Maximum file size for each file" option must contain a valid value. You can either leave the text field empty or enter a string like "512" (bytes), "80K" (kilobytes) or "50M" (megabytes).'));
  }
}

function _filefield_widget_settings_max_filesize_per_node_validate($element, &$form_state) {
  if (empty($form_state['values']['max_filesize_per_node'])) {
    return; // Empty means no size restrictions, so don't throw an error.
  }
  else if (!is_numeric(parse_size($form_state['values']['max_filesize_per_node']))) {
    form_error($element, t('The "Maximum file size per node" option must contain a valid value. You can either leave the text field empty or enter a string like "512" (bytes), "80K" (kilobytes) or "50M" (megabytes).'));
  }
}

/**
 * Determine the widget's files directory
 *
 * @param $field CCK field
 * @return files directory path.
 */
function filefield_widget_file_path($field_instance) {
  $dest = $field_instance['widget']['file_path'];
  if (module_exists('token')) {
    global $user;
    $dest = token_replace($dest, 'user', $user);
  }

  return file_directory_path() .'/'. $dest;
}

function filefield_save_upload($element) {
  $upload_name = $element['#field_name'] .'_'. $element['#delta'];
  $field_instance = content_fields($element['#field_name'], $element['#type_name']);

  if (empty($_FILES['files']['name'][$upload_name])) {
    return 0;
  }

  $dest = filefield_widget_file_path($field_instance);
  if (!field_file_check_directory($dest, FILE_CREATE_DIRECTORY)) {
    watchdog('imagefield', 'The upload directory %directory for the file field %field (content type %type) could not be created or is not accessible. A newly uploaded file could not be saved in this directory as a consequence, and the upload was canceled.', array('%directory' => $dest, '%field' => $element['#field_name'], '%type' => $element['#type_name']));
    form_set_error($upload_name, t('The file could not be uploaded.'));
    return 0;
  }

  if (!$file = field_file_save_upload($upload_name, $element['#upload_validators'], $dest)) {
    watchdog('imagefield', 'The file upload failed. %upload', array('%upload' => $upload_name));
    form_set_error($upload_name, t('The Image upload failed.'));
    return 0;
  }
  return $file['fid'];
}

/**
 * FileField widget element callbacks.
 */
function filefield_widget_value($element, $edit = FALSE) {
  if (!$edit) {
    $file =  field_file_load($element['#default_value']['fid']);
    $item = $element['#default_value'];
  }
  else {
    // uploads take priority over value of fid text field.
    if ($fid = filefield_save_upload($element)) {
      $edit['fid'] = $fid;
    }

    // load file.
    $file = field_file_load($edit['fid']);
    $item = array(
      'fid' => $edit['fid'],
      'list' => !empty($edit['list']),
      'data' => empty($edit['data']) ? array() : $edit['data'],
    );
  }
  // merge file and item data so it is available to all widgets.
  $item = array_merge($item, $file);

  // if this widget is another type and leaning on filefield to do the dirty work....
  // pass it back home.
  $function = $element['#type'] .'_widget_value';
  if (function_exists($function)) {
    $item = array_merge($item, $function($element, $edit));
  }
  return $item;
}

function filefield_widget_process($element, $edit, &$form_state, $form) {

  $item = $element['#value'];
  $field_name = $element['#field_name'];
  $delta = $element['#delta'];

  $field = content_fields($element['#field_name'], $element['#type_name']);

  // check remove buttons...
  $remove_name = $element['#field_name'] .'_'. $element['#delta'] .'_remove_btn';
  if (isset($form_state['clicked_button']) && $form_state['clicked_button']['#name'] == $remove_name) {
    $item = _filefield_default_value($field);
  }

  // figute out our fid...
  $element['fid'] = array('#type' => 'hidden', '#value' =>  $item['fid']);

  if ($item['fid'] != 0) {
    $element['preview'] = array('#type' => 'markup', '#value' => theme($element['#type'] .'_preview', $item));
  }

  // placeholder.. will be serialized into the data column. this is a place for widgets
  // to put additional data.
  $element['data'] = array('#tree' => 'true');

  if ($field['show_description']) {
    $element['data']['description'] = array(
      '#type' => 'textfield',
      '#title' => t('Description'),
      '#value' => isset($item['data']['description']) ? $item['data']['description'] : '',
    );
  }

  if ($field['force_list_default']) {
    $element['list'] = array(
      '#type' => 'hidden',
      '#value' => $field['list_default'],
    );
  }
  else {
    $element['list'] = array(
      '#type' => 'checkbox',
      '#title' => t('List'),
      '#value' => isset($item['list']) ? $item['list'] : $field['list_default'],
      '#attributes' => array('class' => 'filefield-list'),
    );
  }



  foreach ($element['#upload_validators'] as $callback => $arguments) {
    $help_func = $callback .'_help';
    $desc[] = call_user_func_array($help_func, $arguments);
  }
  $element['upload'] = array(
    '#name' => 'files['. $element['#field_name'] .'_'. $element['#delta'] .']',
    '#type' => 'file',
    '#title' => t('New Upload'),
    '#description' => implode('<br />', $desc),
    '#attributes' => array(
      'accept' => implode(',', array_filter(explode(' ', $field['widget']['file_extensions']))),
    )
  );


  if ($item['fid'] != 0) {
    $element['upload']['#title'] = t('Replace');
  }

  $element['#prefix'] = '<div id="'. $element['#id'] .'-ahah-wrapper" class="filefield-ahah-wrapper">';
  $element['#suffix'] = '</div>';
  $element['upload_btn'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
    '#process' => array('form_expand_ahah'),
    '#submit' => array('node_form_submit_build_node'),
    '#ahah' => array( // with JavaScript
       'path' => 'filefield/ahah/'.   $element['#type_name'] .'/'. $element['#field_name'] .'/'. $element['#delta'],
       'wrapper' => $element['#id'] .'-ahah-wrapper',
       'method' => 'replace',
       'effect' => 'fade',
    ),
    '#field_name' => $element['#field_name'],
    '#delta' => $element['#delta'],
    '#type_name' => $element['#type_name'],
    '#upload_validators' => $element['#upload_validators'],
  );
  if ($item['fid'] != 0) {
    $element['remove_btn'] = array(
      '#name' => $element['#field_name'] .'_'. $element['#delta'] .'_remove_btn',
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#process' => array('filefield_widget_process_remove_btn', 'form_expand_ahah'),
      '#submit' => array('node_form_submit_build_node'),
      '#ahah' => array( // with JavaScript
        'path' => 'filefield/ahah/'.   $element['#type_name'] .'/'. $element['#field_name'] .'/'. $element['#delta'],
        'wrapper' => $element['#id'] .'-ahah-wrapper',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#field_name' => $element['#field_name'],
      '#delta' => $element['#delta'],
    );
  }


  return $element;
}

function filefield_widget_validate($element, &$form_state) {

}

function _filefield_widget_validate($element, &$form_state) {
}


/**
 * FormAPI theme function. Theme the output of an image field.
 */
function theme_filefield_widget($element) {
  return theme('form_element', $element, $element['#children']);
}

function theme_filefield_widget_preview($item) {
  return '<div class="filefield-preview clear-block">'.
           '<div class="filename">'. theme('filefield_file', $item) .'</div>'.
           '<div class="filesize">'. format_size($item['filesize']) .'</div>'.
           '<div class="filemime">'. $item['filemime'] .'</div>'.
         '</div>';
}

function theme_filefield_widget_item($element) {
  return '<div class="filefield-row clear-block">'.
              '<div class="filefield-filename clear-block">'. drupal_render($element['preview']) . '</div>'.
              '<div class="fielfield-edit clear-block">'.
                '<div class="filefield-list">'. drupal_render($element['list']) . '</div>' .
                '<div class="filefield-description">'. drupal_render($element['description']) . '</div>' .
                '<div class="filefield-stuff">'. drupal_render($element) .'</div>'.
              '</div>'.
         '</div>';
}

/**
 * #require validation for filetype fields.
 */

function filefield_node_form_validate($form, &$form_state) {
  $type = content_types($form['type']['#value']);
  foreach ($type['fields'] as $field_name => $field) {
    if (!(in_array($field['module'], array('imagefield', 'filefield'))) || !$field['required']) continue;
    $empty = $field['module'] .'_content_is_empty';
    $valid = false;
    $total_filesize = 0;
    foreach($form_state['values'][$field_name] as $delta => $item) {
      if ($empty($item, $field)) continue;
      $valid = true;
      $total_filesize += (int)$item['filesize'];
    }

    if (!$valid) {
      form_set_error($field_name, t('%title field is required.', array('%title' => $field['widget']['label'])));
    }
    $max_filesize = parse_size($field['widget']['max_filesize_per_node']);
    if ($max_filesize && $total_filesize > $max_filesize) {
      form_set_error($field_name, t('Total filesize for %title, %tsize,  exceeds field settings of %msize.',
                                    array(
                                      '%title' => $field['widget']['label'],
                                      '%tsize' => format_size($total_filesize),
                                      '%msize' => format_size($max_filesize)
                                    )
                                  ));
    }
  }
}

function filefield_node_form_submit($form, $form_state) {
  // we ignore all but the save button here.
  if ($form_state['values']['op'] != t('Save')) {
    return;
  }
  // @todo:  try to delete removed files.
}
