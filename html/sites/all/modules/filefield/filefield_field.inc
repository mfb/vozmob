<?php
// $Id: filefield_field.inc,v 1.18 2009/03/15 06:21:21 quicksketch Exp $

/**
 * @file
 * FileField field hooks and callbacks.
 */

function filefield_field_settings_form($field) {
  drupal_add_js(drupal_get_path('module', 'filefield') .'/filefield.js');

  $form = array();

  $form['list_field'] = array(
    '#type' => 'radios',
    '#title' => t('List field'),
    '#options' => array(0 => t('Disabled'), 1 => t('Enabled')),
    '#default_value' => $field['list_field'] === '' ? 0 : (int) $field['list_field'],
    '#description' => t('The "list" option lets a user choose if a file should shown in a list when viewing the content after creation.'),
    '#attributes' => array('class' => 'filefield-list-field'),
  );
  $form['list_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Files listed by default'),
    '#default_value' => $field['list_default'] === '' ? 1 : (int) $field['list_default'],
  );
  $form['description_field'] = array(
    '#type' => 'radios',
    '#title' => t('Description field'),
    '#default_value' => $field['description_field'] === '' ? 0 : (int) $field['description_field'],
    '#options' => array(0 => t('Disabled'), 1 => t('Enabled')),
    '#description' => t('When enabled, will display a text field where users may enter a description about the uploaded file.'),
  );

  return $form;
}

function filefield_field_settings_validate($field) {
}

function filefield_field_settings_save($field) {
  return array('list_field', 'list_default', 'description_field');
}

function filefield_field_settings_database_columns($field) {
  return array(
    'fid' => array('type' => 'int', 'not null' => FALSE),
    'list' => array('type' => 'int', 'size' => 'tiny', 'not null' => FALSE),
    'data' => array('type' => 'text', 'serialize' => true),
  );
}

function filefield_field_settings_views_data($field) {
  $data = content_views_field_views_data($field);
  $db_info = content_database_info($field);
  $table_alias = content_views_tablename($field);

  // Set our own field handler so that we can hook the file formatter
  // configuration table into the options form.

  // By defining the relationship, we already have a "Has file" filter
  // plus all the filters that Views already provides for files.
  // No need for having a filter by ourselves.
  unset($data[$table_alias][$field['field_name'] .'_fid']['filter']);

  // Add a relationship for related file.
  $data[$table_alias][$field['field_name'] .'_fid']['relationship'] = array(
    'base' => 'files',
    'field' => $db_info['columns']['fid']['column'],
    'handler' => 'content_handler_relationship',
    'label' => t($field['widget']['label']),
    'content_field_name' => $field['field_name'],
  );
  return $data;
}



/**
 * Implementation of CCK's hook_field().
 */
function filefield_field_load($node, $field, &$items, $teaser, $page) {
  if (empty($items)) {
    return array();
  }
  foreach ($items as $delta => $item) {
    // Despite hook_content_is_empty(), CCK still doesn't filter out
    // empty items from $op = 'load', so we need to do that ourselves.
    if (empty($item['fid']) || !($file = field_file_load($item['fid']))) {
      $items[$delta] = NULL;
    }
    else {
      $item['data'] = unserialize($item['data']);
      $items[$delta] = array_merge($item, $file);
    }
  }
  return array($field['field_name'] => $items);
}

function filefield_field_insert($node, $field, &$items, $teaser, $page) {
  return filefield_field_update($node, $field, $items, $teaser, $page);
}

function filefield_field_update($node, $field, &$items, $teaser, $page) {

  // Accumulator to gather current fid to compare with the original node
  // for deleting replaced files.
  $curfids = array();
  foreach ($items as $delta => $item) {
    $items[$delta] = field_file_save($node, $item);
    // Remove items from the array if they have been deleted.
    if (empty($items[$delta]) || empty($items[$delta]['fid'])) {
      $items[$delta] = NULL;
    }
    else {
      $curfids[] = $items[$delta]['fid'];
    }
  }

  // if this is a new node... there are no
  // old items to worry about.
  if ($node->is_new) {
    return;
  } 

  // Delete items from original node if no new revision was created.
  $orig = node_load($node->nid); 
  // If there are, figure out which ones must go.
  if ($node->revision == 0 && !empty($orig->$field['field_name'])) {
    foreach ($orig->$field['field_name'] as $oitem) {
      if (!in_array($oitem['fid'], $curfids)) {
        // For hook_file_references, remember that this is being deleted.
        $oitem['field_name'] = $field['field_name'];
        field_file_delete($oitem); 
      }
    }
  }
}

function filefield_field_delete_revision($node, $field, &$items, $teaser, $page) {
  foreach ($items as $delta => $item) {
    // For hook_file_references, remember that this is being deleted.
    $item['field_name'] = $field['field_name'];
    if (field_file_delete($item)) {
      $items[$delta] = NULL;
    }
  }
}


function filefield_field_delete($node, $field, &$items, $teaser, $page) {
  foreach ($items as $delta => $item) {
    // For hook_file_references, remember that this is being deleted.
    $item['field_name'] = $field['field_name'];
    field_file_delete($item);
  }
}

function filefield_field_sanitize($node, $field, &$items, $teaser, $page) {
  foreach ($items as $delta => $item) {
    // Cleanup $items during node preview.
    if (empty($item['fid']) || !empty($item['delete'])) {
      $items[$delta] = NULL;
      continue;
    }
    // Load the complete file if a filepath is not available.
    if (!empty($item['fid']) && empty($item['filepath'])) {
      $items[$delta] = array_merge($item, field_file_load($item['fid']));
    }
    // Extract data array from serialized string.
    if (is_string($item['data'])){
      $items[$delta]['data'] = unserialize($item['data']);
    }
    // Add nid so formatters can create a link to the node.
    $items[$delta]['nid'] = $node->nid;
  }
}


