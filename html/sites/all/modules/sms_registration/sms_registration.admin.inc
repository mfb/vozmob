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

  $form['sms_registration_question_name_blog'] = array(
      '#type' => 'textfield',
      '#title' => t('First post message'),
      '#description' => t('This is the message the system send to user after the first post.'),
      '#default_value' => t('Thanks! Do you want to have a blog in vozmob? If yes please send one word identifying the blog.')
);

  $form['sms_registration_message_welcome_blog']  = array(
      '#type' => 'textfield',
      '#title' => t('Registration success message'),
      '#description' => t('This is the message vozmob sends to a user if they are successfully registered.'),
      '#default_value' => t('Welcome, !name. Your blog was created. Your password is !pass')
);
  $form['sms_registration_message_blog_not_created']  = array(
      '#type' => 'textfield',
      '#title' => t('Try again message'),
      '#description' => t('This is the message vozmob sends to a user if their registration attempt failed.'),
      '#default_value' => t('The blog was not created. Please choose another name.')
);

  return system_settings_form($form);
}
