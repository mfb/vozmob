<?php

define('TAXONOMY_ACTION_ADD', 0);
define('TAXONOMY_ACTION_REPLACE', 1);
define('TAXONOMY_ACTION_DELETE', 2);
define('TAXONOMY_ACTION_REPLACE_VOCABULARY', 3);

function views_bulk_operations_taxonomy_action_info() {
  if (!module_exists('taxonomy')) return array();
  return array('views_bulk_operations_taxonomy_action' => array(
    'type' => 'node',
    'description' => t('Modify node\'s taxonomy terms'),
    'configurable' => TRUE,
    'behavior' => array('changes_node_property'),
  ));
}

function views_bulk_operations_taxonomy_action(&$node, $context) {
  if (empty($context['terms']) && $context['do'] == TAXONOMY_ACTION_REPLACE) { // Just delete terms
    taxonomy_node_delete($node);
    $node->taxonomy = array();
    return;
  }
  $terms = array();
  foreach ($context['terms'] as $tid) {
    $terms[$tid] = taxonomy_get_term($tid);
  }
  // check for add or delete
  if ($context['do'] == TAXONOMY_ACTION_ADD || $context['do'] == TAXONOMY_ACTION_DELETE) {
    $existing_terms = taxonomy_node_get_terms($node);
    if ($context['do'] == TAXONOMY_ACTION_DELETE) {
      // delete terms
      while (list($delete_tid) = each($terms)) {
        if (array_key_exists($delete_tid,$existing_terms)) {
          unset($existing_terms[$delete_tid]);
        }
      }
      $terms = $existing_terms;
      if (empty($terms)) {
        taxonomy_node_delete($node);
      }
    } else {
      // add terms
      foreach ($terms as $add_tid => $term) {
        if (array_key_exists($add_tid, $existing_terms)) {
          unset($terms[$add_tid]);
        }
      }
      $terms = array_merge($terms, $existing_terms);
    }
  }
  else if ($context['do'] == TAXONOMY_ACTION_REPLACE_VOCABULARY) {
    $existing_terms = taxonomy_node_get_terms($node);
    foreach ($existing_terms as $existing_term) {
      foreach ($terms as $term) {
        if ($term->vid == $existing_term->vid) {
          unset($existing_terms[$existing_term->tid]);
        }
      }
    }       
    $terms = array_merge($terms, $existing_terms);
  }

  $node->taxonomy = $terms;
}

function views_bulk_operations_taxonomy_action_form($context) {
  $vocabularies = array();
  $result = db_query("SELECT DISTINCT v.vid FROM {vocabulary_node_types} v LEFT JOIN {node} n ON v.type = n.type WHERE n.nid IN (%s)", implode(',', $context['selection']));
  while ($v = db_fetch_object($result)) {
    $vocabularies[$v->vid] = taxonomy_vocabulary_load($v->vid);
  }
  if (empty($vocabularies)) {
    drupal_set_message(t('The selected nodes are not associated with any vocabulary. Please select other nodes and try again.'), 'error');
    return array();
  }
  foreach ($vocabularies as $vid => $vocabulary) {
    foreach (taxonomy_get_tree($vid) as $term) {
      $terms[$term->tid] = $vocabulary->name.': '.$term->name;
    }
  }
  $form['terms'] = array(
    '#type' => 'select',
    '#title' => t('Terms'),
    '#options' => $terms,
    '#default_value' => NULL,
    '#multiple' => TRUE,
    '#size' => min(20, count($terms)),
    '#description' => t('Choose the terms you want to associate with the nodes.'),
    '#weight' => -5,
  );
  $form['do'] = array(
    '#type' => 'radios',
    '#title' => t('Action to take'),
    '#default_value' => TAXONOMY_ACTION_ADD,
    '#options' => array(
      TAXONOMY_ACTION_ADD => t('Add the selected terms'),
      TAXONOMY_ACTION_REPLACE => t('Replace existing terms with selected ones'),
      TAXONOMY_ACTION_REPLACE_VOCABULARY => t('Replace terms within same vocabulary'),
      TAXONOMY_ACTION_DELETE => t('Delete selected terms')
    ),
    '#required' => TRUE,
    '#weight' => -2,
  );
  return $form;
}

function views_bulk_operations_taxonomy_action_submit($form, $form_state) {
  return array(
    'do' => $form_state['values']['do'], 
    'terms' => $form_state['values']['terms'],
  );
}

function views_bulk_operations_taxonomy_action_validate($form, $form_state) {
  if ($form_state['values']['do'] != TAXONOMY_ACTION_REPLACE && empty($form_state['values']['terms'])) {
    form_set_error('terms', t('You did not select any term nor did you choose to replace existing terms. Please select one or more terms or choose to replace the terms.'));
  }
}

