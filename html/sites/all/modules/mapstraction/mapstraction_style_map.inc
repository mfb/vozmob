<?php
class mapstraction_style_map extends views_plugin_style {
  function option_definition() {
    $options['dimensions'] = array('default' => '600x400');
    $this->options['controls'] = array();
    
    return $options;
  }
  
  function options_form(&$form, &$form_state) {
    $form['api'] = array(
      '#type' => 'select',
      '#title' => t('Mapping API'),
      '#description' => t('Select the Mapstraction API to use for this view.'),
      '#options' => mapstraction_apis(),
      '#default_value' => $this->options['api'],
    );
    
    foreach (mapstraction_apis(TRUE) as $key => $api) {
      module_load_include('inc', 'mapstraction', 'mapstraction.apis');
      if (function_exists($api['settings form'])) {
        $form['api_settings'][$key] = $api['settings form']($this->options['api_settings'][$key]);
      }
    }
    
    $form['dimensions'] = array(
      '#type' => 'textfield',
      '#title' => t('Dimensions'),
      '#size' => 10,
      '#maxlength' => 255,
      '#default_value' => $this->options['dimensions'],
    );

    $form['controls'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Controls'),
      '#description' => t('Select which controls should be displayed on the map.'),
      '#options' => array(
        'pan' => t('Pan'),
        'overview' => t('Overview'),
        'scale' => t('Scale'),
        'map_type' => t('Map type')),
      '#default_value' => $this->options['controls'],
    );
    
    $form['zoom_control'] = array(
      '#type' => 'select',
      '#title' => t('Zoom Control'),
      '#options' => array(
        0 => t('None'),
        'large' => t('Large'),
        'small' => t('Small'),
      ),
      '#default_value' => $this->options['controls']['zoom'],
    );
    
    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#value' => t('You need at least one field before you can configure your field settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>',
      );
    }
    else {
      $field_names[$field] = array('' => '--');
      foreach ($handlers as $field => $handler) {
        if ($label = $handler->label()) {
          $field_names[$field] = $label;
        }
        else {
          $field_names[$field] = $handler->ui_name();
        }
      }
      $field_options = array(
        'title' => t('Title'),
        'latitude' => t('Latitude'),
        'longitude' => t('Longitude'),
        'class' => t('Class'),
      );
      $form['fields'] = array(
        '#type' => 'fieldset',
        '#title' => 'Field usage',
        '#description' => t('Select the fields that contain the latitude,
                            longitude and title of each point. If selected, the
                            class field will be used to apply a class to each 
                            point. Remaining fields will be available in the 
                            hidden "content" region of the point.'),
      );
      foreach ($field_options as $k => $v) {
        $form['fields'][$k] = array(
          '#type' => 'select',
          '#title' => $v,
          '#options' => $field_names,
          '#default_value' => $this->options['fields'][$k],
          '#required' => ($k == 'class' ? FALSE : TRUE),
        );
      }
    }
  }
  
  function options_submit($form, &$form_state) {
    $form_state['values']['style_options']['controls']['zoom'] = $form_state['values']['style_options']['zoom_control'];
    unset($form_state['values']['style_options']['zoom_control']);
  }

  function map_points($rows) {
    $points = array();
    foreach ($rows as $id => $row) {
      $point = array('href' => 'node/'. $row->nid, 'nid' => $row->nid);
      foreach ($this->view->field as $key => $field) {
        if ($key == $this->options['fields']['title']) {
          $point['title'] = $field->theme($row);
        }
        elseif ($key == $this->options['fields']['latitude']) {
          $point['lat'] = $field->theme($row);
        }
        elseif ($key == $this->options['fields']['longitude']) {
          $point['lon'] = $field->theme($row);
        }
        elseif ($key == $this->options['fields']['class']) {
          $point['attributes']['class'] = $this->map_point_class($field->theme($row));
        }
        else {
          $point['content'] .= $field->theme($row);
        }
      }
      $points[] = $point;
    }
    return $points;
  }
}