<?php
// $Id: sms_user.actions.inc,v 1.1.2.1 2010/11/18 06:14:33 univate Exp $
/**
 * @file
 * SMS User Action Implementation.
 *
 */

/**
 * Implementation of hook_action_info().
 */
function sms_user_action_info() {
  $info['sms_user_send_sms_action'] = array(
      'type' => 'user',
      'description' => t('Send SMS to Users'),
      'configurable' => TRUE,
  );
  return $info;
}

function sms_user_send_sms_action(&$user, $context) {
  $message = $context['sms_text'];
  sms_user_send($user->uid, $message);
}

/**
 * Form for 'send sms' action.
 *
 * @param array $context
 * @return $form
 */
function sms_user_send_sms_action_form($context) {
  $limit = variable_get('sms_user_max_chars', SMS_USER_MAX_CHARS);
  drupal_add_js(drupal_get_path('module', 'sms_user') .'/sms_user.js');

  $form['sms_text'] = array(
    '#type'  => 'textarea',
    '#title' => 'SMS Text',
    '#required' => TRUE,
  );
  $form['sms_limit'] = array(
    '#type'   => 'markup',
    '#prefix' => '<div id="keystrokes" class="description" limit="'. $limit .'"><span></span>',
    '#value'  => t('@max chars max', array('@max' => $limit)),
    '#suffix' => '</div>',
  );

  return $form;
}

function sms_user_send_sms_action_validate($form, $form_state) {
  $sms_text = $form_state['values']['sms_text'];
  if (empty($sms_text)) {
    form_set_error('sms_text', t('This field is required'));
  }
  $length = drupal_strlen($sms_text);
  if ($length > variable_get('sms_user_max_chars', SMS_USER_MAX_CHARS)) {
    $error = t('SMS too long: (@text) [length=@len]',
        array('@text' => $sms_text, '@len' => $length));
    form_set_error('sms_text', $error);
  }
}

/**
 * Custom submit handler when creating a new action for sending an sms.
 *
 * @param array $form
 * @param array $form_state
 * @return array
 */
function sms_user_send_sms_action_submit($form, $form_state) {
  $context['sms_text'] = $form_state['values']['sms_text'];
  return $context;
}
