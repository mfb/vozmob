<?php
// $Id$

/**
 * Render SMS Registration settings form.
 */
function sms_registration_settings() {
  $form['sms_registration_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Registration address'),
    '#description' => t('Enter the e-mail address used to receive site registrations.'),
    '#default_value' => variable_get('sms_registration_address', ''),
  );
  $form['sms_registration_request_user'] = array(
    '#type' => 'radios',
    '#options' => array('' => t('None'), 'name' => t('User name'), 'mail' => t('User e-mail address')),
    '#title' => t('Request user fields on registration'),
    '#description' => t('Select a user field to request from users after creating a new account.'),
    '#default_value' => variable_get('sms_registration_request_user', 'name'),
  );
  $form['sms_registration_username'] = array(
    '#type' => 'textfield',
    '#title' => t('User name prefix'),
    '#description' => t('Enter a user name prefix on randomly generated user names.'),
    '#default_value' => variable_get('sms_registration_username', 'Drupal-'),
  );
  $form['sms_registration_role'] = array(
    '#type' => 'select',
    '#title' => t('Default role'),
    '#description' => t('Select a default role granted to users registering via SMS.'),
    '#options' => user_roles(TRUE),
    '#default_value' => variable_get('sms_registration_role', NULL),
  );
  return system_settings_form($form);
}
