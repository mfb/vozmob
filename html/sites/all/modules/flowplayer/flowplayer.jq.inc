<?php
// $Id: flowplayer.jq.inc,v 1.1.2.4 2009/09/08 15:03:10 robloach Exp $

/**
 * @file
 * Adds support for the jQ module.
 *
 * jq_add('flowplayer', $selector, $config);
 */
function flowplayer_jq($op, $plugin = NULL) {
  switch ($op) {
    case 'info':
      return array(
        'flowplayer' => array(
          'name' => t('Flowplayer'),
          'description' => t('Flash video player.'),
          'version' => '3.1.3',
          'url' => 'http://flowplayer.org',
        ),
      );
    break;
    case 'add':
      switch ($plugin) {
        case 'flowplayer':
          // When adding the plugin, allow additional arguments so that
          // we can pass them through.
          $args = func_get_args();
          unset($args[0]); // $op
          unset($args[1]); // $plugin
          call_user_func_array('flowplayer_add', $args);
        break;
      }
    break;
  }
}
