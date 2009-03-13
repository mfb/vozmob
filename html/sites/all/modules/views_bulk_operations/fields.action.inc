<?php
// $Id: fields.action.inc,v 1.1.2.4 2009/03/11 03:08:19 kratib Exp $
/**
 * @file Drupal action to set individual field values. 
 * 
 * Heavily "inspired" by fago's work in CCK on 'Populate a field' action (cck/includes/content.rules.inc).
 */

function views_bulk_operations_fields_action_info() {
  if (!module_exists('content')) return array();
  return array('views_bulk_operations_fields_action' => array(
    'type' => 'node',
    'description' => t('Modify node fields'),
    'configurable' => TRUE,
    'behavior' => array('changes_node_property'),
  ));
}

function views_bulk_operations_fields_action_form($context) {
  module_load_include('inc', 'content', 'includes/content.node_form');
  $form = array();
  $form_state = array();
  $node = array('type' => '');
  $weight = -100;
  if (isset($context['selection'])) {
    $fields = array();
    $result = db_query("SELECT DISTINCT type FROM {node} WHERE nid IN (%s)", implode(',', $context['selection']));
    while ($type = db_result($result)) {
      $type_info = content_types($type);
      $fields += $type_info['fields'];
    }
  }
  else {
    $fields = content_fields();
  }
  $form['field_info'] = array('#type' => 'value', '#value' => array());
  foreach ($fields as $field) {
    $field['required'] = FALSE;
    $node[$field['field_name']] = NULL;

    // The field info and widget.
    $form['#field_info'][$field['field_name']] = $field;
    $form['field_info']['#value'][] = $field['field_name'];
    $form += (array)content_field_form($form, $form_state, $field);
    $form[$field['field_name']] += array('#weight' => $weight++, '#prefix' => '<div class="fields-action-togglable">', '#suffix' => '</div>');

    // Checkbox to enable/disable this field.
    $form[$field['field_name'] . '_check'] = array('#type' => 'checkbox', '#attributes' => array('class' => 'fields-action-toggler'));

    // PHP code to program the value.
    $db_info = content_database_info($field);
    $columns = array_keys($db_info['columns']);
    foreach ($columns as $key => $column) {
      $columns[$key] = t("'@column' => value for @column", array('@column' => $column));
    }
    $sample = t("return array(\n  0 => array(@columns),\n  // You'll usually want to stop here. Provide more values\n  // if you want your 'default value' to be multi-valued:\n  1 => array(@columns),\n  2 => ...\n);", array('@columns' => implode(', ', $columns)));

    $form[$field['field_name'] . '_code'] = array(
      '#type' => 'textarea',
      '#default_value' => '',
      '#rows' => 6,
      '#description' => t('Expected format: <pre>!sample</pre>', array(
        '!sample' => $sample,
      )),
      '#prefix' => '<div class="fields-action-togglable">',
      '#suffix' => '</div>',
    );
  }
  $form['#node'] = (object)$node;
  return $form;
}

function theme_views_bulk_operations_fields_action_form(&$form) {
  drupal_add_js(drupal_get_path('module', 'views_bulk_operations').'/fields.action.js');
  $header = array(theme('table_select_header_cell'), t('Field'), t('Code'));
  $rows = array();
  foreach ($form['field_info']['#value'] as $field_name) {
    $rows[] = array(
      drupal_render($form[$field_name . '_check']),
      drupal_render($form[$field_name]),
      drupal_render($form[$field_name . '_code']),
    );
  }
  $output  = t('<h4>Using the Code column</h4>
                <ul>
                <li>Will override the value specified in the field widget.</li>
                <li>Should not include &lt;?php ?&gt; delimiters.</li>
                <li>If in doubt, refer to <a target="_blank" href="@link_devel">devel.module</a> \'Dev load\' tab on a content page to figure out the expected format.</li>
                </ul>', array('@link_devel' => 'http://www.drupal.org/project/devel'));
  $output .= theme('table', $header, $rows); 
  return $output;
}

function views_bulk_operations_fields_action_validate($form, $form_state) {
  $field_types = _content_field_types();
  $chosen = 0;
  foreach ($form_state['values']['field_info'] as $field_name) {
    $field = content_fields($field_name);
    if ($form_state['values'][$field_name . '_check']) {
      $chosen++;
      $function = $field_types[$field['type']]['module'] .'_field';
      if (function_exists($function)) {
        $form['#node'] = (object)array('type' => '', $field_name => $form_state['values'][$field_name]);
        $items = isset($form['#node']->$field_name) ? $form['#node']->$field_name : array();
        $function('validate', $form['#node'], $field, $items, $form, NULL);
        content_field('validate', $form['#node'], $field, $items, $form, NULL);
      }
    }
  }
  if (!$chosen) {
    form_set_error('', t('You must select at least one field to modify.'));
  }
}

function views_bulk_operations_fields_action_submit($form, $form_state) {
  $values = array();
  foreach ($form_state['values']['field_info'] as $field_name) {
    $values[$field_name] = $form_state['values'][$field_name];
    $values[$field_name . '_check'] = $form_state['values'][$field_name . '_check'];
    $values[$field_name . '_code'] = $form_state['values'][$field_name . '_code'];
  }
  return $values;
}

function views_bulk_operations_fields_action(&$node, $context) {
  $type_info = content_types($node->type);
  if (!empty($type_info['fields'])) foreach ($type_info['fields'] as $field) {
    if (@$context[$field['field_name'] . '_check']) { // there's a value for this field
      if (!empty($context[$field['field_name'] . '_code'])) {
        $value = eval($context[$field['field_name'] . '_code']);
      }
      else {
        $value = $context[$field['field_name']];
      }
      $node->{$field['field_name']} = $value;
    }
  }
}

