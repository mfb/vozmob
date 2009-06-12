<?php
// $Id: content_field.inc,v 1.1.2.4 2009/06/02 15:05:27 yched Exp $

/**
 * @file
 *   This file provides a CTools content type for fields.
 */

/**
 * Callback function to supply a list of content types.
 */
function content_content_field_ctools_content_types() {
  return array(
    'title' => t('Content field'),
    'defaults' => array('label' => '', 'formatter' => ''),
  );
}

/**
 * Return all field content types available.
 */
function content_content_field_content_type_content_types() {
  // This will hold all the individual field content types.
  $types = array();

  // Get all fields on the site.
  $fields = content_fields();
  $field_types = _content_field_types();
  $node_types = node_get_types();

  foreach ($fields as $field) {
    // Name the content type a combination of field and node type names.
    $content_type_name = $field['type_name'] . ':' . $field['field_name'];

    // Assemble the information about the content type.
    $info = array();
    $info['title'] = t('@type: (@field_type) @field', array('@type' => $node_types[$field['type_name']]->name, '@field' => $field['field_name'], '@field_type' => $field_types[$field['type']]['label']));

    if (isset($field_types[$field['type']]['content_icon'])) {
      $info['icon'] = $field_types[$field['type']]['content_icon'];
    }

    $info['category'] = 'Content';
    $info['description'] = t('Field on the referenced node.');
    $info['required context'] = new ctools_context_required(t('Node'), 'node', array('type' => array($field['type_name'])));

    $types[$content_type_name] = $info;
  }

  return $types;
}

/**
 * Output function for the 'field' content type.
 */
function content_content_field_content_type_render($subtype, $conf, $panel_args, $context) {
  if (is_array($context)) {
    $context = array_pop($context);
  }
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;

  // Extract the node type and field name from the subtype
  list($node_type, $field_name) = explode(':', $subtype, 2);

  // Get the formatter that was selected in the settings dialog.
  $formatter = $conf['formatter'];

  // Get more information about our field.
  $field = content_fields($field_name);

  // Force panel settings into the field's display settings.
  $field['display_settings']['label']['format'] = $conf['label'] == 'normal' ? 'hidden' : $conf['label'];
  $field['display_settings']['full']['format'] = $formatter;
  $node->build_mode = NODE_BUILD_NORMAL;
  // TODO : allow panel-specific template suggestions for content-field.tpl.php ?

  $output = content_view_field($field, $node);

  $block->module = 'content';
  $block->delta = $field_name;
  if ($conf['label'] == 'normal') {
    $block->title = $field['widget']['label'];
  }
  $block->content = $output;

  return $block;
}

/**
 * Returns a settings form for the custom type.
 */
function content_content_field_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  $form['label'] = array(
    '#type' => 'select',
    '#title' => t('Label'),
    '#default_value' => isset($conf['label']) ? $conf['label'] : '',
    '#options' => array(
      'normal' => t('Block title'),
      'above' => t('Above'),
      'inline' => t('Inline'),
      'hidden' => t('Hidden'),
    ),
    '#description' => t('Configure how the label is going to be displayed.'),
  );

  // Extract the node type and field name from the subtype
  list($node_type, $field_name) = explode(':', $form_state['subtype_name'], 2);

  // Get all the information about our field.
  $field = content_fields($field_name);

  // Get information about all the field types on the site.
  $field_types = _content_field_types();

  // Get the information about the type that our field is.
  $type_info = $field_types[$field['type']];

  // Put the possible formatters for our type into an array.
  $options = array();
  foreach ($type_info['formatters'] as $formatter_name => $formatter) {
    $options[$formatter_name] = $formatter['label'];
  }

  $form['formatter'] = array(
    '#type' => 'select',
    '#title' => t('Formatter'),
    '#default_value' => isset($conf['formatter']) ? $conf['formatter'] : 'default',
    '#options' => $options,
    '#description' => t('Select a formatter.'),
    '#required' => TRUE,
  );
}

function content_content_field_content_type_edit_form_submit(&$form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Admin title for field content type.
 */
function content_content_field_content_type_admin_title($subtype, $conf, $context) {
  return t('"@s" field (@name)', array('@s' => $context->identifier, '@name' => $subtype));
}

