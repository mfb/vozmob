<?php
// $Id: jq.admin.inc,v 1.2.2.4 2008/10/01 02:02:09 aaron Exp $

/**
 *  The administration form. Allows an administrator to turn off specific plugins
 */
function jq_settings_form() {
  $plugins = jq_plugins(NULL, FALSE, TRUE);
  $form = array();
  $form['jq'] = array(
    '#type' => 'fieldset',
    '#title' => t('jQuery Plugins'),
    '#description' => t('These jQuery plugins are currently registered through the jQ module.'),
    '#collapsible' => TRUE,
  );
  foreach ($plugins as $key => $plugin) {
    $description = $plugin['description'];
    if ($plugin['version'] || $plugin['url']) {
      $description .= '<br />';
    }
    if ($plugin['version']) {
      $description .= t('<br />%Version: @version', array('%Version' => t('Version'), '@version' => $plugin['version']));
    }
    if ($plugin['url']) {
      $description .= t('<br />%Homepage: !url', array('%Homepage' => t('Homepage'), '!url' => l($plugin['name'], $plugin['url'])));
    }
    $description .= t('<br />%Invocation: ', array('%Invocation' => t('Invocation')));
    $description .= $plugin['invocation'] ? $plugin['invocation'] : t('Invoke this plugin with %code', array('%code' => "jq_add('$key');"));
    $form['jq']['jq_allow_'. $key] = array(
      '#type' => 'fieldset',
      '#title' => $plugin['name'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['jq']['jq_allow_'. $key]['allow'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable @plugin (%code)', array('@plugin' => $plugin['name'], '%code' => $key)),
      '#description' => $description,
      '#default_value' => variable_get('jq_allow_'. $key, TRUE),
    );
    $subform = module_invoke($plugin['module'], 'jq', 'settings');
    if (is_array($subform)) {
      $form['jq']['jq_allow_'. $key]['settings'] = array(
        '#type' => 'fieldset',
        '#title' => t('Extra settings'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      $form['jq']['jq_allow_'. $key]['settings']['subform'] = $subform;
    }
  }
  if (empty($plugins)) {
    drupal_set_message(t('There are currently no defined jQuery plugins registered through the jQ module. You only need to enable this module if directed to by another.'), 'error');
  }
  return system_settings_form($form);
}

/**
 *  Build the jquery plugin array to be cached.
 *  Any module that defines hook_jquery_plugins may define new jquery plugins for the system.
 *
 *  For a plugin to be registered with this module, a module needs to invoke hook_jq.
 *  hook_jq($op = 'info|add', $plugin = NULL)
 *  If $op is 'add', then it is called with the plugin when invoked on a page.
 *  If $op is 'info', then it needs to return an associative array of defined plugins:
 *    'name' => // ... Name of the plugin.
 *    'description' => // ... Description of the plugin.
 *    'files' => array(
 *      'js' => array(
 *        // ... An array of js files to be loaded on the page.
 *      ),
 *      'css' => array(
 *        // ... An array of css files to be loaded on the page.
 *      ),
 *    ),
 */
function _jq_plugins($display_errors = FALSE, $log_errors = TRUE) {
  $plugins = array();

  // Scan the '/plugins' and '/sites/example.com/plugins' directories for .js and .css files.
  // If there are multiple plugins with the same filename in the directory, the LAST file read will be used.
  $files = drupal_system_listing('(\.js$|\.css$)', 'plugins', 'basename', 0);
  foreach ($files as $file) {
    $plugins[$file->name]['name'] = $file->name;
    $plugins[$file->name]['plugin'] = $file->name;
    if (substr($file->filename, strripos($file->filename, '.') + 1) == 'js') {
      $plugins[$file->name]['files']['js'][] = $file->filename;
    }
    else {
      $plugins[$file->name]['files']['css'][] = $file->filename;
    }
  }

  // any module may put its hook_jq functions in module_name.jq.inc, so that it may be included automatically
  module_load_all_includes('jq.inc');
  foreach (module_implements('jq') as $module) {
    $mod_jq = module_invoke($module, 'jq', 'info');
    if (is_array($mod_jq)) {
      foreach ($mod_jq as $key => $plugin) {
        if (isset($plugins[$key])) {
          $error = 'There is a conflict with the %plugin jQuery plugin. It is defined by both the %module1 and %module2 modules.';
          $error_args = array('%plugin' => $key, '%module1' => $plugins[$key]['module'], '%module2' => $module);
          if ($log_errors) {
            watchdog('jq', $error, $error_args, WATCHDOG_WARNING);
          }
          if ($display_errors) {
            drupal_set_message(t($error, $error_args), 'error');
          }
        }
        else {
          $plugins[$key] = $plugin;
          $plugins[$key]['plugin'] = $key;
          $plugins[$key]['module'] = $module;
        }
      }
    }
  }
  ksort($plugins);
  return $plugins;
}
