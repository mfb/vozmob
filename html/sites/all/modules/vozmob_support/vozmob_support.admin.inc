<?php
// $Id$

/**
 * @file
 * MMS Messsaging configuration page.
 */

/**
 * Admin settings form
 */
function vozmob_support_messaging_mms_settings() {
  $form['vozmob_support_messaging_mms_from_node'] = array(
    '#title' => t('Node from address'),
    '#type' => 'textfield',
    '#default_value' => variable_get('vozmob_support_messaging_mms_from_node', NULL),
    '#description' => t('Enter an optional e-mail address to be used as the <em>From</em> address when sending a node via e-mail gateway. Token <em>[nid]</em> will be replaced with the node ID.'),
  );
  return system_settings_form($form);
}
