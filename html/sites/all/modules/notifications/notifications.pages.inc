<?php
// $Id: notifications.pages.inc,v 1.1.2.1 2008/11/08 20:25:09 jareyero Exp $
/**
 * @file
 * User pages for Notifications
 */

/**
 * Menu callback. Overview page for user subscriptions.
 */
function notifications_page_user_overview($account) {
  // Build summary
  $count = array();  
  // Query all subscriptions for this user
  $result = db_query("SELECT s.type, count(s.sid) as number FROM {notifications} s WHERE s.uid = %d  GROUP BY s.type", $account->uid);
  while ($subs = db_fetch_object($result)) {
    $count[$subs->type] =  $subs->number;
  }
  $header = array(t('Type'), t('Number'));
  $rows = array();
  
  // List types and count for each type
  foreach (notifications_subscription_types() as $type => $info) {
    $access = user_access('administer notifications') || !empty($info['access']) && user_access($info['access']);
    // If no access and no count, skip this type.
    // But if no access and there are some we show the type and number without link.
    if ($access || !empty($count[$type])) {
      $rows[] = array(
         $access ? l($info['title'], "user/$account->uid/notifications/$type") : $info['title'],
         isset($count[$type]) ? $count[$type] : 0,
      );
    }
  }

  return $rows ? theme('table', $header, $rows) : t('No existing or allowed subscriptions');
}

/**
 * Menu callback add subscription
 * 
 * Presents confirmation page or not depending on confirm parameter
 */
function notifications_page_subscribe($account, $type, $fields, $values, $send_interval = NULL, $send_method = NULL) {
  global $user;

  // Check signature if present
  $params = array('subscribe', $account->uid, $type, $fields, $values);
  $signed = !empty($_GET['signature']) && $_GET['signature'] == _notifications_signature($params, !empty($_GET['confirm']));
  if (!empty($_GET['signature'])) drupal_set_message(_notifications_signature($params, !empty($_GET['confirm'])));
  // Build subscriptions object
  $subscription = (object)array(
    'uid' => $account->uid,
    'type' => $type,
    'fields' => notifications_field_args($fields, $values),
    'send_interval' => $send_interval ? $send_interval : notifications_user_setting('send_interval', $account),
    'send_method' => $send_method ? $send_method : notifications_user_setting('send_method', $account),
    'event_type' => notifications_subscription_types($type, 'event_type'),
  );      
  if (notifications_user_allowed('subscription', $account, $subscription)) {
    // Display subscription information
    if (!empty($_GET['confirm']) && $signed) {
      // Subscribe, no confirmation
      notifications_save_subscription($subscription);
      drupal_set_message(t('Your subscription was activated.'));
      drupal_goto();
    }
    else {
      // Ask for confirmation
      drupal_set_title(t('Confirm your subscription'));
      return drupal_get_form('notifications_form_subscribe_confirm', $subscription, $account);
    }
  }
  else {
    drupal_set_message(t('Subscription type or parameters not allowed'), 'error');
    drupal_goto();
  }
    
  drupal_access_denied();
}

/**
 * Form for subscription confirmation
 */
function notifications_form_subscribe_confirm($form_state, $subscription, $account) {

  // Pass on simple values
  foreach (array('sid', 'uid', 'type', 'fields', 'event_type') as $field) {
    $form[$field] = array('#type' => 'value', '#value' => $subscription->$field);
  }

  // The names will be added here
  notifications_module_invoke('names', $subscription);

  $form['info'] = array(
    '#type' => 'item',
    '#title' => t('!type subscription to', array('!type' => $subscription->type_name)),
    '#value' => theme('item_list', $subscription->names),
  );

  // Additional parameters
  $form['send_interval'] = array(
    '#type' => 'select',
    '#title' => t('Send interval'),
    '#options' => _notifications_send_intervals(),
    '#default_value' => $subscription->send_interval,
  );
  $send_methods = _notifications_send_methods($account);
  $form['send_method'] = array(
    '#type' => 'select',
    '#title' => t('Send method'),
    '#options' => $send_methods,
    '#default_value' => $subscription->send_method,
    '#disabled' => count($send_methods) < 2,
  );

  $form['confirm'] = array('#type' => 'submit', '#value' => t('Subscribe'));
  $form['cancel'] = array('#type' => 'submit', '#value' => t('Cancel'));
  return $form;
}

/**
 * Process form submission
 */
function notifications_form_subscribe_confirm_submit($form, &$form_state) {
  $subscription = (object)$form_state['values'];
  switch ($form_state['values']['op']) {
    case t('Subscribe'):
      notifications_save_subscription($subscription);
      drupal_set_message(t('Your subscription was activated.'));
      break;
    case t('Cancel'):
      drupal_set_message(t('Your subscription was cancelled'));
      break;
  }
  return 'user/'. $subscription->uid .'/notifications';   
}

/**
 * Process arguments and return an array of field/value pairs
 */
function notifications_field_args($fields, $values) {
  $names = explode(',', $fields);
  $params = explode(',', $values);
  return array_combine($names, $params);  
}

/**
 * Menu callback add subscription
 * 
 * This just admits one field/value pair
 */
function notifications_page_unsubscribe($sid) {
  global $user;
  
  // Check signature if present
  $signed = !empty($_GET['signature']) && $_GET['signature'] == _notifications_signature(array('unsubscribe', $sid), !empty($_GET['confirm']));
  
  if (is_numeric($sid) && ($subscription = notifications_load_subscription($sid)) ) {
    if (user_access('administer notifications') ||
      ($user->uid && $user->uid == $subscription->uid) ||
      $signed
    ) {
      // Skip confirmation page when requested and the signature is ok
      if (!empty($_GET['confirm']) && $signed) {
        notifications_delete_subscription($sid);
        drupal_set_message(t('Your subscription has been removed.'));
        drupal_goto();
      } else {
        drupal_set_title(t('Unsubscribe'));
        return drupal_get_form('notifications_form_unsubscribe_confirm', $subscription);
      }
    }
  }
  drupal_access_denied();
}

/**
 * Form for unsubscription confirmation
 */
function notifications_form_unsubscribe_confirm($form_state, $subscription) {

  // Pass on subscription values
  $form['subscription'] = array('#type' => 'value', '#value' => $subscription);

  // The names will be added here
  notifications_module_invoke('names', $subscription);

  $form['info'] = array(
    '#type' => 'item',
    '#title' => t('!type subscription to', array('!type' => $subscription->type_name)),
    '#value' => theme('item_list', $subscription->names),
  );

  $form['confirm'] = array('#type' => 'submit', '#value' => t('Unsubscribe'));
  $form['cancel'] = array('#type' => 'submit', '#value' => t('Cancel'));
  return $form;
}

/**
 * Process form submission
 */
function notifications_form_unsubscribe_confirm_submit($form, &$form_state) {
  $subscription = $form_state['values']['subscription'];
  switch ($form_state['values']['op']) {
    case t('Unsubscribe'):
      notifications_delete_subscription($subscription->sid);
      drupal_set_message(t('Your subscription has been removed.'));
      break;
    case t('Cancel'):
      // Do nothing, not worth showing a message, just get back
      break;
  } 
}

/**
 * Edit subscription
 */
function notifications_subscription_form($form_state, $subscription) {
  // The names will be added here
  notifications_module_invoke('names', $subscription);
  $form['info'] = array(
    '#type' => 'item',
    '#title' => t('!type subscription to', array('!type' => $subscription->type_name)),
    '#value' => theme('item_list', $subscription->names),
  );

  // Subscription values
  
  $form['subscription'] = array('#type' => 'value', '#value' => $subscription);

  $form['send_interval'] = array(
    '#type' => 'select',
    '#title' => t('Send interval'),
    '#options' => _notifications_send_intervals(),
    '#default_value' => $subscription->send_interval,
  );
  $send_methods = _notifications_send_methods();
  $form['send_method'] = array(
    '#type' => 'select',
    '#title' => t('Send method'),
    '#options' => $send_methods,
    '#default_value' => $subscription->send_method,
  );
  $form['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#options' => _notifications_subscription_status(),
    '#default_value' => $subscription->status,
  );

  $form['save'] = array('#type' => 'submit', '#value' => t('Save'));
  
  $form['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
  
  return $form;  
}

/**
 * Save edited subscription
 */
function notifications_subscription_form_submit($form, $form_state) {
  // Rebuild subscription object
  $subscription = $form_state['values']['subscription'];
  foreach (array('send_interval', 'send_method', 'status') as $field) {
    if (isset($form_state['values'][$field])) {
      $subscription->$field = $form_state['values'][$field];
    }
  }
  switch($form_state['values']['op']) {
    case t('Save'):
      notifications_save_subscription($subscription);
      break;
    case t('Delete'):
      notifications_delete_subscription($subscription->sid);
      break;
  }
}