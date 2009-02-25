<?php
// $Id: views_bulk_operations_plugin_style.inc,v 1.1.2.22 2009/02/24 05:31:04 kratib Exp $

define('VBO_EXECUTION_DIRECT', 1);
define('VBO_EXECUTION_BATCH', 2);
define('VBO_EXECUTION_QUEUE', 3);

class views_bulk_operations_plugin_style extends views_plugin_style_table {
  function options(&$options) {
    parent::options($options);
    $options['selected_operations'] = array();
    $options['execution_type'] = VBO_EXECUTION_DIRECT;
    $options['display_type'] = 0;
    $options['skip_confirmation'] = FALSE;
    $options['display_result'] = TRUE;
    $options['merge_single_action'] = TRUE;
    $this->populate_operations($options);
  }

  function options_form(&$form, &$form_values) {
    parent::options_form($form, $form_values);
    $execution = array(
      VBO_EXECUTION_DIRECT => t('Invoke them directly'),
      VBO_EXECUTION_BATCH => t('Use Batch API'),
    );
    if (module_exists('job_queue')) {
      $execution[VBO_EXECUTION_QUEUE] = t('Use <a href="@jobqueue">Job Queue</a>', array('@jobqueue' => url('http://drupal.org/project/job_queue'))); 
    }
    $form['execution_type'] = array(
      '#type' => 'radios',
      '#title' => t('To execute operations'),
      '#default_value' => $this->options['execution_type'],
      '#options' => $execution,
    );
    $form['display_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display operations as'),
      '#default_value' => $this->options['display_type'],
      '#options' => array(
        t('Dropdown selectbox with Submit button'),
        t('Each action as a separate button'),
      ),
    );
    $form['skip_confirmation'] = array(
      '#type' => 'checkbox',
      '#title' => t('Skip confirmation step'),
      '#description' => t('Check this box to skip the confirmation page on this view and directly execute the operation.'),
      '#default_value' => $this->options['skip_confirmation'],
    );
    $form['display_result'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display processing result'),
      '#description' => t('Check this box to let Drupal display a message with the result of processing the selected objects.'),
      '#default_value' => $this->options['display_result'],
    );
    $form['merge_single_action'] = array(
      '#type' => 'checkbox',
      '#title' => t('Merge single action\'s form with node selection view'),
      '#description' => t('In case only one action is selected *and* this action is configurable, display its action form along with the node selection view.'),
      '#default_value' => $this->options['merge_single_action'],
    );
    $form['selected_operations'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Selected operations'),
      '#options' => $this->get_operations_options(), 
      '#default_value' => $this->options['selected_operations'],
    );
  }

  function options_validate(&$form, &$form_values) {
  }

  function render() {
    // We build the groups here to pass them to the node_selector function through the form.
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);
    $this->sets = $sets;

    // Append suffix to avoid clashing between multiple VBOs on same page.
    static $form_suffix;
    if (isset($form_suffix)) {
      $form_suffix++;
    }
    else {
      $form_suffix = 1;
    }
    return drupal_get_form('views_bulk_operations_form_' . $form_suffix, $this);
  }

  function get_selected_operations() {
    $selected = array();
    foreach (array_filter($this->options['selected_operations']) as $key => $operation) {
      $selected[$key] = t($this->options['all_operations'][$key]['label']);
    }
    return $selected;
  }

  function get_operation_info($key) {
    $operation = $this->options['all_operations'][$key];
    if ($operation['object'] == 'system') { // System actions: morph them into current view type
      $info = _views_bulk_operations_object_info_for_view($this->view);
      $operation['object'] = $info['type'];
    }
    return $operation;
  }

  private function get_operations_options() {
    $options = array();
    $info = _views_bulk_operations_object_info_for_view($this->view);
    if (!$info) return NULL;
    foreach ($this->options['all_operations'] as $key => $operation) {
      if ($operation['object'] == 'system') { // System actions: morph them into current view type
        $operation['object'] = $info['type'];
      }
      if ($operation['object'] == $info['type']) {
        $options[$key] = $operation['label'] .' ('. $operation['callback'] .')'; 
      }
    }
    return $options;
  }

  private function populate_operations(&$options) {
    $operations = array();

    $objects = views_bulk_operations_get_object_info();
    foreach ($objects as $object_type => $info) {
      $hook_name = $object_type .'_operations';
      foreach (module_invoke_all($hook_name) as $operation) {
        if (empty($operation['callback'])) { continue; }
        $key = md5($operation['callback'] . $operation['label']);
        $operations[$key] = array(
          'label'              => $operation['label'],
          'callback'           => $operation['callback'],
          'callback arguments' => isset($operation['callback arguments']) ? $operation['callback arguments'] : array(),
          'configurable'       => isset($operation['configurable']) ? $operation['configurable'] : false,
          'type'               => 'operation',
          'object'             => $object_type,
        );
      }
    }

    $action_operations = actions_list() + $this->get_custom_actions();
    foreach ($action_operations as $callback => $operation) {
      $key = md5($callback . $operation['description']);
      $operations[$key] = array(
        'label'              => $operation['description'],
        'callback'           => $callback,
        'callback arguments' => isset($operation['parameters']) ? $operation['parameters'] : array(),
        'configurable'       => isset($operation['configurable']) ? $operation['configurable'] : false,
        'type'               => 'action',
        'object'             => $operation['type'],
        'modifies object'    => isset($operation['behavior']) && in_array('changes_node_property', $operation['behavior']),
      );
    }

    $options['all_operations'] = $operations;
  }

  private function get_custom_actions() {
    $actions = array();
    $static_actions = actions_list();
    $result = db_query("SELECT * FROM {actions} WHERE parameters > ''");
    while ($action = db_fetch_object($result)) {
      $actions[$action->aid] = array(
        'description' => $action->description,
        'type' => $action->type,
        'configurable' => FALSE,
        'parameters' => unserialize($action->parameters),
      );
      $actions[$action->aid]['behavior'] = isset($static_actions[$action->callback]['behavior']) ? $static_actions[$action->callback]['behavior'] : null;
    }
    return $actions;
  }
}

