<?php
// $Id: jq_bridge.jq.inc,v 1.2.2.2 2008/05/04 19:06:16 aaron Exp $

function jq_bridge_jq($op, $plugin = NULL, $extra = NULL, $already_loaded = NULL) {
  switch ($op) {
    case 'info':
      $info = array();
      if (module_exists('jquery_form') && !module_hook('jquery_form', 'jq')) {
        $info['jquery_form'] = array(
          'name' => t('jQuery Form'),
          'description' => t('The jQuery Form Plugin allows you to easily and unobtrusively upgrade HTML forms to use AJAX. The main methods, ajaxForm and ajaxSubmit, gather information from the form element to determine how to manage the submit process. Both of these methods support numerous options which allows you to have full control over how the data is submitted.'),
          'url' => 'http://www.malsup.com/jquery/form',
          'version' => t('Unknown'),
          'files' => array(
            'js' => array(
              drupal_get_path('module', 'jquery_form') .'/jquery_form.js',
            ),
          ),
        );
      }
      if (module_exists('jcarousel') && !module_hook('jcarousel', 'jq')) {
        $info['jcarousel'] = array(
          'name' => t('jCarousel'),
          'description' => t('jQ provides a bridge for jCarousel. Please update to the latest version of that module for better support.'),
          'url' => 'http://drupal.org/project/jcarousel',
          'version' => t('Unknown'),
        );
      }
      if (module_exists('cluetip') && !module_hook('cluetip', 'jq')) {
        $cluetip_dir = drupal_get_path('module', 'cluetip') .'/cluetip/';
        $info['cluetip'] = array(
          'name' => t('Cluetip'),
          'description' => t('jQ provides a bridge for the Cluetip jQuery plugin, which adds formatted tooltips to elements. Submit an issue there for better support.'),
          'url' => 'http://drupal.org/project/cluetip',
          'version' => t('Unknown'),
          'files' => array(
            'js' => array(
              $cluetip_dir .'jquery.dimensions.js',
              $cluetip_dir .'jquery.cluetip.js',
            ),
            'css' => array(
              $cluetip_dir .'jquery.cluetip.css',
            ),
          ),
        );
      }
      return $info;
      break;
    case 'add':
      switch ($plugin) {
        case 'jcarousel':
          $skin = is_string($extra) ? $extra : JQ_BRIDGE_JCAROUSEL_SKIN_DEFAULT;
          jcarousel_add($skin);
          break;
        case 'cluetip':
          // cluetip requires hoverIntent. Try to add that plugin. If not successful, then the cluetip module provides its own version
          if (!$already_loaded && !jq_add('hoverIntent')) {
            drupal_add_js(drupal_get_path('module', 'cluetip') .'/cluetip/jquery.hoverIntent.js');
          }
          break;
      }
      break;
  }
}


