<?php
// $Id: jq.add.inc,v 1.2.2.4 2008/10/01 02:02:09 aaron Exp $

function _jq_add($plugin, $extra = array(), $cached = TRUE, $display_errors = FALSE, $log_errors = TRUE) {
  static $invoked_plugins, $errors;
  $jq = jq_plugins($plugin, $cached, $display_errors);
  if (!isset($invoked_plugins[$plugin])) {
    if (isset($plugin) && isset($jq)) {
      if (!variable_get('jq_allow_'. $plugin, TRUE)) {
        $error = t('The %plugin jQuery plugin has been disabled.', array('%plugin' => $plugin));
        if ($log_errors) {
          watchdog('jq', $error, WATCHDOG_NOTICE);
        }
        if ($display_errors) {
          drupal_set_message($error, 'error');
        }
        $invoked_plugins[$plugin] = FALSE;
      }
      else {
        if (is_array($jq['files']['js'])) {
          foreach ($jq['files']['js'] as $file) {
            drupal_add_js($file);
          }
        }
        if (is_array($jq['files']['css'])) {
          foreach ($jq['files']['css'] as $file) {
            drupal_add_css($file);
          }
        }
        $invoked_plugins[$plugin] = TRUE;
      }
    }
    else {
      // log & display an error, but only if we haven't already. don't want to overwhelm with a lot of identical errors per page
      $error = t('The %plugin jQuery plugin is not defined.', array('%plugin' => $plugin));
      if ($log_errors) {
        watchdog('jq', $error, WATCHDOG_ERROR);
      }
      if ($display_errors) {
        drupal_set_message($error, 'error');
      }
      $invoked_plugins[$plugin] = FALSE;
    }
  }
  if ($invoked_plugins[$plugin]) {
    if ($jq['module']) {
      module_load_include('jq.inc', $jq['module']);
      $args = array('add', $plugin);
      $args = array_merge($args, (array)$extra);
      $function = $jq['module'] .'_jq';
      call_user_func_array($function, $args);
    }
  }
  return $invoked_plugins[$plugin];
}
