<?php
// $Id: node_from_noderef.inc,v 1.1.2.1 2009/06/02 12:24:03 yched Exp $

/**
 * @file
 * Implements the node reference relationship for Panels.
 */

/**
 * Implementation of hook_ctools_relationships().
 */
function nodereference_node_from_noderef_ctools_relationships() {
  return array(
    'title' => t('Node from reference'),
    'keyword' => 'nodereference',
    'description' => t('Adds a node from a node reference in a node context; if multiple nodes are referenced, this will get the first referenced node only.'),
    'required context' => new ctools_context_required(t('Node'), 'node'),
    'context' => 'nodereference_node_from_noderef_context',
    'settings form' => 'nodereference_node_from_noderef_settings_form',
    'settings form validate' => 'nodereference_node_from_noderef_settings_form_validate',
  );
}

/**
 * Return a new ctools context based on an existing context
 */
function nodereference_node_from_noderef_context($context = NULL, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL
  if (empty($context->data)) {
    return ctools_context_create_empty('node', NULL);
  }
  if (isset($context->data->{$conf['field_name']}[0]['nid']) && ($nid = $context->data->{$conf['field_name']}[0]['nid'])) {
    if ($node = node_load($nid)) {
      return ctools_context_create('node', $node);
    }
  }
}

/**
 * Settings form for the ctools relationship.
 */
function nodereference_node_from_noderef_settings_form($conf) {
  $options = array();
  foreach (content_fields() as $field) {
    if ($field['type'] == 'nodereference') {
      $options[$field['field_name']] = t($field['widget']['label']);
    }
  }
  $form['field_name'] = array(
    '#title' => t('Node reference field'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => isset($conf['field_name']) ? $conf['field_name'] : '',
    '#prefix' => '<div class="clear-block">',
    '#suffix' => '</div>',
  );

  return $form;
}


