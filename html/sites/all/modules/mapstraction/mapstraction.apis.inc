<?php
// $Id: mapstraction.apis.inc,v 1.1.2.6 2009/03/11 01:51:56 diggersf Exp $

/**
 * @file
 * Contains APIs specific callbacks for settings forms and rendering.
 */

/**
 * A basic settings form that provides an API Key field.
 */
function mapstraction_basic_settings_form($settings, $api_id) {
  $apis = mapstraction_apis(TRUE);
  $api = $apis[$api_id];
  $form = array();
  $form['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('@api_title API Key', array('@api_title' => $api['title'])),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => $settings['api_key'],
    '#process' => array('views_process_dependency'),
    '#dependency' => array(
      'edit-style-options-api' => array($api_id),
    ),
  );
  
  return $form;
}

/**
 * A basic rendering callback that imports a JavaScript file using an API key.
 */
function mapstraction_basic_render($settings, $api_id) {
  $apis = mapstraction_apis(TRUE);
  $api = $apis[$api_id];
  
  // Find %key flag in 'src'
  $src = strtr($api['src'], array('%key' => $settings['api_key']));
  return '<script type="text/javascript" src="' . $src . '"></script>';
}

function mapstraction_yahoo_settings_form($settings) {
  $form = array();
  $form['appid'] = array(
    '#type' => 'textfield',
    '#title' => t('Yahoo Maps App ID'),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => $settings['appid'],
    '#process' => array('views_process_dependency'),
    '#dependency' => array(
      'edit-style-options-api' => array('yahoo'),
    ),
  );
  
  return $form;
}

function mapstraction_yahoo_render($settings) {
  $src = 'http://api.maps.yahoo.com/ajaxymap?v=3.0&appid=' . $settings['appid'];
  return '<script type="text/javascript" src="' . $src . '"></script>';
}

function mapstraction_microsoft_render() {
  $src = 'http://dev.virtualearth.net/mapcontrol/mapcontrol.ashx?v=6';
  return '<script type="text/javascript" src="' . $src . '"></script>';
}

function mapstraction_freeearth_render() {
  $src = 'http://freeearth.poly9.com/api.js';
  return '<script type="text/javascript" src="' . $src . '"></script>';
}

function mapstraction_cloudmade_settings_form($settings) {
  $form = array();
  $form['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('CloudMade API Key'),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => $settings['api_key'],
    '#process' => array('views_process_dependency'),
    '#dependency' => array(
      'edit-style-options-api' => array('cloudmade'),
    ),
  );
  
  $form['style_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Map Style ID'),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => $settings['style_id'],
    '#process' => array('views_process_dependency'),
    '#dependency' => array(
      'edit-style-options-api' => array('cloudmade'),
    ),
  );
  
  return $form;
}

function mapstraction_cloudmade_render($settings, $api_id, $id) {
  $output = mapstraction_openlayers_render($settings);
  $src = drupal_get_path('module', 'mapstraction') . '/cloudmade.js';
  $output .= "<script type='text/javascript' src='$src'></script>\n";
  $js = '
      $(document).ready(function() {
        var cloudmade = new OpenLayers.Layer.CloudMade("CloudMade", {
          key: "' . $settings['api_key'] . '",
          styleId: ' . $settings['style_id'] . ',
        });
        Drupal.mapstraction["' . $id . '"].maps["openlayers"].addLayer(cloudmade);
      });';

  drupal_add_js($js, 'inline', 'footer');
  
  return $output;
}

function mapstraction_openlayers_render($settings) {
  $src = 'http://openlayers.org/api/OpenLayers.js';
  return "<script type='text/javascript' src='$src'></script>\n";
}