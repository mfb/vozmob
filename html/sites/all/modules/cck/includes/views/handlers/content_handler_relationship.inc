<?php
// $Id: content_handler_relationship.inc,v 1.1.2.2 2008/10/04 13:14:21 karens Exp $

/**
 * @file
 * Handles content relationships and deals properly with multiple
 * values by allowing the views administrator to select deltas.
 */
class content_handler_relationship extends views_handler_relationship {
  function option_definition() {
    $options = parent::option_definition();
    $options['delta'] = array('default' => -1);

    return $options;
  }

  /**
   * Add a delta selector if multiple is TRUE.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Only add the form gadget if the field specifies.
    if (!empty($this->definition['multiple'])) {
      $max_delta = $this->definition['multiple'];
      if ($max_delta == 1) {
        $max_delta = 10;
      }

      $options = array('-1' => t('All'));
      for ($i = 0; $i < $max_delta; $i++) {
        $options[$i] = $i + 1;
      }
      $form['delta'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $this->options['delta'],
        '#title' => t('Delta'),
        '#description' => t('The delta allows you to select which item in a multiple value field to key the relationship off of. Select "1" to use the first item, "2" for the second item, and so on. If you select "All", each item in the field will create a new row, which may appear to cause duplicates.'),
      );
    }
  }

  function ensure_my_table() {
    if (!isset($this->table_alias)) {
      $join = $this->get_join();
      if (!isset($join->extra)) {
        $join->extra = array();
      }
      if ($this->options['delta'] != -1) {
        $join->extra[] = array(
          'field' => 'delta',
          'value' => $this->options['delta'],
          'numeric' => TRUE,
        );
      }

      $this->table_alias = $this->query->add_table($this->table, $this->relationship, $join);
    }
    return $this->table_alias;
  }
}