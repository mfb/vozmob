<?php

// $Id: media_mover_api_menus.inc,v 1.1.4.14 2009/05/28 12:05:37 arthuregg Exp $

/**
 * contains all the menu functions for the module
 * @return array
 */
function media_mover_api_menus() {
  global $user;


  // ------------------------------------------------
  // MAIN TABS for admin/media_mover

  // landing page
  $items['admin/build/media_mover'] = array(
    'title' => 'Media Mover',
    'page callback' => 'media_mover_api_page',
    'access arguments' => array('administer media_mover'),
  );

  $items['admin/build/media_mover/config'] = array(
    'title' => 'List',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
  );

  // settings as a tab
  $items['admin/build/media_mover/settings'] = array(
    'title' => 'Settings',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('media_mover_api_admin_settings'),
    'access arguments' => array('administer media_mover'),
    'type' => MENU_LOCAL_TASK,
  );


  // settings in the normal place
  $items['admin/settings/media_mover'] = array(
    'title' => 'Media Mover admin settings',
    'description' => 'Configure default settings for Media Mover modules.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('media_mover_api_admin_settings'),
    'access arguments' => array('administer media_mover'),
  );

  // add a new configuration
  $items['admin/build/media_mover/add'] = array(
    'title' => 'Add Config',
    'page callback' => 'drupal_get_form',
    'access arguments' => array('administer media_mover'),
    'page arguments' => array('media_mover_api_add_config_form'),
    'type' => MENU_LOCAL_TASK,
  );

  // show all files
  $items['admin/build/media_mover/files'] = array(
    'title' => 'All Files',
    'page callback' => 'media_mover_api_files_list',
    'page arguments' => array(arg(4)),
    'access arguments' => array('administer media_mover'),
    'type' => MENU_LOCAL_TASK,
  );

  // files status
  $items['admin/build/media_mover/files/status'] = array(
    'title' => 'File Status',
    'page callback' => 'media_mover_api_files_status',
    'access arguments' => array('administer media_mover'),
    'type' => MENU_LOCAL_TASK,
  );

  // import a configuration
  $items['admin/build/media_mover/import'] = array(
    'title' => 'Import',
    'page callback' => 'media_mover_api_import_config',
    'access arguments' => array('administer media_mover'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 8,
    'file' => 'media_mover_api_io.inc',
  );

  // export a configuration
  $items['admin/build/media_mover/export'] = array(
    'title' => 'Export',
    'page callback' => 'media_mover_api_export',
    'page arguments' => array(4),
    'access arguments' => array('administer media_mover'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 9,
    'file' => 'media_mover_api_io.inc',
  );

  // Tests to run with media mover
  $items['admin/build/media_mover/tests'] = array(
    'title' => 'Tests',
    'page callback' => 'media_mover_api_tests',
    'access arguments' => array('administer media_mover'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
  );

  $items['admin/build/media_mover/tests/list'] = array(
    'title' => 'Overview',
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => -10,
  );

  // -------------------------------------------------------
  // Individual Configuration functions

  // create a landing page for each of the configurations
  if ($configurations = media_mover_api_get_configurations()) {
    foreach ($configurations as $cid => $configuration) {
      $items['admin/build/media_mover/config/'. $cid] = array(
        'title' => drupal_ucfirst($configuration->name),
        'title callback' => 'check_plain',
        'page callback' => 'media_mover_api_configuration_view',
        'page arguments' => array($cid),
        'access arguments' => array('administer media_mover'),
        'description' => $configuration->description,
      );
    }
  }

  // placeholder for
  $items['admin/build/media_mover/config/%/view'] = array(
    'title' => 'Configuration',
    'title callback' => 'media_mover_api_menu_configuration_title_callback',
    'title arguments' => array(4),
    'position' => 'left',
    'page callback' => 'media_mover_api_configuration_view',
    'access arguments' => array('administer media_mover'),
    'page arguments' => array(4),
    'type' => MENU_LOCAL_TASK,
    'weight' => -9,
  );

  // Edit tab for a configuration
  $items['admin/build/media_mover/config/%/edit'] = array(
    'title' => 'Edit Configuration',
    'page callback' => 'drupal_get_form',
    'access arguments' => array('administer media_mover'),
    'page arguments' => array('media_mover_api_config_edit_form', 4),
    'type' => MENU_LOCAL_TASK,
  );

  // run tab for a configuration
  $items['admin/build/media_mover/config/%/run'] = array(
    'title' => 'Run',
    'page callback' => 'media_mover_api_run_configuration',
    'access arguments' => array('administer media_mover'),
    'page arguments' => array(4),
    'type' => MENU_LOCAL_TASK,
  );

  // file tab for a configuration
  $items['admin/build/media_mover/config/%/files'] = array(
    'title' => 'Files',
    'page callback' => 'media_mover_api_files_list',
    'access arguments' => array('administer media_mover'),
    'page arguments' => array(4),
    'type' => MENU_LOCAL_TASK,
  );

  // change the status of a media mover configuration
  $items['admin/build/media_mover/config/%/status'] = array(
    'title' => 'Confirm Configuration Status Change',
    'page callback' => 'drupal_get_form',
    'access arguments' => array('administer media_mover'),
    'type' => MENU_CALLBACK,
    'page arguments' => array('media_mover_api_config_status_change', 4, 6),
  );

  // clear non finished files for a config
  $items['admin/build/media_mover/config/%/files/clear'] = array(
    'title' => 'Files',
    'page callback' => 'media_mover_api_config_files_clear',
    'access arguments' => array('administer media_mover'),
    'page arguments' => array(arg(4)),
    'type' => MENU_LOCAL_TASK,
  );

  // delete an existing configuration
  $items['admin/build/media_mover/config/%/delete'] = array(
    'title' => 'Confirm Configuration Deletion',
    'page callback' => 'drupal_get_form',
    'access arguments' => array('administer media_mover'),
    'page arguments' => array('media_mover_api_config_delete_confirm', 4),
    'type' => MENU_CALLBACK,
  );

  // Empty an existing configuration of files
  $items['admin/build/media_mover/config/%/empty'] = array(
    'title' => 'Confirm Configuration Deletion',
    'page callback' => 'drupal_get_form',
    'access arguments' => array('administer media_mover'),
    'page arguments' => array('media_mover_api_config_empty_confirm', 4),
  );


  // -------------------------------------------------------
  // File tasks

  // edit a file
  $items['admin/build/media_mover/file/edit'] = array(
    'title' => 'Edit File',
    'page callback' => 'drupal_get_form',
    'access arguments' => array('administer media_mover'),
    'type' => MENU_CALLBACK,
    'page arguments' => array('media_mover_api_edit_file_form', 5),
  );

  // delete a file
  $items['admin/build/media_mover/file/delete'] = array(
    'title' => 'Delete File',
    'page callback' => 'drupal_get_form',
    'access arguments' => array('administer media_mover'),
    'type' => MENU_CALLBACK,
    'page arguments' => array('media_mover_api_file_delete_confirm_form', 5),
  );

  // -------------------------------------------------------
  // File tasks

  // rerun a conversion on an existing node
  $items['media_mover/rerun/%/node/%'] = array(
    'title' => 'Rerun process',
    'page callback' => 'drupal_get_form',
    'access arguments' => array('administer media_mover'),
    'type' => MENU_CALLBACK,
    'page arguments' => array('media_mover_api_rerun_node_form', arg(2), arg(4)),
  );

  return $items;
}