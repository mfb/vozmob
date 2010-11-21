<?php
// $Id: sms_devel.virtualgw.inc,v 1.1.4.2 2010/11/18 11:05:46 univate Exp $
/**
 * @file
 * Virtual Gateway for the sms_devel module of the SMS Framework.
 *
 * @package sms
 * @subpackage sms_devel
 */

/*
 A table is required to store activity, to be queried by the interactor page 
 Note: I have changed the "server-side number" nomenclature from "sender" & 
 "in_number" to simply "gw_number".
*/


define('SMS_DEVEL_VIRTUALGW_TYPE_OUT',     0);
define('SMS_DEVEL_VIRTUALGW_TYPE_IN',      1);
define('SMS_DEVEL_VIRTUALGW_TYPE_RECEIPT', 2);


/**
 * Callback for sending messages.
 *
 * Options for this send function:
 *   gw_number - The sender of the message. MSISDN or text string. Min=3, max=11 chars.
 *   reference - Message reference tag (to appear on any receipt).
 *
 * @param $number
 *   MSISDN of message recipient. Expected to include the country code prefix.
 * @param $message
 *   Message body text.
 * @param $options
 *   Options array from SMS Framework.
 * @return
 *   Gateway response array.
 */
function sms_devel_virtualgw_send($number, $message, $options) {
  // Set a default sender if it is not specified
  if (! array_key_exists('gw_number', $options)) {
    $options['gw_number'] = '99999';
  }

  // Set a default reference if it is not specified
  if (! array_key_exists('reference', $options)) {
    $options['reference'] = md5($number . $message);
  }

  // Write log record for outgoing message
  sms_devel_virtualgw_log_insert(SMS_DEVEL_VIRTUALGW_TYPE_OUT, $number, $message, $options);

  // Invoke additional virtual gateway features eg: autoreplies, receipts.
  sms_devel_virtualgw_sendlogic($number, $message, $options);

  // Always return success
  return array(
    'status'      => TRUE,
    'status_code' => SMS_GW_OK,
    'gateway_status_code' => 'OK',
    'gateway_status_text' => 'sms_devel_virtualgw: send: OK',
  );
}


function sms_devel_virtualgw_receiver($number = NULL, $message = NULL, $options = array()) {
  // Handle HTTP requests rather than direct function calls
  if ($number == NULL) {
    $number  = $_REQUEST['number'];
    $message = (array_key_exists('message', $_REQUEST)) ? $_REQUEST['message'] : 'NO_MESSAGE';
    $options['gw_number'] = (array_key_exists('gw_number', $_REQUEST)) ? $_REQUEST['gw_number'] : 'NO_GW_NUMBER';
    $options['reference'] = (array_key_exists('reference', $_REQUEST)) ? $_REQUEST['reference'] : 'NO_REFERENCE';
  }

  // Write log record for incoming message
  sms_devel_virtualgw_log_insert(SMS_DEVEL_VIRTUALGW_TYPE_IN, $number, $message, $options);

  // Call SMS Framework incoming message handler
  sms_incoming($number, $message, $options);
}


function sms_devel_virtualgw_log_insert($type, $number = '', $message = '', $options = array()) {
  $options_z = serialize($options);
  $result = db_query("INSERT INTO {sms_devel_virtualgw} (created, type, number,
    message, options) VALUES (%d, %d, '%s', '%s', '%s')",
    time(), $type, $number, $message, $options_z);
  return $result;
}


function sms_devel_virtualgw_sendlogic($number, $message, $options) {
  $autoreply_enabled = variable_get('sms_devel_virtualgw_autoreply_enabled', FALSE);
  $autoreply_format  = variable_get('sms_devel_virtualgw_autoreply_format',  '');
  $receipts_enabled  = variable_get('sms_devel_virtualgw_receipts_enabled',  FALSE);

  if ($autoreply_enabled) {
    // TODO Figure out the tokens logic
    //$echo_message = $autoreply_format . $message;
    $echo_message = 'echo: '. $message;

    // Swap the numbers for the echo
    $echo_number  = $options['gw_number'];
    $echo_options = array('gw_number' => $number);

    // Echo
    sms_devel_virtualgw_receiver($echo_number, $echo_message, $echo_options);
  }

  if ($receipts_enabled) {
    // Trigger a message delivery receipt
    $status = SMS_MSG_STATUS_DELIVERED;
    $reference = $options['reference'];
    $options['gateway_message_status'] = 'DELIVERED';
    $options['gateway_message_status_text'] = 'Message delivered OK';

    sms_receipt($number, $reference, $status, $options);
  }
}


function sms_devel_virtualgw_js_getactivity() {
  // We should not need to get a default limit - expect it in the request
  $limit = 50;
  // Get limit from request
  if (array_key_exists('rows', $_REQUEST)) {
    $limit = $_REQUEST['rows'];
  }
  // Handle zero or null limit
  if ( ! $limit ) { $limit = 1; }

  $lines = array();

  $result = db_query("SELECT * FROM {sms_devel_virtualgw} ORDER BY created LIMIT %d", $limit);
  while ($row = db_fetch_object($result)) {
    $options = unserialize($row->options);
    switch ($row->type) {
      case SMS_DEVEL_VIRTUALGW_TYPE_OUT:
        $from = $options['sender'];
        $to   = $row->number;
        ;;
      case SMS_DEVEL_VIRTUALGW_TYPE_IN:
        $from = $row->number;
        $to   = $options['sender'];
        ;;
      case SMS_DEVEL_VIRTUALGW_TYPE_RECEIPT:
        // Receipt
        ;;
    }
    $dir = $row->type;
    $msg = $row->message;

    $lines[] = "$dir $from $to $msg";
  }

  $output = implode("\n", $lines);

  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  // Get the form from the cache.
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  // We will run some of the submit handlers so we need to disable redirecting.
  $form['#redirect'] = FALSE;
  // We need to process the form, prepare for that by setting a few internals
  // variables.
  $form['#post'] = $_POST;
  $form['#programmed'] = FALSE;
  $form_state['post'] = $_POST;
  // Build, validate and if possible, submit the form.
  drupal_process_form($form_id, $form, $form_state);
  // This call recreates the form relying solely on the form_state that the
  // drupal_process_form set up.
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  // Render the new output.
  $logfield = $form['sms_devel_virtualgw_log']['sms_devel_virtualgw_logfield'];
  $logfield['#value'] = $output;
  unset($logfield['#prefix'], $logfield['#suffix']); // Prevent duplicate wrappers.
  $output = drupal_render($logfield);

  return drupal_json(array('status' => TRUE, 'data' => $output));
}


function sms_devel_virtualgw_ahah_send() {
  $from    = $_POST['sms_devel_virtualgw_from'];
  $to      = $_POST['sms_devel_virtualgw_to'];
  $message = $_POST['sms_devel_virtualgw_message'];

  $number  = $from;
  $options['gw_number'] = $to;

  // Call the receiver
  sms_devel_virtualgw_receiver($number, $message, $options);

  // Return a good status
  drupal_json(array('status' => TRUE));
}
