<?php
// $Id: fieldgroup.panels.inc,v 1.1.2.4 2008/10/06 17:21:51 karens Exp $

/**
 * Implementation of hook_panels_content_types()
 */
function fieldgroup_panels_content_types() {
  $items = array();
  $items['content_fieldgroup'] = array(
    'title' => t('Content fieldgroup'),
    'content_types' => 'fieldgroup_panels_fieldgroup_content_types',
    'single' => TRUE, // only provides a single content type
    'render callback' => 'fieldgroup_panels_render_fieldgroup',
    'add callback' => 'fieldgroup_panels_edit_fieldgroup',
    'edit callback' => 'fieldgroup_panels_edit_fieldgroup',
    'title callback' => 'fieldgroup_panels_fieldgroup_title',
  );
  return $items;
}

/**
 * 'Render' callback for the 'fieldgroup' panel.
 */
function fieldgroup_panels_render_fieldgroup($conf, $panel_args, $context) {
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();
  $block->module = 'fieldgroup';
  if ($node) {
    // Assemble the fields into groups
    $node = node_build_content($node);
    // Get the "machine name" of the group from the options
    $groupname = $conf['group'];
    // Print out the contents of the given group
    // Note, not using drupal_render($node->content[$groupname]) here to avoid printing the fieldset
    $vars = array();
    if (is_array($node->content[$groupname])) {
      foreach (element_children($node->content[$groupname]) as $key) {
        $vars[$key] = drupal_render($node->content[$groupname][$key]);
      }
    }
    $block->subject = $node->content[$groupname]['#title'];
    $block->content = $vars ? theme('fieldgroup_panel', $vars, $node->nid) : $conf['empty'];
    $block->delta = $node->nid;
  }
  else {
    $block->subject = $conf['group'];
    $block->content = t('Content fieldgroup content goes here.');
    $block->delta = 'unknown';
  }
  return $block;
}

/**
 * Allows users to theme the panels group.
 */
function theme_fieldgroup_panel($vars, $nid) {
  return implode('', $vars);
}

/**
 * Return all fieldgroup panel types available.
 */
function fieldgroup_panels_fieldgroup_content_types() {
  return array(
    'description' => array(
      'title' => t('Content fieldgroup'),
      'icon' => 'icon_node.png',
      'path' => panels_get_path('content_types/node'),
      'description' => t('All fields from a fieldgroup on the referenced node.'),
      'required context' => new panels_required_context(t('Node'), 'node'),
      'category' => array(t('Node context'), -9),
    ),
  );
}

/**
 * 'Edit' callback for the 'fieldgroup' panel.
 */
function fieldgroup_panels_edit_fieldgroup($id, $parents, $conf = array()) {
  // Apply defaults
  if (empty($conf)) {
    $conf = array('title' => '', 'group' => '', 'empty' => '');
  }
  // Retrieve the list of all groups on all content types
  $group_list = array();
  $types = fieldgroup_groups(NULL, FALSE, FALSE);
  // Add each group to the list with the content type it is from in parentheses
  foreach ($types as $type) {
    foreach ($type as $group) {
      $group_list[$group['group_name']] = t('@group_label (@group_type_name)', array('@group_label' => $group['label'], '@group_type_name' => $group['type_name']));
    }
  }
  $form['type_name'] = array(
    '#type' => 'value',
    '#value' => $group['type_name'],
  );
  $form['group'] = array(
    '#type' => 'select',
    '#title' => t('Fieldgroup'),
    '#options' => $group_list,
    '#default_value' => $conf['group'],
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );
  $form['empty'] = array(
    '#type' => 'textarea',
    '#title' => 'Empty text',
    '#description' => t('Text to display if group has no data. Note that title will not display unless overridden.'),
    '#rows' => 5,
    '#default_value' => $conf['empty'],
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );
  return $form;
}

/**
 * 'Title' callback for the 'fieldgroup' panel.
 */
function fieldgroup_panels_fieldgroup_title($conf, $context) {
  $types = fieldgroup_groups(NULL, FALSE, FALSE);
  $type = $types[$conf['type_name']][$conf['group']];
  return t('"@s" fieldgroup @name', array('@s' => $context->identifier, '@name' => $type['label']));
}