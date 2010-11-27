<?php
// $Id: sms_devel.virtualgw.admin.inc,v 1.1.4.2 2010/11/18 11:05:46 univate Exp $
/**
 * @file
 * Virtual Gateway for the sms_devel module: Admin forms include
 *
 * @package sms
 * @subpackage sms_devel
 */


/**
 * Configuration form for gateway module
 *
 * @param $configuration
 *
 * @return
 *   Drupal form array
 */
function sms_devel_virtualgw_settings_form($configuration) {
  $form['sms_devel_virtualgw_link'] = array(
    '#type'  => 'item',
    '#value' => 'See also: '. l('Virtual Gateway dashboard', 'admin/smsframework/devel/virtualgw'),
  );

  return $form;
}


/**
 * The big form
 *
 * @return
 *   Drupal system settings form array.
 */
function sms_devel_virtualgw_form() {
  // Include js

  $form['sms_devel_virtualgw_settings'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );
  $form['sms_devel_virtualgw_settings']['sms_devel_virtualgw_autoreply_enabled'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Enable autoreply'),
    '#default_value' => variable_get('sms_devel_virtualgw_autoreply_enabled', FALSE),
  );
  $form['sms_devel_virtualgw_settings']['sms_devel_virtualgw_autoreply_format'] = array(
    '#type'  => 'textarea',
    '#rows'  => 4,
    '#cols'  => 40,
    '#resizable' => FALSE,
    '#default_value' => variable_get('sms_devel_virtualgw_autoreply_format', 'echo: {message}'),
    '#description'   => t('If enabled then the gateway will reply to your messages through sms_incoming()<br />You may use these tokens: {number} {gw_number} {message} {reference}'),
  );
  $form['sms_devel_virtualgw_settings']['sms_devel_virtualgw_receipts_enabled'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Enable receipts'),
    '#default_value' => variable_get('sms_devel_virtualgw_receipts_enabled', FALSE),
    '#description'   => t('If enabled then the gateway will provide a message receipt through sms_receipt()'),
  );
  $form['sms_devel_virtualgw_settings']['submit'] = array(
    '#type'   => 'submit',
    '#value'  => t('Save'),
    '#submit' => array('sms_devel_virtualgw_form_save_settings'),
  );

  $form['sms_devel_virtualgw_send'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Send a message from the gateway to SMS Framework'),
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );
  $form['sms_devel_virtualgw_send']['sms_devel_virtualgw_from'] = array(
    '#type'  => 'textfield',
    '#title' => t('From ($number)'),
    '#size'  => 30,
  );
  $form['sms_devel_virtualgw_send']['sms_devel_virtualgw_to'] = array(
    '#type'  => 'textfield',
    '#title' => t('To ($options[\'gw_number\'])'),
    '#size'  => 30,
  );
  $form['sms_devel_virtualgw_send']['sms_devel_virtualgw_message'] = array(
    '#type'  => 'textarea',
    '#title' => t('Message ($message)'),
    '#rows'  => 4,
    '#cols'  => 40,
    '#resizable' => FALSE,
  );
  $form['sms_devel_virtualgw_send']['send'] = array(
    '#type'  => 'button',
    '#value' => t('Send'),
    '#ahah'  => array(
      'path'    => 'admin/smsframework/devel/virtualgw/send',
      //'event'   => 'mousedown',
      //'method'  => 'replace',
      //'wrapper' => 'logfield',
    ),
    // onclick="sendSmsMessage();"
  );

  $form['sms_devel_virtualgw_log'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Activity log'),
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $form['sms_devel_virtualgw_log']['sms_devel_virtualgw_logfield_refresh'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Refresh the logfield'),
    '#default_value' => (variable_get('sms_devel_virtualgw_logfield_refresh', 1)) ? 1 : 0,
    // onchange="toggleRefresh(this.checked);"
  );
  $form['sms_devel_virtualgw_log']['sms_devel_virtualgw_logfield_showsent'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Show sent messages'),
    '#default_value' => (variable_get('sms_devel_virtualgw_logfield_showsent', 1)) ? 1 : 0,
  );
  $form['sms_devel_virtualgw_log']['sms_devel_virtualgw_logfield_showreceived'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Show received messages'),
    '#default_value' => (variable_get('sms_devel_virtualgw_logfield_showreceived', 1)) ? 1 : 0,
  );
  $form['sms_devel_virtualgw_log']['sms_devel_virtualgw_logfield_showreceipts'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Show message receipts'),
    '#default_value' => (variable_get('sms_devel_virtualgw_logfield_showreceipts', 1)) ? 1 : 0,
  );
  $form['sms_devel_virtualgw_log']['sms_devel_virtualgw_logfield_lines'] = array(
    '#type'  => 'textfield',
    '#title' => t('Number of recent activity records to pull'),
    '#size'  => 4,
  );
  $form['sms_devel_virtualgw_log']['sms_devel_virtualgw_logfield_refreshes'] = array(
    '#type'  => 'item',
    '#title' => t('Refresh count'),
    '#value' => t(''),
  );
  $form['sms_devel_virtualgw_log']['sms_devel_virtualgw_logfield'] = array(
    '#type'  => 'textarea',
    '#title' => t('Virtual gateway activity'),
    '#rows'  => 20,
    '#cols'  => 60,
    '#resizable' => TRUE,
    '#prefix' => '<div id="logfield">',
    '#value'  => 'Sorry, this feature is not finished yet.',
    '#suffix' => '</div>',
  );
  $form['sms_devel_virtualgw_log']['get'] = array(
    '#type'  => 'button',
    '#value' => t('Get activity'),
    '#ahah'  => array(
      'path'    => 'admin/smsframework/devel/virtualgw/getactivity',
      //'event'   => 'mousedown',
      'method'  => 'replace',
      'wrapper' => 'logfield',
    ),
    // onclick="sendSmsMessage();"
  );

  return $form;
}


function sms_devel_virtualgw_form_save_settings($form, &$form_state) {
  variable_set('sms_devel_virtualgw_autoreply_enabled',
               $form_state['values']['sms_devel_virtualgw_autoreply_enabled']);
  variable_set('sms_devel_virtualgw_autoreply_format',
               $form_state['values']['sms_devel_virtualgw_autoreply_format']);
  variable_set('sms_devel_virtualgw_receipts_enabled',
               $form_state['values']['sms_devel_virtualgw_receipts_enabled']);
}


/**
 * Admin View page
 *
 * @return
 *   HTML content string.
 */
/*
function sms_devel_virtualgw_admin_view() {
  $content = views_embed_view('sms_devel_virtualgw', $display_id = 'default');
  return $content;
}
*/
