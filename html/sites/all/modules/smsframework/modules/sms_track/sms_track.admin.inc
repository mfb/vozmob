<?php
// $Id: sms_track.admin.inc,v 1.1.4.2 2010/11/18 11:05:46 univate Exp $
/**
 * @file
 * Message tracking module: Admin settings form functions
 *
 * @package sms
 * @subpackage sms_track
 */


/**
 * Admin settings form
 *
 * @return
 *   Drupal form array
 */
function sms_track_settings_form() {
  // Archive section
  $form['archive'] = array(
    '#type'  => 'fieldset',
    '#title' => 'Message archiving',
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $form['archive']['archive_dir'] = array(
    '#type'  => 'select',
    '#title' => 'Archive mode',
    '#default_value' => variable_get('sms_track_archive_dir', SMS_DIR_NONE),
    '#options' => array(
      SMS_DIR_NONE => 'No archiving [default]',
      SMS_DIR_OUT  => 'Record outgoing messages only',
      SMS_DIR_IN   => 'Record incoming messages only',
      SMS_DIR_ALL  => 'Record both outgoing and incoming messages',
    ),
    '#description' => t('Note that this will revert to the default option when the SMS Tracking module is disabled.'),
  );
  $form['archive']['archive_max_age_days'] = array(
    '#type'  => 'textfield',
    '#title' => 'Purge messages after n days',
    '#size'          => 3,
    '#maxlength'     => 3,
    '#default_value' => variable_get('sms_track_archive_max_age_days', 0),
    '#description'   => 'Set to 0 (zero) to disable archive purge. This will only work if you have cron configured correctly.',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


function sms_track_settings_form_submit($form, &$form_state) {
  $archive_dir_old = variable_get('sms_track_archive_dir', SMS_DIR_NONE);
  $archive_dir = $form_state['values']['archive_dir'];
  variable_set('sms_track_archive_dir', $archive_dir);

  $archive_max_age_days = $form_state['values']['archive_max_age_days'];
  variable_set('sms_track_archive_max_age_days', $archive_max_age_days);

  // Trigger watchdog messages
  if ($archive_dir_old && ! $archive_dir) {
    watchdog('sms_track', 'SMS Tracking archive collector DISABLED');
  }
  if (! $archive_dir_old && $archive_dir) {
    watchdog('sms_track', 'SMS Tracking archive collector enabled');
  }

  drupal_set_message('Settings saved.');
}


/**
 * Admin View page
 *
 * @return
 *   HTML content string
 */
function sms_track_admin_view() {
  $content = views_embed_view('sms_track', $display_id = 'default');
  return $content;
}
