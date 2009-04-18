<?php
// $Id: notifications_custom.admin.inc,v 1.1.2.3 2009/03/18 11:28:42 jareyero Exp $


/**
 * Page callback, administer custom subscriptions
 */
function notifications_custom_admin_page($op = NULL) {
  $output = '';
  $base_path = 'admin/messaging/customsubs';
  if ($op == 'new') {
    drupal_set_title(t('Add custom subscription'));
    $subs = (object)array('csid' => 0, 'type' => 'custom', 'name' => '', 'title' => '',
      'type' => 'custom_', 'module' => 'notifications_custom',
      'event_type' => '', 'description' => '', 'weight' => 0, 'visibility' => 0, 'fields' => array());
    $output .= drupal_get_form('notifications_custom_form', $subs);    
  }
  else {
    notifications_custom_rebuild();
    if ($custom = notifications_custom_list()) {
      $header = array(t('Type'), t('Name'), t('Title'), t('Operations'));
      foreach ($custom as $subs) {
        $ops = array(
          l(t('edit'), "$base_path/csid/$subs->csid/edit"),
          l(t('fields'), "$base_path/csid/$subs->csid/fields"),
          
        );
        $rows[] = array(
          l($subs->type, "$base_path/csid/$subs->csid/edit"),
          check_plain($subs->name),
          check_plain($subs->title),
          implode(' | ', $ops),
        );
      }
      $output .= theme('table', $header, $rows);
    }
    else {
      $output .= '<p>' . t('There are no custom subscriptions defined.') . '</p>';
    }
  }
  $output .= l(t('Add new custom subscription.'), "$base_path/new");
  return $output;
}

/**
 * Edit / create custom subscriptions
 */
function notifications_custom_form($form_state, $subs) {
  $form['#subscription_type'] = $subs;
  $form['subscription'] = array('#tree' => TRUE);
  $form['subscription']['csid'] = array('#type' => 'value', '#value' => $subs->csid);
  $form['subscription']['type'] = array('#type' => 'value', '#value' => $subs->type);
  $form['subscription']['name'] = array('#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $subs->name,
    '#description' => t('Name for this subscription, only for administrative pages.'),
    '#required' => TRUE,
    '#weight' => -20,
  );
  // The type will be editable only if its a new subscription type, otherwise will be messy
  $editable = empty($subs->csid);
  $form['subscription']['newtype'] = array(
    '#title' => t('Subscription type'),
    '#type' => 'textfield',
    '#default_value' => $subs->type,
    '#required' => $editable,
    '#disabled' => !$editable,
    '#description' => t('The machine-readable name of this subscription type. This name must contain only lowercase letters, numbers, and underscores. This name must be unique and once created cannot be changed.'),
    '#weight' => -10,
  );
  $form['subscription']['title'] = array('#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $subs->title,
    '#description' => t('The title of the new subscription that will be shown to the user along with a subscribe checkbox.'),
    '#required' => TRUE,
    '#weight' => -5,
  );
  $event_types = notifications_module_information('event objects');
  $form['subscription']['event_type'] = array(
    '#title' => t('Event type'),
    '#type' => 'select',
    '#options' => $event_types,
    '#default_value' => $subs->event_type,
    '#description' => t('The type of events that will trigger this subscription.'),
  );
  if (!empty($subs->fields)) {
    foreach ($subs->fields as $data) {  
      $format = notifications_format_subscription_field($data['type'], $data['value']);
      $rows[] = array($format['name'], $format['value']);
    }
    $form['subscription']['fields'] = array('#type' => 'item', '#title' => t('Fields'), '#value' => theme('table', array(), $rows));
  }
  $form['subscription']['explanation'] = array('#type' => 'textarea',
    '#title' => t('Explanation'),
    '#default_value' => $subs->explanation,
    '#description' => t('An optional explanation to go with the subscription. The explanation will be shown to the user.'),
  );
  $form['subscription']['visibility'] = array('#type' => 'radios',
    '#title' => t('Visibility'),
    '#default_value' => $subs->visibility,
    '#options' => array(t('Hidden option, only accessible by administrators.'), t('User editable, will be shown to users.')),
  );
  $form['subscription']['register'] = array('#type' => 'checkbox',
    '#title' => t('Visible in user registration form.'),
    '#default_value' => $subs->register,
  );
  $form['subscription']['default_value'] = array('#type' => 'checkbox',
    '#title' => t('Enabled for new users.'),
    '#default_value' => $subs->default_value,
    '#description' => t('If checked this subscription will be enabled by default for new users.'),
  );
  $form['subscription']['weight'] = array('#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $subs->weight,
    '#description' => t('The weights define the order in which the form fields are shown. Lighter fields "float up".'),
  );
  $form['buttons'] = array('#type' => 'fieldset');
  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  if ($subs->module == 'notifications_custom') {
    $form['buttons']['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
  } else {
    $form['buttons']['reset'] = array('#type' => 'submit', '#value' => t('Reset to defaults'));
  }
  return $form;
}

/**
 * Validate subscription type. Just check for 'type' duplicates
 */
function notifications_custom_form_validate($form, &$form_state) {
  if (empty($form_state['values']['subscription']['csid'])) {
    if (notifications_custom_load($form_state['values']['subscription']['newtype'])) {
      form_set_error('newtype', t('Duplicate subscription type. You need to select a unique string for it.'));
    } else {
      $form_state['values']['subscription']['type'] = $form_state['values']['subscription']['newtype'];
    }
  }
}

/**
 * Form submission for custom subscriptions form
 */
function notifications_custom_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $subs = $form_state['values']['subscription'];
  
  if ($op == t('Reset to defaults')) {
    notifications_custom_delete($subs['type']);
    notifications_custom_rebuild();
  }
  elseif ($op == t('Delete')) {
    $form_state['redirect'] = 'admin/messaging/customsubs/csid/' . $subs['csid'] . '/delete';
  }
  elseif (empty($subs['csid'])) {
    drupal_write_record('notifications_custom', $subs);
    drupal_set_message(t('The subscription type has been created. Now you must add one or more fields to it.'));
    $form_state['redirect'] = 'admin/messaging/customsubs/csid/' . $subs['csid'] . '/fields';    
  }
  else {
    drupal_write_record('notifications_custom', $subs, 'csid');
    drupal_set_message(t('The subscription type has been updated.'));
  }
}

/**
 * Fields form
 */
function notifications_custom_fields_form($form_state, $subscription) {
  $form['subscription'] = array('#type' => 'value', '#value' => $subscription);
  // Now the hard part, which are the fields. Only when subscriptions is created.
  $form['fields'] = array(
    '#title' => t('Fields'),
    '#type' => 'fieldset',
    '#tree' => 'true',
    '#theme' => 'notifications_custom_fields',
  );

  // Take the values from form state (if submitted) or from the subscription itself
  if (!empty($form_state['submitted'])) {
    if (!empty($form_state['values']['fields']['type'])) {
      foreach ($form_state['values']['fields']['type'] as $key => $type) {
        if (empty($form_state['values']['fields']['delete'][$key])) {
          // Add field to the list and mark as formatted so we can use this value for the form
          $fields[] = array('type' => $type, 'value' => $form_state['values']['fields']['value'][$key], 'parsed' => TRUE);  
        }
      }
    }
    if (!empty($form_state['values']['newfield'])) {
      $fields[] = array('type' => $form_state['values']['newfield'], 'value' => '');
    }
  } else {
    $fields = $subscription->fields;
  }

  // Build the form with current fields
  if ($fields) {
    foreach ($fields as $fid => $data) {
      $form['fields']['type'][$fid] = array(
        '#type' => 'hidden', '#value' => $data['type'],
      ); 
      $form['fields']['name'][$fid] = array(
        '#value' => notifications_subscription_fields($data['type'], 'name'),
      );      
      $form['fields']['delete'][$fid] = array(
        '#type' => 'checkbox',
        '#default_value' => 0,
      );
      // Generate the field and pass the value only if not parsed yet
      if (empty($data['parsed'])) {
        $form['fields']['value'][$fid] = notifications_subscription_form_field($data['type'], $data['value']);
      }
      else {
        $form['fields']['value'][$fid] = notifications_subscription_form_field($data['type']);
        $form['fields']['value'][$fid]['#default_value'] = $data['value'];
      }
    }
  } else {
    $form['fields']['#description'] = t('You have to define at least one field for this subscription.');
  }
  $form['fields']['name']['new'] = array(
    '#type' => 'select',
    '#options' => notifications_subscription_fields(NULL, 'name'),
  );
  $form['fields']['delete']['new'] = array('#value' => t('new'));
  $form['fields']['value']['new'] = array(
    '#type' => 'submit',
    '#value' => t('Add new field'),
    '#submit' => array('notifications_custom_fields_form_add_field'),
  );
  $form['submit'] = array('#type' => 'submit',
    '#value' => t('Save fields'),
  );
  return $form; 
}


/**
 * Submit callback for adding new fields
 */
function notifications_custom_fields_form_add_field(&$form, &$form_state) {
  $form_state['values']['newfield'] = $form_state['values']['fields']['name']['new'];
  $form_state['rebuild'] = TRUE;
}

/**
 * Validate field values
 */
function notifications_custom_fields_form_validate($form, &$form_state) {
  $field_values = array();
  $fields = $form_state['values']['fields'];
  if (!empty($fields['type'])) {
    foreach ($fields['type'] as $fid => $type) {
      $value = NULL;
      // Remove if checked for deletion
      if (!empty($fields['delete'][$fid])) {
        unset($form_state['values']['fields']['type'][$fid]);
        continue;
      }
      // Add if it has a value and not marked for deletion      
      elseif (!empty($fields['value'][$fid])) {
        $value = $fields['value'][$fid];       
        // We may need additional validation or field - value mappging for some fields
        if ($callback = notifications_subscription_fields($type, 'value callback')) {
          $value = call_user_func($callback, $value, "fields][value][$fid");
        }
        // If we still have a value (mapping may have failed, go and save)
        if ($value) {
          $field_values[] = array('type' => $type, 'value' => $value);
        }
      }
      if (!$value) { // We don't have a value, error message
        form_set_error("fields][value][$fid", t('You must set a value for this field.'));
      }
    }
    // Final check, we should have some valid field/value pairs
    if ($field_values) {
      $form_state['field_values'] = $field_values;
    }
    else {
      form_set_error(NULL, t('You must set at least one field for this subscription type.'));
    }    
  }

}

/**
 * Submit fields form
 */
function notifications_custom_fields_form_submit($form, $form_state) {
  $subscription = $form_state['values']['subscription'];
  $fields = $form_state['field_values'];
  db_query("UPDATE {notifications_custom} SET fields = '%s' WHERE csid = %d", serialize($fields), $subscription->csid);
  drupal_set_message(t('The fields for this subscription have been updated.'));
}

/**
 * Menu callback; delete a single subscription type.
 */
function notifications_custom_delete_confirm(&$form_state, $custom) {
  $form['type'] = array('#type' => 'value', '#value' => $custom->type);
  $form['name'] = array('#type' => 'value', '#value' => $custom->name);

  $message = t('Are you sure you want to delete the subscription type %type?', array('%type' => $custom->name));
  $caption = '';

  $num_nodes = db_result(db_query("SELECT COUNT(*) FROM {notifications} WHERE type = '%s'", $custom->type));
  if ($num_nodes) {
    $caption .= '<p>'. format_plural($num_nodes, '<strong>Warning:</strong> there is currently 1 %type subscription on your site. It may not be able to be displayed or edited correctly, once you have removed this content type.', '<strong>Warning:</strong> there are currently @count %type subscriptions on your site. They may not be able to be displayed or edited correctly, once you have removed this subscription type.', array('%type' => $custom->name)) .'</p>';
    $form['subscriptions'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete subscriptions'),
      '#description' => t('Marking this checkbox will delete also all subscription instances of this type for users of this site.'),
    );
  }

  $caption .= '<p>'. t('This action cannot be undone.') .'</p>';

  return confirm_form($form, $message, 'admin/messaging/customsubs', $caption, t('Delete'));
}

/**
 * Process subscription type delete confirm submissions.
 */
function notifications_custom_delete_confirm_submit($form, &$form_state) {
  notifications_custom_delete($form_state['values']['type'], $form_state['values']['subscriptions']);

  $t_args = array('%name' => $form_state['values']['name']);
  drupal_set_message(t('The subscription type %name has been deleted.', $t_args));
  watchdog('notifications', 'Deleted subscription type %name.', $t_args, WATCHDOG_NOTICE);

  $form_state['redirect'] = 'admin/messaging/customsubs';
  return;
}
