<?php
// $Id: content.panels.inc,v 1.1.2.2 2008/10/04 13:14:21 karens Exp $

/**
 * @file
 * Panels 2 interface for CCK fieldgroups and fields.
 *
 * TODO:
 *
 * - Adjust the fields and groups that are displayed for context, when that
 *   capability is added to Panels.
 *
 */

/**
 * Implementation of hook_panels_content_types()
 */
function content_panels_content_types() {
  $items = array();
  $items['content_field'] = array(
    'title' => t('Content field'),
    'single' => TRUE,
    'content_types' => 'content_panels_field_content_types',
    'add callback' => 'content_panels_edit_field',
    'edit callback' => 'content_panels_edit_field',
    'render callback' => 'content_panels_render_field',
    'title callback' => 'content_panels_title_content',
    );
  return $items;
}

function content_panels_field_content_types() {
  return array(
    'description' => array(
      'title' => t('Content field'),
      'icon' => 'icon_node.png',
      'path' => panels_get_path('content_types/node'),
      'description' => t('A content field from the referenced node.'),
      'required context' => new panels_required_context(t('Node'), 'node'),
      'category' => array(t('Node context'), -9),
    ),
  );
}

function content_panels_edit_field($id, $parents, $conf = array()) {
  $form = array();
  $form['label'] = array(
    '#type' => 'select',
    '#title' => t('Label'),
    '#default_value' => $conf['label'],
    '#options' => array(
      'normal' => t('Block title'),
      'above' => t('Above'),
      'inline' => t('Inline'),
      'hidden' => t('hidden'),
    ),
    '#description' => t('Configure how the label is going to be displayed'),
  );
  $options = array('' => '');
  $fields = content_fields();
  $field_types = _content_field_types();
  foreach ($fields as $field_name => $field) {
    $type_info = $field_types[$field['type']];
    foreach ($type_info['formatters'] as $formatter_name => $formatter) {
      $label  = $type_info['label'] .':'. $field['widget']['label'] ;
      $label .= $field['widget']['label'] != $field_name ? ' ('. $field_name .')' : '';
      $options[$label][$field_name .':'. $formatter_name] = $formatter['label'];
    }
  }
  ksort($options);
  $form['field_formatter'] = array(
    '#type' => 'select',
    '#title' => t('Field / Formatter'),
    '#default_value' => $conf['field_formatter'],
    '#options' => $options,
    '#description' => t('Select a field and formatter.'),
    '#required' => TRUE,
  );
  return $form;
}

/**
 * 'Title' callback for the 'field' content type.
 */
function content_panels_title_content($conf, $context) {
  $data = explode(':', $conf['field_formatter']);
  $field_name = $data[0];
  $formatter = $data[1];
  $fields = content_fields();
  $field = $fields[$field_name];
  $field_types = _content_field_types();
  return t('"@s" field @name', array('@s' => $context->identifier, '@name' => $field_types[$field['type']]['label'] .': '. $field['widget']['label'] .' ('. $field['field_name'])) .')';
}

function content_panels_render_field($conf, $panel_args, $context) {
  if (!empty($context) && empty($context->data)) {
    return;
  }
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  $info = _content_type_info($id);
  $data = explode(':', $conf['field_formatter']);
  $field_name = $data[0];
  $formatter = $data[1];

  $field = $info['fields'][$field_name];
  $field_info = $info['field types'][$field['type']];

  $field['display_settings']['label']['format'] = $conf['label'] == 'normal' ? 'hidden' : $conf['label'];
  $field['display_settings']['full']['format'] = $formatter;

  $block->module = 'content';
  $block->delta = $conf['field_name'];

  if ($conf['label'] == 'normal') {
    $block->title = $field['widget']['label'];
  }
  $node_field = isset($node->$field['field_name']) ? $node->$field['field_name'] : array();
  if (content_handle('field', 'view', $field) == CONTENT_CALLBACK_CUSTOM) {
    $field_types = _content_field_types();
    $module = $field_types[$field['type']]['module'];
    $function = $module .'_field';
    if (function_exists($function)) {
      $value = $function('view', $node, $field, $node_field, 0, 0);
    }
  }
  else {
    foreach ($node_field as $delta => $item) {
      $node_field[$delta]['view'] = content_format($field, $item, $formatter, $node);
    }
    $value = theme('field', $node, $field, $node_field, 0, 0);
  }
  $block->content = $value;

  return $block;
}