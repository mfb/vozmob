<?php

// Include the definition of blueprint_settings() and blueprint_theme_get_default_settings().
include_once './' . drupal_get_path('theme', 'blueprint') . '/theme-settings.php';


/**
 * Implementation of THEMEHOOK_settings() function.
 *
 * @param $saved_settings
 *   An array of saved settings for this theme.
 * @return
 *   A form array.
 */
function BPSTARTER_settings($saved_settings) {

  // Get the default values from the .info file.
  $defaults = blueprint_theme_get_default_settings('BPSTARTER');

  // Merge the saved variables and their default values.
  $settings = array_merge($defaults, $saved_settings);

  /*
   * Create the form using Forms API: http://api.drupal.org/api/6
   */
  $form = array();
  /* -- Delete this line if you want to use this setting
  $form['BPSTARTER_example'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Use this sample setting'),
    '#default_value' => $settings['BPSTARTER_example'],
    '#description'   => t("This option doesn't do anything; it's just an example."),
  );
  // */

  // Add the base theme's settings.
  $form += blueprint_settings($saved_settings, $defaults);

  // Return the form
  return $form;
}
