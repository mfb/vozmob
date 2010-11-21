<?php
// $Id: sms_valid.admin.inc,v 1.1.4.2 2010/11/18 06:14:34 univate Exp $
/**
 * @file
 * SMS Framework core module: Admin settings form functions
 *
 * @package sms
 * @subpackage sms_valid
 */ 


/**
 * Validation settings form
 *
 * @param $prefix
 *   Default country code. This should not be used.
 *
 * @ingroup forms
 */
function sms_valid_admin_settings_form(&$form_state, $prefix = 64) {
  $use_rulesets       = variable_get('sms_valid_use_rulesets',       FALSE);
  $use_global_ruleset = variable_get('sms_valid_use_global_ruleset', FALSE);

  // Mode selector
  if ($use_rulesets) {
    $mode = ($use_global_ruleset) ? 2 : 1;
  }
  else {
    $mode = 0;
  }
  $form['mode'] = array(
    '#type'  => 'radios',
    '#title' => 'Number validation',
    '#default_value' => $mode,
    '#options' => array(
        0 => 'No rulesets. Only use validation hooks implemented by gateway or other modules. [default]',
        1 => 'Use prefix-based validation rulesets.',
        2 => 'Use one ruleset for all numbers.',
    ),
    '#description' => t('Note that this will revert to the default option when the SMS Validation module is disabled.'),
  );

  // Global ruleset section
  $form['global'] = array(
    '#type'  => 'fieldset',
    '#title' => 'Global ruleset selection',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['global']['note'] = array(
    '#type'  => 'item',
    '#value' => 'Identifies the ruleset that will be used for all numbers if the "Use one ruleset" option is selected.',
  );

  $form['global']['global_ruleset'] = array(
    '#type'    => 'select',
    '#title'   => t('Ruleset to use as the global ruleset'),
    '#options' => sms_valid_get_rulesets_for_form(),
    '#default_value' => variable_get('sms_valid_global_ruleset', $prefix),
  );

  // Local number ruleset section
  $form['local'] = array(
    '#type'  => 'fieldset',
    '#title' => 'Local number detection and handling',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['local']['note'] = array(
    '#type'  => 'item',
    '#value' => 'You can this this to tell the number validation function that any number with this prefix should be considered a local number. The prefix will be stripped away and the number will be validated against the selected ruleset.',
  );

  $form['local']['local_number_prefix'] = array(
    '#type'  => 'textfield',
    '#title' => t('Prefix that identifies a local number'),
    '#size'          => 8,
    '#maxlength'     => 20,
    '#default_value' => variable_get('sms_valid_local_number_prefix', 0),
    '#description'   => 'Set to blank to disable local number identification.',
    '#disabled' => ($use_global_ruleset) ? TRUE : FALSE,
  );

  $form['local']['local_number_ruleset'] = array(
    '#type' => 'select',
    '#title' => t('Default ruleset to try for local numbers'),
    '#options' => sms_valid_get_rulesets_for_form(),
    '#default_value' => variable_get('sms_valid_local_number_ruleset', $prefix),
    '#description' => 'This identifies the default ruleset that will be used for local numbers.',
    '#disabled' => ($use_global_ruleset) ? TRUE : FALSE,
  );

  // Last resort ruleset section
  $form['last'] = array(
    '#type'  => 'fieldset',
    '#title' => 'Last resort for undetected ruleset prefixes',
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE,
  );

  $form['last']['note'] = array(
    '#type'  => 'item',
    '#value' => 'If the number validation function cannot find a ruleset to use (ie. it cannot find a prefix match) you can tell it to try a last resort ruleset.<br /><strong>WARNING!</strong> Please be very careful when using this option to ensure that you do not have unexpected behavior in your rulesets.',
  );

  $form['last']['last_resort_enabled'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Use last resort ruleset'),
    '#default_value' => variable_get('sms_valid_last_resort_enabled', FALSE),
    '#disabled' => ($use_global_ruleset) ? TRUE : FALSE,
  );

  $form['last']['last_resort_ruleset'] = array(
    '#type' => 'select',
    '#title' => t('Ruleset to try if the ruleset prefix cannot be identified from the number'),
    '#options' => sms_valid_get_rulesets_for_form(),
    '#default_value' => variable_get('sms_valid_last_resort_ruleset', $prefix),
    '#description' => 'This only works if you have selected the checkbox above.',
    '#disabled' => ($use_global_ruleset) ? TRUE : FALSE,
  );

  $form['defaults_save'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );

  return $form;
}


/**
 * @see sms_valid_admin_settings_form()
 */
function sms_valid_admin_settings_form_submit(&$form, $form_state) {
  $mode                 = $form_state['values']['mode'];
  $use_rulesets         = ($mode) ? TRUE : FALSE;
  $use_global_ruleset   = ($mode == 2) ? TRUE : FALSE;

  $global_ruleset       = $form_state['values']['global_ruleset'];
  $local_number_prefix  = $form_state['values']['local_number_prefix'];
  $local_number_ruleset = $form_state['values']['local_number_ruleset'];
  $last_resort_enabled  = $form_state['values']['last_resort_enabled'];
  $last_resort_ruleset  = $form_state['values']['last_resort_ruleset'];

  variable_set('sms_valid_use_rulesets',         $use_rulesets);
  variable_set('sms_valid_use_global_ruleset',   $use_global_ruleset);
  variable_set('sms_valid_global_ruleset',       $global_ruleset);
  variable_set('sms_valid_local_number_prefix',  $local_number_prefix);
  variable_set('sms_valid_local_number_ruleset', $local_number_ruleset);
  variable_set('sms_valid_last_resort_enabled',  $last_resort_enabled);
  variable_set('sms_valid_last_resort_ruleset',  $last_resort_ruleset);

  drupal_set_message('Settings saved.');
}


/**
 * Validation rulesets list form
 *
 * @ingroup forms
 */
function sms_valid_admin_rulesets_form(&$form_state) {
  $rulesets = sms_valid_get_all_rulesets();
  
  $form['note'] = array(
    '#type'  => 'item',
    '#value' => t('A ruleset is a number prefix with a set of deeper number prefixes, each with an allow/deny directive. For example, a ruleset prefix "64" and a rule like "21+" would allow a number like "64-21-123-4567". You can choose to have one big ruleset or you can split them into manageable rulesets by country, category, or whatever you decide.'),
  );

  foreach ($rulesets as $r) {
    $prefix    = $r['prefix'];
    $qty_rules = count($r['rules']);
    $rule_edit = '  ('. l('edit', "admin/smsframework/validation/ruleset/$prefix") .')';

    $form[$prefix]['prefix'] = array(
      '#type'  => 'textfield',
      '#size'  => 5,
      '#maxlength' => 5,
      '#disabled'  => TRUE,
      '#value' => $r['prefix']);
    $form[$prefix]['name']   = array('#value' => $r['name']);
    $form[$prefix]['iso2']   = array('#value' => $r['iso2']);
    $form[$prefix]['qty_rules'] = array('#value' => $qty_rules . $rule_edit);
    $form[$prefix][$prefix.'_out'] = array(
      '#type'  => 'checkbox',
      '#title' => 'Outbound',
      '#default_value' => sms_valid_ruleset_is_enabled($prefix, SMS_DIR_OUT),
    );
    $form[$prefix][$prefix.'_in']  = array(
      '#type'  => 'checkbox',
      '#title' => 'Inbound',
      '#default_value' => sms_valid_ruleset_is_enabled($prefix, SMS_DIR_IN),
    );
    $form[$prefix][$prefix.'_delete']  = array(
      '#type'  => 'checkbox',
      '#title' => 'Delete',
      '#default_value' => FALSE,
    );
  }

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save Changes'),
  );

  return $form;
}


/**
 * @see sms_valid_admin_rulesets_form()
 */
function theme_sms_valid_admin_rulesets_form($form) {
  $rows = array();
  foreach ($form as $prefix => $element) {
    if (isset($element['prefix']) && is_array($element['prefix'])) {
      $rows[] = array(
        drupal_render($element['prefix']),
        drupal_render($element['name']),
        drupal_render($element['iso2']),
        drupal_render($element['qty_rules']),
        drupal_render($element[$prefix.'_out']),
        drupal_render($element[$prefix.'_in']),
        drupal_render($element[$prefix.'_delete']),
      );
      unset($form[$prefix]);
    }
  }

  $output .= drupal_render($form['note']);
  unset($form['note']);

  $header = array(t('Prefix'), t('Name'), t('Country'), t('Qty Rules'), array('data' => t('Allow messages'), 'colspan' => 2), t('Delete'));
  $output .= theme('table', $header, $rows);
  $output .= drupal_render($form);

  return $output;
}


/**
 * @see sms_valid_admin_rulesets_form()
 */
function sms_valid_admin_rulesets_form_submit($form, &$form_state) {
  foreach ($form_state['values'] as $key => $element) {
    // Nasty hack to get these values
    $items = explode('_', $key);
    if (count($items) == 2) {
      // Just run once for each prefix
      if ($items[1] == 'out') {
        $prefix = $items[0];

        $out = $form_state['values'][$prefix.'_out'];
        $in  = $form_state['values'][$prefix.'_in'];
        sms_valid_ruleset_set_status($prefix, sms_dir($out, $in));

        // Handle deletes
        $delete = $form_state['values'][$prefix.'_delete'];
        if ($delete) {
          sms_valid_delete_ruleset($prefix);
        }
      }
    }
  }
  drupal_set_message(t('Rulesets saved.'));
  //$form_state['redirect'] = 'admin/smsframework/validation/countrycodes';
}


/**
 * Validation ruleset editing form
 *
 * @param $prefix
 *   Prefix for ruleset that will be displayed on page load.
 *
 * @ingroup forms
 */
function sms_valid_admin_ruleset_form(&$form_state, $prefix = NULL) {
  if ($prefix === NULL) {
    $ruleset = FALSE;
  }
  else {
    $ruleset = sms_valid_get_ruleset($prefix);
  }

  // Ruleset selection area
  $form['title'] = array(
    '#type' => 'item',
    '#value' => t('Select a ruleset from the drop-down box and click Refresh to update the ruleset form below'),
  );

  $form['select_prefix'] = array(
    '#type' => 'select',
    '#options' => sms_valid_get_rulesets_for_form(),
    '#default_value' => $prefix,
  );

  $form['select'] = array(
    '#type' => 'submit',
    '#value' => t('Refresh Editor (below)'),
    '#submit' => array('sms_valid_admin_ruleset_form_select'),
  );

  // Ruleset editor area
  $form['ruleset'] = array(
    '#type'  => 'fieldset',
    '#title' => 'Ruleset',
  );

  // If this is a new ruleset then this should be a textfield
  $form['ruleset']['prefix'] = array(
    '#type'      => 'textfield',
    '#title'     => t('Prefix'),
    '#size'      => 5,
    '#maxlength' => 5,
    '#value'     => ($ruleset) ? $ruleset['prefix'] : '',
    '#description' => 'Should be 4 digits or less. Highest allowed prefix is 65535.',
  );

  $form['ruleset']['name'] = array(
    '#type'      => 'textfield',
    '#title'     => t('Name'),
    '#size'      => 80,
    '#maxlength' => 200,
    '#default_value' => ($ruleset) ? $ruleset['name'] : '',
  );

  // Display a proper country list if the countries_api module is loaded
  if (function_exists('countries_api_get_array')) {
    $options[''] = '(none)';
    $options = array_merge($options, countries_api_get_array());
    $form['ruleset']['iso2'] = array(
      '#type'    => 'select',
      '#title'   => t('Associated country (optional)'),
      '#options' => $options,
      '#default_value' => ($ruleset) ? $ruleset['iso2']: '',
    );
  }
  else {
    $form['ruleset']['iso2'] = array(
      '#type'      => 'textfield',
      '#title'     => t('Country code (ISO 3166-1 alpha-2) (optional)'),
      '#size'      => 2,
      '#maxlength' => 2,
      '#default_value' => ($ruleset) ? $ruleset['iso2'] : '',
      '#description' => 'This would be a nice drop-down box if you had the Countries API module enabled.',
    );
  }

  $form['ruleset']['out'] = array(
    '#type'  => 'checkbox',
    '#title' => 'Allow outbound communication',
    '#default_value' => ($ruleset) ? sms_valid_ruleset_is_enabled($prefix, SMS_DIR_OUT) : 0,
  );

  $form['ruleset']['in']  = array(
    '#type'  => 'checkbox',
    '#title' => 'Allow inbound commmunication',
    '#default_value' => ($ruleset) ? sms_valid_ruleset_is_enabled($prefix, SMS_DIR_IN) : 0,
  );

  $form['ruleset']['rules'] = array(
    '#type'  => 'textarea',
    '#title' => 'Rules',
    '#cols'  => 80,
    '#rows'  => 15,
    '#default_value' => ($ruleset) ? sms_valid_rules_to_text($ruleset['rules']) : '',
    '#description' => t('One rule per line. Enter a number prefix (any length), not including the ruleset prefix.<br />Any prefix with a "-" at the end signifies an expicit deny.<br />Any prefix with a "+" at the end signifies an explicit allow.<br />All other rules are ignored.<br />Default is to deny any numbers that do not match.<br />Comments must be prefixed with a hash (#). You may place comments in-line only.<br />See the guide at %url', array('%url' => 'http://moo0.net/smsframework/?q=node/19')),
  );

  $form['ruleset']['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save Ruleset'),
  );

  return $form;
}


/**
 * @see sms_valid_admin_ruleset_form()
 */
function theme_sms_valid_admin_ruleset_form($form) {
  $header = array();
  $rows   = array();
  $rows[] = array(
    array('data' => '<strong>'. drupal_render($form['title']) .'</strong>',
          'colspan' => 3),
  );
  $rows[] = array(
    drupal_render($form['select_prefix']),
    drupal_render($form['select']),
    array('data' => '', 'width' => '80%'),
  );

  unset($form['title']);
  unset($form['select_prefix']);
  unset($form['select']);

  $output .= theme('table', $header, $rows);
  $output .= drupal_render($form);

  return $output;
}


/**
 * @see sms_valid_admin_ruleset_form()
 */
function sms_valid_admin_ruleset_form_select(&$form, $form_state) {
  $prefix = $form_state['values']['select_prefix'];
  $form['#redirect'] = "admin/smsframework/validation/ruleset/$prefix";
}


/**
 * @see sms_valid_admin_ruleset_form()
 */
function sms_valid_admin_ruleset_form_submit(&$form, $form_state) {
  $ruleset['prefix'] = $form_state['clicked_button']['#post']['prefix'];
  $ruleset['name']   = $form_state['values']['name'];
  $ruleset['iso2']   = $form_state['values']['iso2'];

  $rules_text        = $form_state['clicked_button']['#post']['rules'];
  $ruleset['rules']  = sms_valid_text_to_rules($rules_text);

  $out = $form_state['values']['out'];
  $in  = $form_state['values']['in'];
  $ruleset['dirs_enabled'] = sms_dir($out, $in);

  sms_valid_save_ruleset($ruleset);

  drupal_set_message('Ruleset saved.');
}


/**
 * Validation number test form
 *
 * @param $last_test_validation_number
 *   Number that will appear in the text field.
 *
 * @ingroup forms
 */
function sms_valid_admin_test_form(&$form_state, $last_test_validation_number = NULL) {
  $last_number = variable_get('sms_valid_test_last_number', '');
  $test_mode   = variable_get('sms_valid_test_mode', 0);

  $form['test_mode'] = array(
    '#type' => 'radios',
    '#title' => 'Validator(s)',
    '#default_value' => $test_mode,
    '#options' => array(
        0 => 'Test against rulesets only. Verbose feedback.',
        1 => 'Test against the main validation function.',
    ),
    '#description' => 'The main validation function includes rulesets (if enabled), the active gateway module and other modules that implement number validation hooks.',
  );

  $form['number'] = array(
    '#type' => 'textfield',
    '#title' => t('Number'),
    '#size' => 20,
    '#maxlength' => 30,
    '#default_value' => $last_number,
    '#description' => 'Examples: 64211234567, 021-123-4567, 21.123.4567<br />Number will be validated using all configured settings and rulesets.',
  );

  $form['validate'] = array(
    '#type' => 'submit',
    '#value' => t('Validate Number'),
  );

  return $form;
}


/**
 * @see sms_valid_admin_test_form()
 */
function sms_valid_admin_test_form_submit(&$form, $form_state) {
  $number = $form_state['values']['number'];
  drupal_set_message('You entered '. $number);
  variable_set('sms_valid_test_last_number', $number);

  $test_mode = $form_state['values']['test_mode'];
  variable_set('sms_valid_test_mode', $test_mode);

  if (! variable_get('sms_valid_use_rulesets', FALSE)) {
    drupal_set_message('Note: Rulesets are disabled.');
  }

  switch ($test_mode) {
    case 0:  // Test only against rulesets
      $options = array('test' => TRUE);
      $result  = sms_valid_validate($number, $options);
      $pass    = $result['pass'];
      $log_msg = implode("<br />", $result['log']);
      drupal_set_message($log_msg);
      break;
    case 1:  // Test main validation function
      $error = sms_validate_number($number);
      if ($error) {
        drupal_set_message('Error message from validation function:<br />'.$error);
      }
      $pass = (! $error);
      break;
  }

  if ($pass) {
    drupal_set_message('Validation succeeded and returned number '. $number);
  } else {
    form_set_error('number', 'Validation FAILED');
  }
}
