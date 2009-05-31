<?php

function mapstraction_google_settings_form($settings) {
  $form = array();
  $form['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Maps API Key'),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => $settings['api_key'],
  );
  
  return $form;
}

function mapstraction_google_render($settings) {
  $src = 'http://maps.google.com/maps?file=api&v=2&key=' . $settings['api_key'];
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
  );
  
  return $form;
}

function mapstraction_yahoo_render($settings) {
  $src = 'http://api.maps.yahoo.com/ajaxymap?v=3.0&appid=' . $settings['appid'];
  return '<script type="text/javascript" src="' . $src . '"></script>';
}

function mapstraction_mapquest_render() {
  $src = 'http://btilelog.beta.mapquest.com/tilelog/transaction?transaction=script&key=mjtd%7Clu6t210anh%2Crn%3Do5-labwu&itk=true&v=5.3.0_RC5&ipkg=controls1';
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
  );
  
  $form['style_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Map Style ID'),
    '#size' => 40,
    '#maxlength' => 255,
    '#default_value' => $settings['style_id'],
  );
  
  return $form;
}

function mapstraction_cloudmade_render($settings, $id) {
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