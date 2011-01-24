<?php
// $Id: taxonomy.inc,v 1.1.2.11 2009/12/10 17:04:52 jazzslider Exp $

/**
 * Implementation of hook_exportables().
 * @return array
 */
function taxonomy_exportables() {
  $exportables = array();

  $exportables['taxonomy_vocabulary'] = array(
    'hook name'         => 'taxonomy_default_vocabularies',
    'load function'     => 'taxonomy_vocabulary_load',
    'save function'     => 'taxonomy_save_vocabulary',
    'delete function'   => 'taxonomy_del_vocabulary',
    'load all function' => 'taxonomy_get_vocabularies',
    'id property'       => 'vid',
    'title property'    => 'name',
    'storage type'      => 'array',
  );

  return $exportables;
}

/*
function taxonomy_taxonomy_default_vocabularies() {
  $vocabularies = array();
  $vocabularies[] = array(
     'name' => 'Tags',
     'machine' => 'tags', // <--
     'relations' => TRUE,
     'hierarchy' => FALSE,
     'multiple' => FALSE,
     'required' => TRUE,
     'tags' => TRUE,
     'module' => 'taxonomy',
     'weight' => 9,
     'nodes' => array(
      'blog_entry' => 'blog_entry',
    ),
  );
  return $vocabularies;
}
*/

/**
 * Implementation of hook_features_api().
 */
function taxonomy_features_api() {
  return array(
    'taxonomy_vocabulary' => array(
      'name' => t('Vocabularies'),
      'default_hook' => 'taxonomy_default_vocabularies',
      'default_file' => FEATURES_DEFAULTS_INCLUDED,
      'features_source' => TRUE,
      'file' => drupal_get_path('module', 'exportables') .'/modules/taxonomy.inc',
    ),
  );
}

/**
 * Implementation of hook_features_export().
 */
function taxonomy_vocabulary_features_export($data, &$export, $module_name = '') {
  // first do the standard exportables export
  $pipe = _exportables_features_export('taxonomy_vocabulary', $data, $export, $module_name);

  if (!in_array('taxonomy', $export['dependencies'])) {
    $export['dependencies'][] = 'taxonomy';
  }

  // then loop through the data again so we can add taxonomy-specific
  // dependency components to the pipe
  foreach ($data as $type) {
    $vocabulary = taxonomy_vocabulary_load(exportables_machine_load_id('taxonomy_vocabulary', $type));
    if ($vocabulary) {
      // if this vocabulary is used by a specific node type, add a pipe for it
      foreach ($vocabulary->nodes as $node_type => $node_type_id) {
        $pipe['node'][] = $node_type;
      }
    }
  }

  return $pipe;
}

/**
 * Implementation of hook_features_export_options().
 */
function taxonomy_vocabulary_features_export_options() {
  return _exportables_features_export_options('taxonomy_vocabulary');
}

/**
 * Implementation of hook_features_export_render().
 */
function taxonomy_vocabulary_features_export_render($module, $data) {
  return _exportables_features_export_render('taxonomy_vocabulary', $module, $data);
}
