<?php
// $Id: notifications.admin.inc,v 1.5.2.6.2.16 2008/12/30 15:29:44 jareyero Exp $

/**
 * Admin settings
 */
function notifications_settings_form() {
  $form['general'] = array(
      '#type' => 'fieldset',
      '#title' => t('General settings'),
      '#weight' => -10,
  );
  $form['general']['notifications_sendself'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Notify poster of own posts'),
    '#default_value' => variable_get('notifications_sendself', 0),
    '#description'   => t("Notifies a node poster about their own posts.  Useful principally during testing.  Default is OFF."),
  );
  $form['general']['notifications_send_immediate'] = array(
    '#title' => t('Immediate sending'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('notifications_send_immediate', 0),
    '#description' => t('Notifications are usually queued to be sent on cron process later. Checking this option will cause immediate notifications to be sent right away, instead of being queued. This will produce more timely notifications for sites with a small number of users. Not recommended for sites with a large number of users.'),
  );
  $form['general']['notifications_sender'] = array(
    '#title' => t('Notifications Sender'),
    '#type' => 'radios',
    '#options' => array(
      t('No one (All notifications will appear as coming from the web site)'),
      t('User name, site data (Only the user name will be used)'),
      t('Full user data (User name and available user information)'),
    ),
    '#default_value' => variable_get('notifications_sender', 0),
    '#description' => t('Use the site information as the sender for notification messages or use account data from the user causing the event. WARNING: Selecting the last option (Full user data) may undisclose private information to subscribers like the user e-mail address.'),
  );
  // Logging settings  
  $period = array(0 => t('Disabled')) + drupal_map_assoc(array(3600, 10800, 21600, 32400, 43200, 86400, 172800, 259200, 604800, 1209600, 2419200, 4838400, 9676800), 'format_interval');
  $form['general']['notifications_log'] = array(
    '#title' => t('Logging'),
    '#type' => 'select',
    '#options' => $period,
    '#default_value' => variable_get('notifications_log', 0),
    '#description' => t('If enabled all notifications will be logged and kept for the specified time after they\'re processed.'),
  );
  // Default options
  $form['defaults'] = array(
      '#type' => 'fieldset',
      '#title' => t('Default settings'),
  );
  $form['defaults']['notifications_default_send_interval'] = array(
    '#type' => 'select',
    '#title' => t('Default send interval'),
    '#options' => _notifications_send_intervals(),
    '#default_value' => variable_get('notifications_default_send_interval', 0),
  );

  // Processing limits
  $limit = variable_get('notifications_process_limit', array('row' => 0, 'message' => 0, 'percent' => 0, 'time' => 0));
  $form['notifications_process_limit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Limits for queue processing'),
    '#tree' => TRUE,
    '#description' => t('These are the limits for each cron run on queue processing. The process will stop when it first meets any of them. Set to 0 for no limit.'),
    '#collapsible' => TRUE, '#collapsed' => TRUE,
  );
  $form['notifications_process_limit']['row'] = array(
    '#title' => t('Number of rows'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $limit['row'],
  );
  $form['notifications_process_limit']['message'] = array(
    '#title' => t('Number of messages sent'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $limit['message'],
  );
  $form['notifications_process_limit']['time'] = array(
    '#title' => t('Time (seconds)'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $limit['time'],
  );
  $form['notifications_process_limit']['percent'] = array(
    '#title' => t('Time (% of cron time)'),
    '#type' => 'textfield',
    '#size' => 10,
    '#default_value' => $limit['percent'],
    '#description' => t('Maximum percentage of cron time the process may use.'),
  );
  return system_settings_form($form);
}

/**
 * Send intervals administration
 */
function notifications_send_intervals_form() {
  // Collect information about digesting methods and print out some help
  $form['digest'] = array(
    '#title' => t('Digest formats'),
    '#type' => 'fieldset',
    '#description' => t('These are the digest formats available for each interval.')
  );
  $digest_methods[''] = t('None');
  foreach (notifications_digest_method() as $type => $method) {
    $digest_methods[$type] = $method['name'];
    $rows[] = array($method['name'], $method['description']);    
  }
  $form['digest']['info'] = array('#value' => theme('table', array(), $rows));
  
  // Build options
  $units = array(
    60 => t('Minutes'),
    60*60 => t('Hours'), 
    24*60*60 => t('Days'),
  );
  $form['intervals'] = array(
    '#type' => 'fieldset',
    '#title' => t('Send intervals'),
    '#tree' => TRUE,
    '#theme' => 'notifications_send_intervals',
    '#description' => t('To delete an interval, set the time unit and the name empty.'),
  );
  $intervals = _notifications_send_intervals() + array('new1' => '', 'new2' => '');
  $index = 0;
  foreach ($intervals as $time => $name) {
    // Calculate value and unit
    $current = $number = 0; 
    if (!is_numeric($time)) {
      $number = $unit = '';
    }
    elseif ($time >= 0) {
      foreach (array_reverse(array_keys($units)) as $unit) {
        if ($time % $unit == 0) {
          $current = $unit;
          $number = $time / $unit;
          break;
        }
      }
    }
    else {
      // May be -1 for 'Never'
      $number = $time;
      $unit = '';
    }
    $form['intervals'][$index]['time'] = array(
      '#default_value' => $number,
      '#type' => 'textfield',
      '#size' => 2,
    );
    $form['intervals'][$index]['unit'] = array(
      '#type' => 'select',
      '#options' => array('' => '') + $units,
      '#default_value' => $unit,
    );
    $form['intervals'][$index]['name'] = array(
      '#type' => 'textfield',
      '#default_value' => $name,
      '#size' => 40,
    );
    $digest = notifications_digest_method($time);
    $form['intervals'][$index]['digest'] = array(
      '#type' => 'select',
      '#disabled' => ($number < 0),
      '#default_value' => $digest ? $digest['type'] : '',
      '#options' => $digest_methods,
    );
    $index++;
  }
  // New row
  $form['submit'] = array('#type' => 'submit', '#value' => t('Update'));
  $form['reset'] = array('#type' => 'submit', '#value' => t('Reset to defaults'));

  return $form;
}

/**
 * Form submit for time intervals
 */
function notifications_send_intervals_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  if ($form_values['op'] == t('Reset to defaults')) {
    variable_del('notifications_send_intervals');
    variable_del('notifications_digest_methods');
  }
  else {
    $intervals = array();
    $digest = array();
    foreach ($form_values['intervals'] as $index => $values) {
      if (is_numeric($values['time']) && $values['name']) {
        $unit = $values['unit'] ? (int)$values['unit'] : 1;
        $time = (int)$values['time'] * $unit;
        $intervals[$time] = $values['name'];
        $digest[$time] = $values['digest'];
      }
    }
    ksort($intervals);
    variable_set('notifications_send_intervals', $intervals);
    variable_set('notifications_digest_methods', $digest);
  }
  drupal_set_message(t('The time intervals for your subscriptions have been updated'));
}

/**
 * Build a table with send intervals
 */
function theme_notifications_send_intervals($element) {
  $output = '';
  $header = array(array('data' => t('Time'), 'colspan' => 2), t('Display name'), t('Digest'));
  foreach (element_children($element) as $key) {
    $rows[] = array(
      drupal_render($element[$key]['time']),
      drupal_render($element[$key]['unit']),
      drupal_render($element[$key]['name']),
      drupal_render($element[$key]['digest']),
    );  
  }
  $output .= theme('table', $header, $rows);
  $output .= drupal_render($element);
  return $output;
}

/**
 * Event configuration administration
 * 
 * Will allow to define which events trigger a notification and which ones not
 */
function notifications_admin_events_form() {
  // Compile array from plug-ins and settings
  $events = notifications_event_types();
  $current = variable_get('notifications_events', array());

  if ($events) {
    $form['notifications_events'] = array(
      '#title' => t('Enabled events'),
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#description' => t('Check the events for which notifications should be triggered.'),
    );
    foreach ($events as $object => $object_info) {
      foreach ($object_info as $action => $action_info) {
        $form['notifications_events'][$object][$action] = array(
          '#type' => 'checkbox',
          '#title' => !empty($action_info['description']) ? $action_info['description'] : "$object:$action",
          '#default_value' => isset($current[$object][$action]) ? $current[$object][$action] : 1,
        );
      }
    }
    return system_settings_form($form);
  } else {
    $form['warning'] = array('#value' => t('You need to enable some plug-ins to provide notifications events.'));
    return $form;
  }  
}

/* ********************************************t*********** */
/*  user screens: display, edit functions */
/* ******************************************************* */

/**
 * Theme subscriptions list
 */
function theme_notifications_form_table($element) {
  $output = '';
  if ($fields = element_children($element)) {
    $header = $element['#header'];
    $rows = array();
    // The first element determines the number of columns
    foreach (element_children($element[$fields[key($fields)]]) as $index) {
      $row = array();
      foreach ($fields as $key) {
        $row[] = isset($element[$key][$index]) ? drupal_render($element[$key][$index]) : '';
      }
      $rows[] = $row;
    }
    $output .= theme('table', $header, $rows);
  }
  $output .= drupal_render($element);
  return $output;
}

/** Administration pages **/

/**
 * Current subscriptions page
 */
function notifications_admin_status_page() {
  $output = '';
  
  // Subscriptions summary by type
  $header = array(t('Type'), t('Number'));
  $result = db_query("SELECT type, count(*) AS count FROM {notifications} GROUP BY type");
  $count = 0;
  $types = notifications_subscription_types();
  while ($stype = db_fetch_object($result)) {
    $rows[] = array(
      !empty($types[$stype->type]['title']) ? $types[$stype->type]['title'] : '<strong>' . $stype->type . '</strong>' , 
      $stype->count);
    $count += $stype->count;
  }
  $summary = theme('table', $header, $rows);
  $summary .= t('Total: %number', array('%number' => $count));
  $output .= theme('box', t('Subscriptions by type'), $summary);
  
  // Summary by sending method
  $header = array(t('Method'), t('Number'));
  $result = db_query("SELECT send_method, count(*) AS count FROM {notifications} GROUP BY send_method");
  while ($subs = db_fetch_object($result)) {
    $name = messaging_method_info($subs->send_method, 'title');
    $rows[] = array(
      $name ? $name : '<strong>' . $subs->send_method . '</strong>' , 
      $subs->count);
  }
  $summary = theme('table', $header, $rows);
  $output .= theme('box', t('Subscriptions by send method'), $summary); 
  
  // Queue summary 
  $output .= notifications_admin_queue_summary();
  return $output;  
}

/**
 * Summary of queued notifications
 */
function notifications_admin_queue_summary() {
  $output = '';
  // Queue status
  $send_intervals = _notifications_send_intervals();
  $header = array(t('Send interval'), t('Number'));
  $count = 0;
  $result = db_query("SELECT send_interval, count(*) AS count FROM {notifications_queue} GROUP BY send_interval");    
  while ($stype = db_fetch_object($result)) {
    $rows[] = array($send_intervals[$stype->send_interval], $stype->count);
    $count += $stype->count;
  }
  $output .= theme('table', $header, $rows);
  $output .= t('Total: %number', array('%number' => $count));
  return theme('box', t('Notifications in queue'), $output);
}

/**
 * Admin queue management
 * 
 * @ TO DO Add confirmation before queue reset
 */
function notifications_admin_queue($op = 'status', $param = NULL) {
  $output = '';
  $output .= drupal_get_form('notifications_admin_queue_operations');
  $output .= notifications_admin_queue_summary();

  // Display logs from last process
  if (!empty($_SESSION['notifications_logs'])) {
    $output .= theme('box', t('Process log'), theme('item_list', $_SESSION['notifications_logs']));
    unset($_SESSION['notifications_logs']);
  }
  return $output;
}

/**
 * Form for queue operations
 */
function notifications_admin_queue_operations() {
  $form['operations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Operations'),
    '#theme' => 'notifications_table_form',
    '#table_fields' => array('description', 'button'),
  );
  foreach (notifications_queue_operations() as $op => $data) {
    $form['operations']['description'][$op] = array(
      '#value' => $data['description'],
    );
    $form['operations']['button'][$op] = array(
      '#type' => 'submit',
      '#value' => $data['label'],
    );
  }
  return $form; 
}

/**
 * Operations form submit, translate op into callback
 */
function notifications_admin_queue_operations_submit($form, $form_state) {
  include_once drupal_get_path('module', 'notifications') .'/notifications.cron.inc';

  foreach (notifications_queue_operations() as $op => $data) {
    if ($form_state['values']['op'] == $data['label']) {
      $callback = $data['callback'];
      $args = !empty($data['callback arguments']) ? $data['callback arguments'] : array();
      call_user_func_array($callback, $args);
      break;
    }  
  }
  // Store logs to render on next page redirect
  $_SESSION['notifications_logs'] = notifications_log();
}

/**
 * List of queue operations
 */
function notifications_queue_operations() {
  $operations = array(
    'run' => array(
      'label' => t('Run process'),
      'description' => t('Run normal queue processing, same as cron run.'),
      'callback' => 'notifications_process_run',
      'callback arguments' => array(FALSE),
    ),
    'immediate' => array(
      'label' => t('Process immediate'),
      'description' => t('Process only rows marked for immediate sending.'),
      'callback' => 'notifications_process_rows',
      'callback arguments' => array(array('cron' => 1, 'send_interval' => 0)),
    ),
    'reset' => array(
      'label' => t('Reset queue'),
      'description' => t('Delete all notifications in queue.'),
      'callback' => 'notifications_admin_queue_process',
      'callback arguments' => array('reset queue'),
    ),
    'test' => array(
      'label' => t('Run test'),
      'description' => t('Test run queue processing, without updating nor sending messages.'),
      'callback' => 'notifications_admin_queue_process',
      'callback arguments' => array('test run'),
    ),
  );
  return $operations;
}

/**
 * Queue operations callback
 */
function notifications_admin_queue_process($op) {
  switch ($op) {
    case 'reset queue':
      db_query("DELETE FROM {notifications_queue}");
      db_query("DELETE FROM {notifications_event}");
      drupal_set_message(t('The queue has been reset.'));
      break;
    case 'test run':
      notifications_process('option', 'test', TRUE);
      notifications_process_run(FALSE);
      break;
  }
}

/**
 * Menu callback: subscriptions administration.
 */
function notifications_admin_subscriptions($form_state) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return notifications_multiple_delete_confirm($form_state, array_filter($form_state['values']['subscriptions']));
  }
  $form = notifications_subscriptions_filter_form();

  $form['#theme'] = 'notifications_subscriptions_filter_form';
  $form['admin']  = notifications_admin_subscriptions_form();

  return $form;
}

/**
 * Return form for node administration filters.
 */
function notifications_subscriptions_filter_form() {
  $session = &$_SESSION['subscriptions_overview_filter'];
  $session = is_array($session) ? $session : array();
  $filters = notifications_subscriptions_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'node_filters', // We reuse this theme from node admin pages
  );
  $form['#submit'][] = 'notifications_subscriptions_filter_form_submit';
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    $value = $filters[$type]['options'][$value];
    $form['filters']['current'][] = array('#value' => t('<strong>%a</strong> is <strong>%b</strong>', array('%a' => $filters[$type]['title'], '%b' => $value)));
  }

  foreach ($filters as $key => $filter) {
    $names[$key] = $filter['title'];
    $form['filters']['status'][$key] = array('#type' => 'select', '#options' => $filter['options']);
  }

  $form['filters']['filter'] = array('#type' => 'radios', '#options' => $names, '#default_value' => 'status');
  $form['filters']['buttons']['submit'] = array('#type' => 'submit', '#value' => (count($session) ? t('Refine') : t('Filter')));
  if (count($session)) {
    $form['filters']['buttons']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['buttons']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_js('misc/form.js', 'core');

  return $form;
}

/**
 * Process result from node administration filter form.
 */
function notifications_subscriptions_filter_form_submit($form, &$form_state) {
  $filters = notifications_subscriptions_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
    case t('Refine'):
      if (isset($form_state['values']['filter'])) {
        $filter = $form_state['values']['filter'];

        // Flatten the options array to accommodate hierarchical/nested options.
        $flat_options = form_options_flatten($filters[$filter]['options']);

        if (isset($flat_options[$form_state['values'][$filter]])) {
          $_SESSION['subscriptions_overview_filter'][] = array($filter, $form_state['values'][$filter]);
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['subscriptions_overview_filter']);
      break;
    case t('Reset'):
      $_SESSION['subscriptions_overview_filter'] = array();
      break;
  }
}

/**
 * Administer subscriptions
 */
function notifications_admin_subscriptions_form() {
  $filter = notifications_subscriptions_build_filter_query();

  $result = pager_query('SELECT n.*, u.name FROM {notifications} n '. $filter['join'] .' INNER JOIN {users} u ON n.uid = u.uid '. $filter['where'] .' ORDER BY n.sid DESC', 50, 0, NULL, $filter['args']);

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $options = array();
  foreach (notifications_subscriptions_operations() as $operation => $array) {
    if (!empty($array['parent'])) {
      $options[$array['parent']][$operation] = $array['label'];
    } else {  
      $options[$operation] = $array['label'];
    }
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => 'approve',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('notifications_admin_subscriptions_form_validate'),
    '#submit' => array('notifications_admin_subscriptions_form_submit'),
  );

  $destination = drupal_get_destination();
  $subscriptions = array();
  $status = _notifications_subscription_status();
  $send_methods = messaging_method_info(NULL, 'name');
  $send_intervals = _notifications_send_intervals();
  while ($sub = db_fetch_object($result)) {
    $subscriptions[$sub->sid] = '';
    $name = notifications_subscription_types($sub->type, 'title');
    $form['type'][$sub->sid] = array('#value' => $name ? $name : $sub->type );
    $form['username'][$sub->sid] = array('#value' => theme('username', $sub));
    $form['send_method'][$sub->sid] = array('#value' => !empty($send_methods[$sub->send_method]) ? $send_methods[$sub->send_method] : $sub->send_method);
    $form['send_interval'][$sub->sid] = array('#value' => !empty($send_intervals[$sub->send_interval]) ? $send_intervals[$sub->send_interval] : $sub->send_interval);  
    $form['status'][$sub->sid] =  array('#value' => $status[$sub->status]);
    $operations = array(
      l(t('edit'), 'notifications/subscription/' . $sub->sid, array('query' => $destination)),
      l(t('drop'), 'notifications/unsubscribe/' . $sub->sid, array('query' => $destination)),
    );
    $form['operations'][$sub->sid] = array('#value' => implode(', ', $operations));
  }
  $form['subscriptions'] = array('#type' => 'checkboxes', '#options' => $subscriptions);
  $form['pager'] = array('#value' => theme('pager', NULL, 50, 0));
  $form['#theme'] = 'notifications_admin_subscriptions';
  return $form;
}

/**
 * Validate notifications_admin_subscriptions form submissions.
 * 
 * Check if any items have been selected to perform the chosen
 * 'Update option' on.
 */
function notifications_admin_subscriptions_form_validate($form, &$form_state) {
  $items = array_filter($form_state['values']['subscriptions']);
  if (count($items) == 0) {
    form_set_error('', t('No items selected.'));
  }
}

function notifications_admin_subscriptions_form_submit($form, $form_state) {
  $operations = notifications_subscriptions_operations();
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked subscriptions
  $items = array_filter($form_state['values']['subscriptions']);
  
  if ($function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($items), $operation['callback arguments']);
    }
    else {
      $args = array($items);
    }
    call_user_func_array($function, $args);
  }
  else {
    // We need to rebuild the form to go to a second step.  For example, to
    // show the confirmation form for the deletion of nodes.
     $form_state['rebuild'] = TRUE;
  }
}

/**
 * Form for multiple delete
 */
function notifications_multiple_delete_confirm(&$form_state, $items) {

  $form['items'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  // array_filter returns only elements with TRUE values
  foreach ($items as $id => $value) {
    $title = t('Subscription %id', array('%id' => $id));
    $form['items'][$id] = array(
      '#type' => 'hidden',
      '#value' => $id,
      '#prefix' => '<li>',
      '#suffix' => check_plain($title) ."</li>\n",
    );
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'notifications_multiple_delete_confirm_submit';
  return confirm_form($form,
                      t('Are you sure you want to delete these items?'),
                      'admin/content/node', t('This action cannot be undone.'),
                      t('Delete all'), t('Cancel'));
}

function notifications_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach ($form_state['values']['items'] as $id => $value) {
      notifications_delete_subscription($id);
    }
    drupal_set_message(t('The items have been deleted.'));
  }
  $form_state['redirect'] = 'admin/messaging/subscriptions/admin';
  return;
}

/**
 * Build query for node administration filters based on session.
 */
function notifications_subscriptions_build_filter_query() {
  $filters = notifications_subscriptions_filters();

  // Build query
  $where = $args = array();
  $join = '';
  foreach ($_SESSION['subscriptions_overview_filter'] as $index => $filter) {
    list($key, $value) = $filter;
    switch ($key) {
      case 'status':
      case 'send_interval':
        $where[] = "n.$key = %d";
        $args[] = $value;
        break;
      case 'send_method':
      case 'type':
        $where[] = "n.$key = '%s'";
        $args[] = $value;
        break;
    }
    $args[] = $value;
  }
  $where = count($where) ? 'WHERE '. implode(' AND ', $where) : '';

  return array('where' => $where, 'join' => $join, 'args' => $args);
}

/**
 * List node administration filters that can be applied.
 */
function notifications_subscriptions_filters() {
  // Regular filters
  $filters['status'] = array(
    'title' => t('status'),
    'options' => _notifications_subscription_status(),
  );

  $filters['type'] = array(
    'title' => t('type'), 
    'options' => notifications_subscription_types(NULL, 'title')
  );

  $filters['send_method'] = array(
    'title' => t('method'), 
    'options' => messaging_method_info(NULL, 'title')
  );
  
  $filters['send_interval'] = array(
    'title' => t('interval'), 
    'options' => _notifications_send_intervals(),
  );
  
  return $filters;
}

/**
 * Subscription mass operations.
 */
function notifications_subscriptions_operations() {
  $operations = array(
    'block' => array(
      'label' => t('Block'),
      'callback' => 'notifications_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('status' => NOTIFICATIONS_SUBSCRIPTION_BLOCKED)),
    ),
    'activate' => array(
      'label' => t('Activate'),
      'callback' => 'notifications_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('status' => NOTIFICATIONS_SUBSCRIPTION_ACTIVE)),
    ),
    'deactivate' => array(
      'label' => t('Deactivate'),
      'callback' => 'notifications_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('status' => NOTIFICATIONS_SUBSCRIPTION_INACTIVE)),
    ),
    'delete' => array(
      'label' => t('Delete'),
      'callback' => NULL,
    ),
  );
  // Sending methods
  $parent = t('Change send method to');
  foreach (messaging_method_info(NULL, 'name') as $key => $name) {
    $operations['send_method-' . $key] = array(
      'label' => $name,
      'parent' => $parent,
      'callback' => 'notifications_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('send_method' => $key)),
    );
  }
  $parent = t('Change send interval to');
  foreach (_notifications_send_intervals() as $key => $name) {
    $operations['send_method-' . $key] = array(
      'label' => $name,
      'parent' => $parent,
      'callback' => 'notifications_subscriptions_mass_update',
      'callback arguments' => array('updates' => array('send_interval' => $key)),
    );
  }

  // Intervals
  return $operations;
}

/**
 * Make mass update of subscriptions, changing all nodes in the $nodes array
 * to update them with the field values in $updates.
 *
 * IMPORTANT NOTE: This function is intended to work when called
 * from a form submit handler. Calling it outside of the form submission
 * process may not work correctly.
 *
 * @param array $subscriptions
 *   Array of subscriptions nid to update.
 * @param array $updates
 *   Array of key/value pairs with node field names and the
 *   value to update that field to.
 */
function notifications_subscriptions_mass_update($subscriptions, $updates) {
  foreach ($subscriptions as $id) {
    _notifications_subscriptions_mass_update_helper($id, $updates);
  }
  drupal_set_message(t('The update has been performed.'));
}

function _notifications_subscriptions_mass_update_helper($sid, $updates) {
  $subs = notifications_load_subscription($sid);
  foreach ($updates as $name => $value) {
    $subs->$name = $value;
  }
  notifications_save_subscription($subs);
  return $subs;  
}

/**
 * Theme node administration overview.
 *
 * @ingroup themeable
 */
function theme_notifications_admin_subscriptions($form) {
  // If there are rows in this form, then $form['title'] contains a list of
  // the title form elements.
  $has_posts = isset($form['type']) && is_array($form['type']);
  $select_header = $has_posts ? theme('table_select_header_cell') : '';
  $header = array($select_header, t('Type'), t('User'), t('Send method'), t('Send interval'), t('Status'));

  $header[] = t('Operations');
  $output = '';

  $output .= drupal_render($form['options']);
  if ($has_posts) {
    foreach (element_children($form['type']) as $key) {
      $row = array();
      $row[] = drupal_render($form['subscriptions'][$key]);
      $row[] = drupal_render($form['type'][$key]);

      $row[] = drupal_render($form['username'][$key]);
      $row[] = drupal_render($form['send_method'][$key]);
      $row[] = drupal_render($form['send_interval'][$key]);       
      $row[] = drupal_render($form['status'][$key]);
      $row[] = drupal_render($form['operations'][$key]);
      $rows[] = $row;
    }

  }
  else {
    $rows[] = array(array('data' => t('No subscriptions available.'), 'colspan' => '6'));
  }

  $output .= theme('table', $header, $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}

/**
 * Theme node administration filter form.
 *
 * @ingroup themeable
 */
function theme_notifications_subscriptions_filter_form($form) {
  $output = '';
  $output .= '<div id="notifications-admin-filter">';
  $output .= drupal_render($form['filters']);
  $output .= '</div>';
  $output .= drupal_render($form);
  return $output;
}

/**
 * Generic table formatting for forms
 */
function theme_notifications_table_form($form) {
  $output = '';
  
  // Get table information from special form properties
  $index = !empty($form['#table_index']) ? $form['#table_index'] : $form['#table_fields'][0];
  $header = !empty($form['#table_header']) ? $form['#table_header'] : array();
 
  foreach (element_children($form[$index]) as $key) {
    $row = array();
    foreach ($form['#table_fields'] as $field) {
      $row[] = drupal_render($form[$field][$key]);
    }
    $rows[] = $row;
  }

  if ($rows) {
    $output .= theme('table', $header, $rows);
  } else {
    $output .= '<p>' . t('No elements') . '</p>';
  }
  $output .= drupal_render($form);
  return $output;
}