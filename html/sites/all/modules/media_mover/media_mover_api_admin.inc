<?php
/* $Id: media_mover_api_admin.inc,v 1.11 2008/02/29 16:41:22 bdragon Exp $ */

/**
 * checks a configuration to see if it harvests from another configuration
 * @param $cid is the configuration id
 * @return parent $cid or false
 */
function media_mover_api_config_parent_config($cid) {
  $result = db_result(db_query('SELECT configuration FROM {media_mover_configurations} WHERE verb = "harvest" AND action = "1" AND module = "mm_node" AND cid = %d', $cid));
  if ($result = unserialize($result)) {
    return($result['mm_harvest_cid']);
  }
  return false;
}

/**
 * calls all of the media mover modules which
 * implement the 'admin' param
 */
function media_mover_admin_settings() {
  foreach (module_implements('media_mover') as $m) {
    $form[$m] = module_invoke($m, 'media_mover', 'admin');
  }
  return system_settings_form($form);
}

/**
 * admin settings for the API module
 */
function media_mover_api_admin_form() {
  global $user;

  $form['admin'] = array(
    '#type' => 'fieldset',
    '#title' => t('Media Mover Options'),
    '#collapsible' => true,
  );

  $form['admin']['mma_no_cron_run'] = array(
    '#type' => 'checkbox',
    '#title' => t('Do not run Media Mover on cron'),
    '#default_value' => variable_get('mma_no_cron_run', false),
    '#description' => t("By checking this, Media Mover will not run when cron.php is called. This is helpful when you don't want to have harvest operations running."),
  );

  // @@@ Make this more friendly.
  $form['admin']['mma_default_uid'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Drupal owner of created files'),
    '#default_value' => variable_get('mma_default_uid', 0),
  );

  $form['admin']['mma_file_perm'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set created file permissions'),
    '#default_value' => variable_get('mma_file_perm', true),
  );

  $form['admin']['mma_file_mask'] = array(
    '#type' => 'textfield',
    '#title' => t('umask to use'),
    '#default_value' => variable_get('mma_file_mask', 0644),
    '#description' => t('Use this if you need to change the permssions on your file system for files created by Media Mover.'),
    '#size' => 4,
  );

  $form['admin']['mma_node_edit_file_show'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show files on node edit screen'),
    '#default_value' => variable_get('mma_node_edit_file_show', false),
    '#description' => t('When a user edits a node, they will see a list of any Media Mover files attached to the node.'),
  );

  $form['admin']['mma_node_file_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete Media Mover files'),
    '#default_value' => variable_get('mma_node_file_delete', true),
    '#description' => t('When you delete a node that has Media Mover files associated with it, do you want those files deleted as well?'),
  );

  $form['admin']['time'] = array(
    '#type' => 'fieldset',
    '#title' => t('Job timeout alert'),
    '#collapsible' => true,
    '#collapsed' => variable_get('mma_cron_notify', false),
    '#description' => t('Sometimes a MM job can get stuck and prevent MM from running in the future. Use this to notify an admin that the queue needs review.'),
  );

  $form['admin']['time']['mma_cron_notify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify admin when a MM job has run to long'),
    '#default_value' => variable_get('mma_cron_notify', false),
  );

  $form['admin']['time']['mma_cron_notify_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address to notify'),
    '#default_value' => variable_get('mma_cron_notify_email', $user->mail),
    '#description' => t('Email address to notify that a MM job has taken more than the set time.'),
  );

  $form['admin']['time']['mma_cron_notify_time'] = array(
    '#type' => 'textfield',
    '#title' => t('MM alert time'),
    '#default_value' => variable_get('mma_cron_notify_time', 10),
    '#description' => t('Set the maximum time (in minutes) that a MM job can run before alerting an admin.'),
    '#size' => 4,
  );

  return $form;
}

/**
 * Overview page.
 * Display information about media_mover modules
 * and existing configurations.
 */
function media_mover_api_overview() {
  drupal_add_css(drupal_get_path('module', 'media_mover_api') . '/media_mover_api.css');
  $form = array();
  $form['media_mover_modules'] = array(
    '#type' => 'fieldset',
    '#title' => t('Media Mover modules'),
  );

  $row = array();
  $header = array(t('Loaded modules'), t('Action type(s)'), t('Module actions'));
  foreach (module_implements('media_mover') as $m) {
    $module_actions = array();
    $module_verbs = array();
    // get all mm module actions
    $actions = module_invoke($m, 'media_mover', 'actions');
    foreach (_mm_verbs() as $verb) {
      if (isset($actions[$verb]) && is_array($actions[$verb])) {
        foreach ($actions[$verb] as $action_verb => $action) {
          $module_verbs[] = $verb;
          $module_actions[] =  $action;
        }
      }
    }
    $item = array();
    $item[] = array('header' => TRUE, 'data' => module_invoke($m, 'media_mover', 'name'));
    $item[] = implode(", ", $module_verbs);
    $item[] = implode(", " , array_unique($module_actions));
    $row[] = $item;
  }

  $form['media_mover_modules']['data'] = array(
    '#type' => 'markup',
    '#value' => theme('table', $header, $row),
  );

  $form['media_mover_configs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Media Mover configurations'),
  );

  // build the new header.
  $header = array(array('data' => t('Name'), 'colspan' => 2), t('Description'), t('Files'), t('Last run'), t('Status'), t('Run'), t('Edit'), t('Reset'), t('Delete'), );

  $parents = array();
  $children = array();
  $rows = array();

  // get the configurations
  $results =  pager_query('SELECT * FROM {media_mover_config_list} ORDER BY cid');
  while ($result = db_fetch_array($results)) {

    // handle the status of the current configuration
    switch ($result['status']) {
      case 'enabled':
      case 'stopped':
        $status = t('enabled') .' ('. l(t('disable'), 'admin/content/media_mover/'.  $result['cid'] .'/status/disabled') .')';
      break;
      case 'running':
        $status = t('running') .' ('. l(t('stop'), 'admin/content/media_mover/'. $result['cid'] .'/stop') .')';
      break;
      case 'disabled':
        $status = t('disabled') .' ('. l(t('enable'), 'admin/content/media_mover/'. $result['cid'] .'/status/enabled') .')';
      break;
    }

    // get the total number of files for this config
    $count = db_result(db_query('SELECT COUNT(mmfid) FROM {media_mover_files} WHERE cid = %d', $result['cid']));
    $count = l($count, 'admin/content/media_mover/'. $result['cid'] .'/files');

    // initialize the row
    $row = array();

    // is this configuration harvesting from another MM config?
    if ($parent_id = media_mover_api_config_parent_config($result['cid'])) {
      $title = array('|--> &nbsp; ',  $result['name']);
    }
    else {
      $title = array(array('data' => $result['name'], 'colspan'  => 2));
    }

    // merge the title into the row
    $row = array_merge($title, array(
      $result['description'],
      $count,
      $result['last_start_time'] ?
        date('M j, Y g:ia', $result['last_start_time']) :
        ($result['start_time'] ?
          date('M j, Y g:ia', $result['start_time']) : 'never'),
      $status,
      l(t('run'), 'admin/content/media_mover/'. $result['cid'] .'/run'),
      l(t('edit'), 'admin/content/media_mover/'. $result['cid'] .'/edit'),
      l(t('reset'), 'admin/content/media_mover/'. $result['cid'] .'/reset/full'),
      l(t('delete'), 'admin/content/media_mover/'. $result['cid'] .'/delete'),
    ));

    // if this is a child of another process, put it into the children array
    if ($parent_id) {
      $children[$parent_id][] = $row;
    }
    else {
      $parents[$result['cid']] = $row;
    }
  }

  // now we need to rebuild the rows, and insert the children under the right parents
  foreach($parents as $pid => $parent) {
    $rows[] = array('data' => $parent, 'class' => 'mmapi-parent');
      if (!empty($children[$pid])){
      foreach ($children[$pid] as $chid) {
        $rows[] = $chid;
      }
    }
    // insert an empty array
    $rows[] = array('data' => array(), 'class' => 'mmapi-parent');
  }

  $form['media_mover_configs']['data'] = array(
    '#type' => 'markup',
    '#value' => theme('table', $header, $rows),
  );
  $form['media_mover_configs']['pager'] = array(
    '#type' => 'markup',
    '#value' => theme('pager'),
  );

  return $form;
}

/**
 * Add / Edit configuration form.
 */
function media_mover_config_form(&$form_state, $config = NULL) {
  // Make sure that autocomplete and collapse are loaded so that ahah loaded
  // form pieces work.
  drupal_add_js('misc/autocomplete.js');
  drupal_add_js('misc/collapse.js');

  if ($config) {
    $form_state['storage']['config'] = $config;
  }

  if (isset($config->cid)) {
    $form['cid'] = array(
      '#type' => 'value',
      '#value' => $config->cid,
    );
  }
  $form['new_config']= array(
    '#type' => 'fieldset',
    '#title' => t('Configuration Info'),
  );
  $form['new_config']['mm_config_name']= array(
    '#type' => 'textfield',
    '#title' => t('Name your configuration'),
    '#description' => t('Give a descriptive name to your configuration'),
    '#default_value' => isset($config->name) ? $config->name : '',
  );
  $form['new_config']['mm_config_descript']= array(
    '#type' => 'textarea',
    '#title' => t('Describe configuration'),
    '#description' => t('Enter a description for your configuration.'),
    '#default_value' => isset($config->description) ? $config->description : '',
  );

  // add all mm module config forms
  $form['actions'] = media_mover_api_build_config($form_state);
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  return $form;
}

/**
 * AHAH callback for Add / Edit configuration form.
 */
function media_mover_api_config_js($verb = NULL) {
  list($module, $action) = explode('--', $_POST[$verb]['options']);
  $form_build_id = $_POST['form_build_id'];

  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form = form_get_cache($form_build_id, $form_state);

  $form_state['values'][$verb]['options'] = $_POST[$verb]['options'];
  $config = $form_state['storage']['config'];

  $config->$verb->module = $module;
  $config->$verb->action = $action;
  $config->$verb->configuration = NULL;

  $form = drupal_rebuild_form('media_mover_config_form', $form_state, array($form_state, $config), $form_build_id);

  $output = theme('status_messages') . drupal_render($form['actions'][$verb]['conf']);

  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit;
}

/**
 * Utility function for Add / Edit configuration form.
 * Set up configuration data.
 */
function media_mover_api_build_config(&$form_state) {
  $config = $form_state['storage']['config'];

  $disabled = FALSE;
  // Are we editing an existing config?
  if (isset($config->cid)) {
    // Some of our elements aren't allowed to change when editing.
    $disabled = TRUE;
  }
  // build some descriptive text
  $description['harvest'] = t('Harvesting is where files are selected from');
  $description['process'] = t('Processing is how files are changed');
  $description['storage'] = t('Storage is where files are stored in addition to Media Mover\'s internal storage');
  $description['complete'] = t('Complete is something that happens to files at the end of a configuration');


  // Put together the actions list.
  $actions = array();
  foreach (module_implements('media_mover') as $m) {
    $temp = module_invoke($m, 'media_mover', 'actions');
    $name = module_invoke($m, 'media_mover', 'name');
    foreach ($temp as $k => $v) {
      foreach ($v as $kk => $vv) {
        $actions[$k][$m .'--'. $kk] = t('@media_mover_name: @media_mover_action', array('@media_mover_name' => $name, '@media_mover_action' => $vv));
      }
    }
  }

  $verbs = _mm_verbs();
  foreach ($verbs as $verb) {
    $form[$verb] = array(
      '#type' => 'fieldset',
      '#title' => t('%verb configuration', array('%verb' => $verb)),
      //'#attributes' => array('class' => 'mm_config_fieldset'),
      '#description' => $description[$verb],
//      '#prefix' => '<div class="clear-block" id="mm_config_wrapper_'. $verb.'">',
//      '#suffix' => '</div>',
    );

    $form[$verb]['options'] = array(
      '#title' => t('Select which %verb module and action to use', array('%verb' => $verb)),
      '#type' => 'select',
      '#options' => $actions[$verb],
      '#default_value' => (isset($config->$verb)) ? $config->$verb->module .'--'. $config->$verb->action : 'media_mover--1',
      '#ahah' => array(
        'path' => 'admin/content/media_mover/js/'. $verb,
        'wrapper' => 'mm_config_wrapper_'. $verb,
//        'action' => 'change',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#parents' => array($verb, 'options'),
      '#disabled' => $disabled,
    );
/*
    $form[$verb]['switch_'. $verb] = array(
      '#type' => 'submit',
      '#value' => t('Choose @verb action', array('@verb' => t($verb))),
      //'#description' => t("If the amount of boxes above isn't enough, click here to add more choices."),
      //'#weight' => 1,
      '#submit' => array('mm_add_config_switch_config', $verb), // If no javascript action.
      '#ahah' => array(
        'path' => 'admin/content/media_mover/js',
        'wrapper' => 'mm_config_wrapper_'. $verb,
        'method' => 'replace',
        'effect' => 'fade',
        'progress' => array(
          'type' => 'throbber',
          'message' => t('Loading form'),
        )
      ),
    ); */
    $form[$verb]['conf'] = array(
      '#prefix' => '<div class="clear-block" id="mm_config_wrapper_'. $verb .'">',
      '#suffix' => '</div>',
      '#parents' => array($verb),
    );

    $no_options_form = array(
      'nooptions_'. $verb => array(
        '#type' => 'fieldset',
        '#title' => t('No options'),
        'data' => array(
          '#type' => 'markup',
          '#value' => t('There are no configuration options for this.'),
        ),
      ),
    );
    if (isset($config->$verb)) {
      $c = $config->$verb;
      if (!$action_form = module_invoke($c->module, 'media_mover', 'config', $c->action, $c->configuration)) {
        $action_form = $no_options_form;
      }
    }
    else {
      $action_form = $no_options_form;
    }
    $action_form['#tree'] = FALSE;
    $form[$verb]['conf']['configuration'] = _media_mover_remap_action_form($action_form, $verb);

    $form[$verb]['conf']['configuration']['#tree'] = TRUE;
    $form[$verb]['conf']['configuration']['#parents'] = array($verb);
  }
  return $form;
}

/**
 * Submit handler for the configuration add/edit form.
 */
function media_mover_config_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $verbs = _mm_verbs();

  // Create the configuration.
  $c = new stdClass();
  $c->name = $values['mm_config_name'];
  $c->description = $values['mm_config_descript'];
  $c->status = 'enabled';

  if (isset($values['cid'])) {
    $c->cid = $values['cid'];
    drupal_write_record('media_mover_config_list', $c, array('cid'));
  }
  else {
    drupal_write_record('media_mover_config_list', $c);
  }
  $cid = $c->cid;

  $conf = array();
  foreach ($verbs as $verb) {
    $conf[$verb] = array();
    if (isset($values[$verb]['conf'])) {
      $conf[$verb] = array();
      $conf[$verb]['configuration'] = $values[$verb]['conf'];
    }
    list($conf[$verb]['module'], $conf[$verb]['action']) = explode('--', $values[$verb]['options']);
  }

  // Delete any old configuration rows that may be lying around.
  db_query('DELETE FROM {media_mover_configurations} WHERE cid = %d', $cid);

  // add each configuration's config to the db
  foreach ($conf as $verb => $config) {
    $c = new stdClass();
    $c->cid = $cid;
    $c->verb = $verb;
    $c->module = $config['module'];
    $c->action = $config['action'];
    $c->configuration = isset($config['configuration']) ? $config['configuration'] : array();
    drupal_write_record('media_mover_configurations', $c);

  //db_query("INSERT INTO {media_mover_configurations} (cid, verb, module, action, configuration) VALUES (%d, '%s', '%s', '%s', '%s')", $cid, $verb, $module, $action, $configuration);
  }

  drupal_set_message(t('Media mover configuration %name created.', array('%name' => $values['mm_config_name'])));

  // We're done with our storage.
  unset($form_state['storage']);
  $form_state['redirect'] = 'admin/content/media_mover';
}

/**
 * Stop a running configuration.
 * @param $config A configuration.
 */
function media_mover_api_config_stop_confirm(&$form_state, $config) {
  $form['warning'] = array(
    '#type' => 'markup',
    '#value' => t('You are about to stop:<br /> %config_name <br /> %config_description <br /><br />'.
    'This stops a running configuration. This is useful if the process got hung. '.
    'Becareful that you can break a running process, so don\'t do this without good reason. <br /><br />',
    array('%config_name' => $config->name, '%config_description' => $config->description) ),
  );
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => $config->cid,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to stop this configuration?'),
    'admin/content/media_mover',
    t('This action cannot be undone.'),
    t('Reset'),
    t('Cancel')
  );
}

/**
 * Submit handler for Stop a running configuration.
 */
function media_mover_api_config_stop_confirm_submit($form, &$form_state) {
  db_query("UPDATE {media_mover_config_list} SET status = '%s', last_start_time = 0 WHERE cid = %d", 'stopped', $form_state['values']['cid']);
  drupal_set_message('Stopped the configuration.');
  $form_state['redirect'] = 'admin/content/media_mover';
}

/**
 * Reset a configuration fully.
 * @param $config A MM configuration.
 */
function media_mover_config_reset_full_confirm(&$form_state, $config) {
  $form['warning'] = array(
    '#type' => 'markup',
    '#value' => t('You are about to reset:<br /> %config_name <br /> %config_description <br /><br />'.
    'This removes Media Mover\'s references to files harvested, processed, and stored by this configuration as well as the files (unless you choose to not delete them). '.
    'Doing this may result in duplicated copies of Media Mover files, depending on your configuration. <br /><br />'.
    'Please only do this if you are debugging an installation or have very good reasons for wanting to do so.<br /><br /.',
    array('%config_name' => $config->name, '%config_description' => $config->description) ),
  );
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => $config->cid,
  );

  $form['confirm_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t("Delete this configuration's files"),
    '#description' => t('If this is checked, all files created by this configuration will also be removed'),
    '#default_value' => TRUE,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to reset this configuration and remove references to its files?'),
    'admin/content/media_mover',
    t('This action cannot be undone.'),
    t('Reset'),
    t('Cancel')
  );
}

/**
 * Submit handler for Reset a configuration fully.
 * this only deletes the records of files
 */
function media_mover_config_reset_full_confirm_submit($form, &$form_state) {

  if ($form_state['values']['confirm_delete']) {
    // run the hook to delete files created by any MM module
    media_mover_api_config_delete_files($form_state['values']['cid']);
    drupal_set_message(t('Deleted files associated with this configuration'));
  }

  db_query("UPDATE {media_mover_config_list} SET status = '%s', last_start_time = 0, start_time = 0 WHERE cid = %d", 'stopped', $form_state['values']['cid']);
  db_query('DELETE FROM {media_mover_files} WHERE cid = %d', $form_state['values']['cid']);
  drupal_set_message(t("Reset the configuration. Removed all of Media Mover's file refrences for this configuration."));
  $form_state['redirect'] = 'admin/content/media_mover';
}

/**
 * Delete a configuration.
 * @param $config A MM configuration.
 */
function media_mover_config_delete_confirm(&$form_state, $config) {
  $form['warning'] = array(
    '#type' => 'markup',
    '#value' => t('You are about to delete:<br /> %config_name <br /> %config_description <br />', array('%config_name' => $config->name, '%config_description' => $config->description) )
      . t('This will also delete references to files, and the files that Media Mover has created with this configuration.'),
  );
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => $config->cid,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete this configuration?'),
    'admin/content/media_mover',
    t('This action cannot be undone and will remove ALL files created by this configuration'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit handler for Delete a configuration.
 */
function media_mover_config_delete_confirm_submit($form, &$form_state) {
  // run the hook to delete files created by any MM module
  media_mover_api_config_delete_files($form_state['values']['cid']);
  // remove the configurations
  db_query('DELETE FROM {media_mover_config_list} WHERE cid = %d', $form_state['values']['cid']);
  // remove the definition of the config
  db_query('DELETE FROM {media_mover_configurations} WHERE cid = %d', $form_state['values']['cid']);
  drupal_set_message('Deleted the configuration.');

  $form_state['redirect'] = 'admin/content/media_mover';
}

/**
 * Form for deleting a file.
 * @@@ Make the display_data here more relevant; we actually HAVE file info now...
 */
function media_mover_delete_file_form(&$form_state, $file) {
  $form['display_data'] = array(
    '#type' => 'markup',
    '#value' => t('You are about to delete file id#: %fid', array('%fid' => $file->mmfid)),
  );

  $form['mmfid'] = array(
    '#type' => 'value',
    '#value' => $file->mmfid,
    );

  return confirm_form($form,
      t('Are you sure you want to delete this file?'),
      'admin/content/media_mover/files',
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
  );
}

/**
 * Submit handler for delete file form.
 */
function media_mover_delete_file_form_submit($form, &$form_state) {
  db_query('DELETE FROM {media_mover_files} WHERE mmfid = %d', $form_state['values']['mmfid']);
  drupal_set_message(t('Deleted the file.'));
  $form_state['redirect'] = 'admin/content/media_mover/files';
}

/**
 * Form for changing status.
 */
function media_mover_status_update_confirm(&$form_state, $config, $status) {
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => $config->cid,
  );
  $form['status'] = array(
    '#type' => 'value',
    '#value' => $status,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to change the status of %config?', array('%config' => $config->name)),
    'admin/content/media_mover',
    t('Old state: %oldstate, New state: %newstate.', array('%oldstate' => t($config->status), '%newstate' => t($status))),
    t('Change'),
    t('Cancel')
  );
}

/**
 * Submit handler for status change form.
 */
function media_mover_status_update_confirm_submit($form, &$form_state) {
  db_query("UPDATE {media_mover_config_list} SET status = '%s' WHERE cid = %d", $form_state['values']['status'], $form_state['values']['cid']);
  $form_state['redirect'] = 'admin/content/media_mover';
}
