<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function vozmob_og_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function vozmob_og_node_info() {
  $items = array(
    'group' => array(
      'name' => t('Group'),
      'module' => 'features',
      'description' => t('Create a new group.'),
      'has_title' => '1',
      'title_label' => t('Título'),
      'has_body' => '1',
      'body_label' => t('Cuerpo'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
