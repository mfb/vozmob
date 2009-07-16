<?php
// $Id: mapstraction_style_map.inc,v 1.1.2.13 2009/06/11 12:40:00 dixon Exp $

/**
 * @file
 * Defines a style plugin that will display points on a map from a number of
 * different providers.
 */
class mapstraction_style_map extends views_plugin_style {
  function option_definition() {
    $options['dimensions'] = array(
      'default' => '600x400'
    );
    $options['initial_point'] = array(
      'auto' => TRUE,
      'latitude' => '',
      'longitude' => '',
      'zoom' => 10,
    );
    $options['controls'] = array();
    $options['behaviours'] = array();
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
        $form['api_settings'][$key] = $api['settings form']($this->options['api_settings'][$key], $key);
      }
    }
    
    $form['dimensions'] = array(
      '#type' => 'textfield',
      '#title' => t('Dimensions'),
      '#size' => 10,
      '#maxlength' => 255,
      '#default_value' => $this->options['dimensions'],
    );
    
    $form['initial_point'] = array(
      '#type' => 'fieldset',
      '#tree' => TRUE,
      '#title' => t('Initial point'),
    );

    $form['initial_point']['auto'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto detect initial point'),
      '#description' => t('If checked, the map will auto detect initial coordinates and zoom level based on all markers on the map. Otherwise you can specify these values by yourself.'),
      '#default_value' => $this->options['initial_point']['auto'],
    );

    $form['initial_point']['latitude'] = array(
      '#type' => 'textfield',
      '#title' => t('Latitude'),
      '#size' => 40,
      '#maxlength' => 255,
      '#process' => array('views_process_dependency'),
      '#dependency' => array(
        'edit-style-options-initial-point-auto' => array(0)
      ),
      '#default_value' => $this->options['initial_point']['latitude'],
    );

    $form['initial_point']['longitude'] = array(
      '#type' => 'textfield',
      '#title' => t('Longitude'),
      '#size' => 40,
      '#maxlength' => 255,
      '#process' => array('views_process_dependency'),
      '#dependency' => array(
        'edit-style-options-initial-point-auto' => array(0)
      ),
      '#default_value' => $this->options['initial_point']['longitude'],
    );

    $form['initial_point']['zoom'] = array(
      '#type' => 'textfield',
      '#title' => t('Zoom level'),
      '#size' => 2,
      '#maxlength' => 2,
      '#process' => array('views_process_dependency'),
      '#dependency' => array(
        'edit-style-options-initial-point-auto' => array(0)
      ),
      '#default_value' => $this->options['initial_point']['zoom'],
    );

    $form['behaviours'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Behaviours'),
      '#options' => array(
        'hover' => t('Show info bubble on mouse hover'),
      ),
      '#default_value' => $this->options['behaviours'],
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
        'icon' => t('Icon URL'),
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
  
  function validate() {
    $errors = array();
    
    // Validate the field style for location.module's lat/lon fields. They
    // must be set to decimal degress.
    $fields = $this->display->handler->get_handlers('field');
    $field_array = array($fields[$this->options['fields']['latitude']],
    $fields[$this->options['fields']['longitude']]);
    
    foreach ($field_array as $field) {
      if ($field->options['table'] == 'location' && $field->options['style'] != 'dd') {
        $errors[] = t('The display style for @field must be set to decimal degrees.', array('@field' => $field->options['label']));
      }
    }
    
    return $errors;
  }
  
  function render() {
    if ($this->view->preview == TRUE) {
      return '<div class="messages error">Preview is disabled for the Mapstraction style plugin.</div>';
    }
    else {
      // Would like to use parent::render() here but there seems to be problem
      // with the way the theme function is handled. Copying this from
      // views_plugin_style.inc for now.
      if ($this->uses_row_plugin() && empty($this->row_plugin)) {
        vpr('views_plugin_style_default: Missing row plugin');
        return;
      }

      // Group the rows according to the grouping field, if specified.
      $sets = $this->render_grouping($this->view->result, $this->options['grouping']);

      // Render each group separately and concatenate.  Plugins may override this
      // method if they wish some other way of handling grouping.
      $output = '';
      foreach ($sets as $title => $records) {
        if ($this->uses_row_plugin()) {
          $rows = array();
          foreach ($records as $label => $row) {
            $rows[] = $this->row_plugin->render($row);
          }
        }
        else {
          $rows = $records;
        }

        $output .= theme($this->theme_functions(), $this->view, $this->options, $rows, $title);
      }
      return $output;
    }
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
        elseif ($key == $this->options['fields']['icon']) {
          $point['icon'] = $field->theme($row);
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