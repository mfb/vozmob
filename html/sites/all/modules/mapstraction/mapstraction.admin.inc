<?php

function mapstraction_admin_settings() { 
  $form['mapstraction_provider'] = array( 
    '#type' => 'radios', 
    '#title' => t('Mapstraction API'), 
    '#description' => t('Select a mapstraction provider'), 
    '#options' => array( 
      t('gmap'), 
      t('yahoo'), 
      t('Microsoft'), 
     // t('MapQuest'),  mapquest has 4 src files in the script url - we'll deal with this later
      t('OpenLayers'), 
      t('OpenStreetMap') 
    ),
    '#default_value' => variable_get('mapstraction_provider', 0) // defaults to gmap 
  ); 

  $form['#submit'] = array( 
    'mapstraction_admin_settings_submit' => array($extra_info) 
  ); 
  return system_settings_form($form); 
}

function mapstraction_admin_settings_submit($form_id, &$form_values) {
  variable_set('mapstraction_provider',$form_values['mapstraction_provider']);
}