<?php

// $Id: media_mover_api_io.inc,v 1.1.2.7.2.6 2009/05/28 12:05:37 arthuregg Exp $

/**
 * @file this file contains all the media mover import and export functions
 */

/* ********************************************************** */
/* IMPORT FUNCTIONS                                           */
/* ********************************************************** */

/**
 * this function builds the drupal form that takes a configuration
 * to be imported
 * @return string html
 */
function _media_mover_api_import_config() {
  drupal_set_title(t('Import a Media Mover configuration'));
  return drupal_get_form('media_mover_api_import_config_form');
}

/**
 * builds the form for the config import
 * @return array drupal form array
 *
 */
function media_mover_api_import_config_form() {
  $form = array();
  $form['config'] = array(
    '#title' => t('Paste your configuration here.'),
    '#type' => 'textarea',
    '#rows' => '40',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}


/**
 * this validates the imported configuration
 *
 * @param string $form_id
 * @param array $form_values
 */
function media_mover_api_import_config_form_validate($form, &$form_state) {
  // evaluate imported code
  ob_start();
  eval($form_state['values']['config']);
  ob_end_clean();

  // make sure that we have the required modules
  foreach($configuration->required as $module) {
    if(! module_exists($module)) {
      form_set_error('config', t('Sorry, this media mover configuration requires the %module which is not currently enabled on your system', array('%module' => $module), 'error'));
    }
  }
}


/**
 * this does the actual import of the configuration
 *
 * @param string $form_id
 * @param array $form_values
 */
function media_mover_api_import_config_form_submit($form, &$form_state) {
  // evaluate imported code
  ob_start();
  eval($form_state['values']['config']);
  ob_end_clean();

  // create a cache id
  $id = time();

  // Store the configuration
  // this is kind of a hack, but it works well enough
  cache_set("media_mover_config_$id", $configuration, 'cache');
  drupal_set_message(t('Successfully imported Media Mover configuration: !name.', array('!name' => $configuration->name)));
  $form_state['redirect'] = 'admin/build/media_mover/add/'. $id;
}


/* ********************************************************** */
/* EXPORT FUNCTIONS                                           */
/* ********************************************************** */

/**
 * returns a list of all the configurations that can be exported
 * @return string html
 */
function _media_mover_api_export($cid) {
  // is an CID being passed in?
  if (! $cid) {
    // get list of all configs
    $configs = media_mover_api_get_configurations();
    if (! count($configs)) {
      drupal_set_message(t('Sorry, there are no active configurations in the system.'));
      return;
    }
    return drupal_get_form('media_mover_api_export_form', $configs);
  }

  // load up the CID
  $config = media_mover_api_get_configuration($cid);
  return drupal_get_form('media_mover_api_export_display', media_mover_api_export_format($config) );
}


/**
 * builds the form for the export items
 *
 * @param array $configs is array of configuration objects
 * @return array drupal form array
 */
function media_mover_api_export_form($form, $configs) {
  $form = array();
  // get each configuration
  foreach ($configs as $config) {
    $options[$config->cid] = $config->name;
  }
  $form['config'] = array(
    '#type' => 'radios',
    '#title' => t('Select the configuration to export'),
    '#options' => $options,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
  );
  return $form;
}

/**
 * takes the output from the export submit and returns the user
 * to the export display
 *
 * @param string $form_id
 * @param array $form_values
 */
function media_mover_api_export_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/build/media_mover/export/'. $form_state['values']['config'];
}


/**
 * creates the display for the export output
 *
 * @param string $output is the export code
 * @return array drupal form array
 */
function media_mover_api_export_display($form, $output) {
  $form['export_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Export code'),
    '#default_value' => $output,
    '#rows' => 20,
  );
  return $form;
}



/* ********************************************************** */
/* INTERNAL FUNCTIONS                                         */
/* ********************************************************** */

/**
 * takes a media mover configuration and builds the data
 * that will be displayed in the export
 * @ TODO fix the $configuration->settings object
 * @param object $configuration
 * @return string $html out
 */
function media_mover_api_export_format($configuration) {
  $output = array();
  // define our configuration
  $output[] = '$configuration = new stdClass;';

  // first we need to get all the required modules for this configuration
  foreach (media_mover_api_verbs() as $verb) {
    // get the list of required modules so we can make validation easier
    $required_modules[] = "'". $configuration->{$verb}->module ."'";
  }
  $output[] = "\$configuration->required = array(". implode(', ', $required_modules) .");";
  // get the configuration as a string
  $output[] = media_mover_api_export_configuration_recurse($configuration, '$configuration->');
  // return a string
  return implode("\n", $output);
}


/**
 * Walks through the configuration element and builds php version of the
 * original object. Uses recursion to find all the elements
 * @param $configuration
 *   Media Mover configuration object
 * @param $parent
 *   Name of the variable that will have data assigned to it
 * @return
 *   text php representation of the object
 */
function media_mover_api_export_configuration_recurse($configuration, $parent = null) {
  $output = array();
  foreach ($configuration as $element_name => $element_data) {
    // is this item a string or a char?
    if (is_int($element_data) || is_string($element_data)) {
      $output[] = "$parent$element_name = '$element_data';";
    }
    // array get all elements and return
    else if (is_array($element_data)) {
      $output[] = "$parent$element_name = ". media_mover_api_export_array_helper($element_data) .";";
    }
    // element is an object, we need to recurse
    else if (is_object($element_data)) {
      $output[] = media_mover_api_export_configuration_recurse($element_data, "{$parent}{$element_name}->");
    }
  }
  return implode("\n", $output);
}


/**
 * takes an array and converts it to text helper for the export function
 * @param array $data
 * @return string textual representation of array
 *
 */
function media_mover_api_export_array_helper($data, $depth = '  ') {
  if (is_array($data)) {
  	$output = " array(\n";
  	foreach ($data as $key => $value) {
  		if (is_array($data[$key])) {
  			$items[] = "$depth'$key' => ". media_mover_api_export_array_helper($value, $depth . '  ');
  		}
  		else {
  			$items[] = "$depth'$key' => '$value'";
  		}
  	}
  	$output .= implode(",\n", $items) ."\n". substr($depth, 2) . ')';
    return $output;
  }
  else {
    return "'$data'";
  }
}
