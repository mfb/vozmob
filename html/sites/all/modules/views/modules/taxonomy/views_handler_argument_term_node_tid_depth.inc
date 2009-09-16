<?php
// $Id: views_handler_argument_term_node_tid_depth.inc,v 1.2 2009/09/15 15:52:39 merlinofchaos Exp $
/**
 * Argument handler for taxonomy terms with depth.
 *
 * This handler is actually part of the node table and has some restrictions,
 * because it uses a subquery to find nodes with
 */
class views_handler_argument_term_node_tid_depth extends views_handler_argument {
  function option_definition() {
    $options = parent::option_definition();

    $options['depth'] = array('default' => 0);
    $options['break_phrase'] = array('default' => FALSE);
    $options['set_breadcrumb'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['depth'] = array(
      '#type' => 'weight',
      '#title' => t('Depth'),
      '#default_value' => $this->options['depth'],
      '#description' => t('The depth will match nodes tagged with terms in the hierarchy. For example, if you have the term "fruit" and a child term "apple", with a depth of 1 (or higher) then filtering for the term "fruit" will get nodes that are tagged with "apple" as well as "fruit". If negative, the reverse is true; searching for "apple" will also pick up nodes tagged with "fruit" if depth is -1 (or lower).'),
    );

    $form['break_phrase'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow multiple terms per argument'),
      '#description' => t('If selected, users can enter multiple arguments in the form of 1+2+3. Due to the number of JOINs it would require, AND will be treated as OR with this argument.'),
      '#default_value' => !empty($this->options['break_phrase']),
    );

    $form['set_breadcrumb'] = array(
      '#type' => 'checkbox',
      '#title' => t("Set the breadcrumb for the term parents"),
      '#description' => t('If selected, the breadcrumb trail will include all parent terms, each one linking to this view. Note that this only works if just one term was received.'),
      '#default_value' => !empty($this->options['set_breadcrumb']),
    );
  }

  function set_breadcrumb(&$breadcrumb) {
    if (empty($this->options['set_breadcrumb']) || !is_numeric($this->argument)) {
      return;
    }

    return views_taxonomy_set_breadcrumb($breadcrumb, $this);
  }

  /**
   * Override default_actions() to remove summary actions.
   */
  function default_actions($which = NULL) {
    if ($which) {
      if (in_array($which, array('ignore', 'not found', 'empty', 'default'))) {
        return parent::default_actions($which);
      }
      return;
    }
    $actions = parent::default_actions();
    unset($actions['summary asc']);
    unset($actions['summary desc']);
    return $actions;
  }

  function query() {
    $this->ensure_my_table();

    if (!empty($this->options['break_phrase'])) {
      $tids = new stdClass();
      $tids->value = $this->argument;
      $tids = views_break_phrase($this->argument, $tids);
      if ($tids->value == -1) {
        return FALSE;
      }

      if (count($tids->value) > 1) {
        $placeholder = " IN (" . implode(', ', array_fill(0, sizeof($tids->value), '%d')) . ")";
      }
      else {
        $placeholder = " = %d";
      }

      $tids = $tids->value;
    }
    else {
      $placeholder = "= %d";
      $tids = array($this->argument);
    }

    $node_table = $this->ensure_my_table();
    $group = 'term_node_nid_depth';
    $this->query->set_where_group('OR', $group);

    // Create a new join to relate the 'term_node' table to our current 'node' table.
    $join = new views_join;
    $join->construct('term_node', $node_table, 'vid', 'vid');
    $term_node = $this->query->add_relationship('term_node', $join, $node);
    $this->query->add_where($group, "$term_node.tid $placeholder", $tids);

    if ($this->options['depth'] > 0) {
      $th_join = new views_join;
      $th_join->construct('term_hierarchy', $term_node, 'tid', 'tid');
      $term_hierarchy = $this->query->add_relationship('term_hierarchy', $th_join, $term_node);
      foreach (range(1, abs($this->options['depth'])) as $count) {
        $th_join = new views_join;
        $th_join->construct($term_hierarchy, $term_hierarchy, 'parent', 'tid');
        $term_hierarchy = $this->query->add_relationship($term_hierarchy, $th_join, $term_hierarchy);
        $this->query->add_where($group, "$term_hierarchy.tid $placeholder", $tids);
      }
    }
    else if ($this->options['depth'] < 0) {
      foreach (range(1, abs($this->options['depth'])) as $count) {
        $th_join->construct($term_hierarchy, $term_hierarchy, 'tid', 'parent');
        $term_hierarchy = $this->query->add_relationship($term_hierarchy, $th_join, $term_hierarchy);
        $this->query->add_where($group, "$term_hierarchy.tid $placeholder", $tids);
      }
    }
  }

  function title() {
    $term = taxonomy_get_term($this->argument);
    if (!empty($term)) {
      return check_plain($term->name);
    }
    // TODO review text
    return t('No name');
  }
}
