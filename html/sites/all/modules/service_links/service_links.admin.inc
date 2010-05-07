<?php
// $Id: service_links.admin.inc,v 1.1.2.6 2009/08/18 17:54:03 robloach Exp $

/**
 * @file
 * Administrative page callbacks for service links module.
 */

/**
 * Theming function for build a draggable service links table
 */
function theme_service_links_drag_table(&$form) {
  $table = array();

  foreach (element_children($form['service_links_show']) as $service_id) {
    $service = &$form['service_links_show'][$service_id]; 
    $col = array();

    $service['name'] = array('#value' => $service['#title']);
    $col[] = drupal_render($service['name']);

    $service['service'] = array('#value' => $service['#service']);
    $col[] = drupal_render($service['service']);

    $service['show'] = array('#type' => 'checkbox', 
      '#value' => $service['#default_value'],
      '#id' => $service['#id'],
      '#name' => $service['#name']);
    $col[] = drupal_render($service['show']);

    $service['weight'] = &$form['service_links_weight'][$service_id];
    $service['weight']['#attributes']['class'] = 'service-weight';
    $col[] = drupal_render($service['weight']);

    $table['weights'][] = $service['#weight'];
    $table['rows'][] = array('data' => $col, 'class' => 'draggable');
  }
  
  if (empty($table['rows'])) {
    $table['rows'][] = array(array('data' => t('No service available.'), 'colspan' => '5'));
  }
  else array_multisort($table['weights'], $table['rows']);

  $table['header'] = array(t('Service Name'), t('Service Group'), t('Show'), t('Weight'));
  drupal_add_tabledrag('service_links', 'order', 'sibling', 'service-weight');
  return theme('table', $table['header'], $table['rows'], array('id' => 'service_links')); 
}

/**
 * Menu callback administration settings for general options.
 */
function service_links_admin_settings() {

  $form['where_to_show_the_links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Where to show the service links'),
    '#description' => t('Set the node types and categories you want to display links for.'),
  );
  $form['where_to_show_the_links']['service_links_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node types'),
    '#default_value' => variable_get('service_links_node_types', array()),
    '#options' => node_get_types('names'),
  );
  if (module_exists('taxonomy')) {
    $form['where_to_show_the_links']['service_links_category_types'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => t('Categories'),
      '#default_value' => variable_get('service_links_category_types', array()),
      '#options' => _service_links_get_terms(),
    );
  }

  $form['how_to_show_the_links'] = array(
    '#type' => 'fieldset',
    '#title' => t('When and how to show the links'),
  );
  $form['how_to_show_the_links']['service_links_in_links'] = array(
    '#type' => 'select',
    '#title' => t('Service links in links'),
    '#default_value' => variable_get('service_links_in_links', 0),
    '#options' => array(0 => t('Disabled'), 1 => t('Teaser view'), 2 => t('Full-page view'), 3 => t('Teasers and full-page view')),
    '#description' => t('When to display the services in the links section.'),
  );
  $form['how_to_show_the_links']['service_links_in_node'] = array(
    '#type' => 'select',
    '#title' => t('Service links in nodes'),
    '#default_value' => variable_get('service_links_in_node', 2),
    '#options' => array(
      SERVICE_LINKS_DISABLED => t('Disabled'),
      SERVICE_LINKS_IN_TEASER => t('Teaser view'),
      SERVICE_LINKS_IN_FULL => t('Full-page view'),
      SERVICE_LINKS_IN_BOTH => t('Teasers and full-page view')),
    '#description' => t('When to display the services after the node text.'),
  );
  $form['how_to_show_the_links']['service_links_style'] = array(
    '#type' => 'select',
    '#title' => t('Service links style'),
    '#default_value' => variable_get('service_links_style', 1),
    '#options' => array(
      SERVICE_LINKS_STYLE_TEXT => t('Only Text'),
      SERVICE_LINKS_STYLE_IMAGE => t('Only Image'),
      SERVICE_LINKS_STYLE_IMAGE_AND_TEXT => t('Image and Text')
    ),
  );
  $form['how_to_show_the_links']['service_links_new_window'] = array(
    '#type' => 'select',
    '#title' => t('Open link in...'),
    '#default_value' => variable_get('service_links_new_window', 0),
    '#options' => array(0 => t('Same window'), 1 => t('New window')),
  );

  $form['short_links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Short Links'),
  );
  $form['short_links']['service_links_short_links_use'] = array(
    '#type' => 'select',
    '#title' => t('Use short links'),
    '#default_value' => variable_get('service_links_short_links_use', 0),
    '#options' => array(0 => t('Never'), 1 => t('Only when requested'), 2 => t('Always')),
  );
  if (variable_get('service_links_short_links_use', 0) > 0) {
    $form['short_links']['service_links_short_links_type'] = array(
      '#type' => 'select',
      '#title' => t('How generate short links'),
      '#default_value' => variable_get('service_links_short_links_type', 1),
      '#options' => array(1 => t('Use node/xxx alias'), 2 => t('Use TinyURL.com service'), 3 => t('Redirect only the Domain name'), 4 => t('Combo: domain redirect and node/xxx alias')),
    );
    $form['short_links']['service_links_domain_redirect'] = array(
      '#type' => 'textfield',
      '#title' => t('Domain to redirect'),
      '#description' => t('Write here the complete address without trailing slash %name', array('%name' => '(http://www.example.com)') ),
      '#default_value' => variable_get('service_links_domain_redirect', ''),
      '#size' => 40,
    );
    $form['#validate'][] = 'service_links_admin_services_validate';
  }

  if (module_exists('aggregator2')) {
    $form['aggregator2_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Aggregator2 settings'),
    );
    $form['aggregator2_settings']['service_links_agg2_link'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use link to original article aggregated by aggregator2 module'),
      '#return_value' => 1,
      '#default_value' => variable_get('service_links_agg2_link', 0),
    );
  }

  return system_settings_form($form);
}

/**
 * Check if domain address textbox is empty
 */
function service_links_admin_services_validate($form, &$form_state) {
  if (isset($form_state['values']['service_links_short_links_type']) && (int) $form_state['values']['service_links_short_links_type'] > 2) {
    if (!$form_state['values']['service_links_domain_redirect']) {
      form_set_error('service_links_domain_redirect', t('Domain redirect address is not set.'));
    }
    if (preg_match("/\/$/", $form_state['values']['service_links_domain_redirect'])) {
      form_set_error('service_links_domain_redirect', t('No trailing slash!'));
    }
  }  
}

/**
 * Menu callback administration settings for services links list.
 */
function service_links_admin_services() {
  $settings = array();

  $services = service_links_get_links(NULL, TRUE);
  $settings['show'] = variable_get('service_links_show', NULL);
  $settings['weight'] = variable_get('service_links_weight', NULL);

  $form['service_links'] = array('#theme' => 'service_links_drag_table');
  $form['service_links']['service_links_show'] = array('#tree' => TRUE);
  $form['service_links']['service_links_weight'] = array('#tree' => TRUE);

  foreach ($services as $service_id => $service) {
    $icon = isset($service['icon']) ? $service['icon'] : drupal_get_path('module', 'service_links') ."/images/$service_id.png";
    $weight = isset($settings['weight'][$service_id]) ? $settings['weight'][$service_id] : 0;

    $form['service_links']['service_links_show'][$service_id] = array (
      '#service' => ucwords(str_replace('_', ' ', $service['module'])),
      '#weight' => $weight,
      '#type' => 'checkbox',
      '#title' => theme('image', $icon). " " . t('Show %name link', array('%name' => $service['name'])),
      '#return_value' => 1,
      '#default_value' => isset($settings['show'][$service_id]) ? $settings['show'][$service_id] : 0,
    );
    $form['service_links']['service_links_weight'][$service_id] = array(
      '#type' => 'weight',
      '#delta' => 100,
      '#default_value' => $weight,
    );
  }
  return system_settings_form($form);
}
