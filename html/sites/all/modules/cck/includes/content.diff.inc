<?php
// $Id: content.diff.inc,v 1.2.2.1 2008/10/04 13:14:21 karens Exp $

/**
 * @file hook_diff() implementations for CCK (especially fields).
 *
 * These should use a field-hook so the data for the diff is
 * field-type specific.
 */

/**
 * Implementation of hook_diff()
 */
function content_diff(&$old_node, &$new_node) {
  $result = array();
  $cck_info = content_types($new_node->type);
  if ($cck_info) {
    foreach ($cck_info['fields'] as $field) {
      $old_values = array();
      $new_values = array();
      switch ($field['type']) {
        case 'nodereference':
          if (isset($old_node->$field['field_name'])) {
            $old_values = _cck_nodereference_diff_values($old_node, $field);
          }
          if (isset($new_node->$field['field_name'])) {
            $new_values = _cck_nodereference_diff_values($new_node, $field);
          }
          break;
        case 'userreference':
          if (isset($old_node->$field['field_name'])) {
            $old_values = _cck_userreference_diff_values($old_node, $field);
          }
          if (isset($new_node->$field['field_name'])) {
            $new_values = _cck_userreference_diff_values($new_node, $field);
          }
          break;
        case 'number_integer':
        case 'text':
          if (isset($old_node->$field['field_name'])) {
            $old_values = _cck_text_diff_values($old_node, $field);
          }
          if (isset($new_node->$field['field_name'])) {
            $new_values = _cck_text_diff_values($new_node, $field);
          }
          break;
        case 'link':
          if (isset($old_node->$field['field_name'])) {
            $old_values = _cck_link_diff_values($old_node, $field);
          }
          if (isset($new_node->$field['field_name'])) {
            $new_values = _cck_link_diff_values($new_node, $field);
          }
          break;
      }
      $result[$field['field_name']] = array(
        '#name' => $field['widget']['label'],
        '#old' => $old_values,
        '#new' => $new_values,
        '#weight' => $field['widget']['weight'],
        '#format' => array(
          'show_header' => FALSE,
        ),
      );
    }
  }
  return $result;
}

function _cck_text_diff_values(&$node, &$field) {
  $result = array();
  foreach ($node->$field['field_name'] as $item) {
    foreach (explode("\n", $item['value']) as $i) {
      $result[] = $i;
    }
  }
  return $result;
}

function _cck_userreference_diff_values(&$node, &$field) {
  $result = array();
  foreach ($node->$field['field_name'] as $item) {
    if ($item['uid']) {
      $result[] = db_result(db_query('SELECT name FROM {users} WHERE uid=%d', $item['uid']));
    }
  }
  return $result;
}

function _cck_nodereference_diff_values(&$node, &$field) {
  $result = array();
  foreach ($node->$field['field_name'] as $item) {
    if ($item['nid']) {
      $result[] = db_result(db_query('SELECT title FROM {node} WHERE nid=%d', $item['nid']));
    }
  }
  return $result;
}

function _cck_link_diff_values(&$node, &$field) {
  $result = array();
  foreach ($node->$field['field_name'] as $item) {
    if ($item['url']) {
      $result[] = 'url: '. $item['url'];
    }
    if ($item['title']) {
      $result[] = 'title: '. $item['title'];
    }
  }
  return $result;
}