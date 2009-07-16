<?php
// $Id: twitter.pages.inc,v 1.2.2.9 2009/07/04 16:14:08 eaton Exp $

/**
 *
 */
function twitter_admin_form() {
  $form = array();

  $form['global_account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global account'),
    '#description' => t('A site-wide Twitter account to use as the default when tweets are posted. This is useful for single-user blogs or sites where many users post to a single shared Twitter account.'),
  );

  $form['global_account']['twitter_global_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter user name'),
    '#default_value' => variable_get('twitter_global_name', NULL),
  );
  
  $form['global_account']['twitter_global_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t("If your Twitter account is protected, or you wish to post to Twitter from Drupal, you must enter the Twitter account's password."),
    '#default_value' => variable_get('twitter_global_password', NULL),
  );

  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter import'),
    '#description' => t('Import and display the Twitter statuses of site users who have entered their Twitter account information.'),
  );

  $form['import']['twitter_import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import Twitter statuses'),
    '#default_value' => variable_get('twitter_import', TRUE),
  );

  $periods = array(0 => t('Never'));
  $periods += drupal_map_assoc(array(604800, 2419200, 7257600, 31449600), 'format_interval');
  $form['import']['twitter_expire'] = array(
    '#type' => 'select',
    '#title' => t('Delete old statuses'),
    '#default_value' => variable_get('amazon_refresh_schedule', 0),
    '#options' => $periods
  );

  $form['posting'] = array(
    '#type' => 'fieldset',
    '#title' => t('Twitter posting'),
    '#description' => t('Users with proper permissions will be given the option to post announcements to their Twitter accounts when they create new content.'),
  );

  $form['posting']['twitter_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types'),
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('twitter_types', array('story' => 'story', 'blog' => 'blog')),
  );

  $form['posting']['twitter_default_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Default format string'),
    '#maxlength' => 140,
    '#description' => t('The given text will be posted to twitter.com. You can use !url, !url-alias, !tinyurl, !title, and !user as replacement text.'),
    '#default_value' => variable_get('twitter_default_format', 'New post: !title !tinyurl'),
  );

  $form['twitter_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Alternate API URL'),
    '#default_value' => variable_get('twitter_api_url', 'twitter.com'),
    '#description' => t("A Twitter-compatible microblogging services like !identica can be used insted of Twitter.com by entering the service's API URL here.",
      array('!identica' => l('Identi.ca', 'http://laconi.ca/trac/wiki/TwitterCompatibleAPI'))),
  );

  $form['twitter_set_source'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set source to Drupal when updating Twitter status'),
    '#default_value' => variable_get('twitter_set_source', TRUE),
  );

  return system_settings_form($form);
}

function twitter_user_settings($account) {
  module_load_include('inc', 'twitter');

  $output = '';
  // This is directly calling a hook implementation, which is bad and naughty,
  // but oh well. We'll fix this in the next reshuffling when user account
  // management gets an overhaul.
  $twitter_accounts = twitter_twitter_accounts($account);
  if (!empty($twitter_accounts)) {
    $output .= drupal_get_form('twitter_account_list_form', $twitter_accounts);
  }
  $output .= drupal_get_form('twitter_add_account', $account);

  return $output;
}

function twitter_account_list_form($form_state, $twitter_accounts = array()) {
  $form['#tree'] = TRUE;
  $form['accounts'] = array();

  foreach ($twitter_accounts as $twitter_account) {
    $form['accounts'][] = _twitter_account_list_row($twitter_account);
  }

  if (!empty($twitter_accounts)) {
    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
    );
  }

  return $form;
}

function _twitter_account_list_row($account) {
  $form['#account'] = $account;

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account['uid'],
  );

  $form['screen_name'] = array(
    '#type' => 'value',
    '#value' => $account['screen_name'],
  );

  $form['image'] = array(
    '#type' => 'markup',
    '#value' => theme('image', $account['profile_image_url'], '', '', array(), FALSE),
  );

  $form['visible_name'] = array(
    '#type' => 'markup',
    '#value' => l($account['screen_name'], 'http://www.twitter.com/' . $account['screen_name']),
  );

  $form['description'] = array(
    '#type' => 'markup',
    '#value' => filter_xss($account['description']),
  );

  $form['protected'] = array(
    '#type' => 'markup',
    '#value' => empty($account['protected']) ? t('No') : t('Yes'),
  );

  $form['import'] = array(
    '#type' => 'checkbox',
    '#default_value' => $account['import'],
  );

  $form['delete'] = array(
    '#type' => 'checkbox',
  );
  return $form;
}

function theme_twitter_account_list_form($form) {
  $header = array('', t('Name'), t('Description'), t('Private'), t('Import'), t('Delete'));
  $rows = array();

  foreach (element_children($form['accounts']) as $key) {
    $element = &$form['accounts'][$key];
    $rows[] = array(
      drupal_render($element['image']),
      drupal_render($element['uid']) . drupal_render($element['screen_name']) . drupal_render($element['visible_name']),
      drupal_render($element['description']),
      drupal_render($element['protected']),
      drupal_render($element['import']),
      drupal_render($element['delete']),
    );
  }

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

function twitter_account_list_form_submit($form, &$form_state) {
  $accounts = $form_state['values']['accounts'];
  foreach($accounts as $account) {
    if (empty($account['delete'])) {
      twitter_user_save($account);
    }
    else {
      twitter_user_delete($account['uid'], $account['screen_name']);
    }
  }
}

function twitter_add_account($form_state, $account = NULL) {
  if (empty($account)) {
    global $user;
    $account = $user;
  }

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['screen_name'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Twitter user name'),
  );
  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#description' => t("If your Twitter account is protected, or you wish to post to Twitter from Drupal, you must enter the Twitter account's password.")
  );

  $form['import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import statuses from this account'),
    '#default_value' => TRUE,
    '#access' => FALSE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add account'),
  );

  return $form;
}

function twitter_add_account_validate($form, &$form_state) {
  module_load_include('inc', 'twitter');

  $verify = FALSE;

  $pass = $form_state['values']['password'];
  $name = $form_state['values']['screen_name'];

  if (!empty($pass)) {
    $verify = TRUE;
  }

  if ($verify) {
    $valid = twitter_authenticate($name, $pass);
    if (!$valid) {
      form_set_error("password", t('Twitter authentication failed. Please check your account name and try again.'));
    }
  }
}

function twitter_add_account_submit($form, &$form_state) {
  module_load_include('inc', 'twitter');

  if (!empty($form_state['values']['screen_name'])) {
    // if the password wasn't empty either get the account info
    // so we can figure out if the account is protected
    if (!empty($form_state['values']['password'])) {
      $account = twitter_fetch_account_info($form_state['values']['screen_name'], $form_state['values']['password']);
      if ($account['protected']) {
        $form_state['values']['protected'] = TRUE;
      }
    }
    twitter_user_save($form_state['values'], TRUE);
  }
}
