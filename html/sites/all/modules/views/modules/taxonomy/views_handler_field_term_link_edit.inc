<?php
// $Id: views_handler_field_term_link_edit.inc,v 1.1.2.2 2011/01/10 23:14:27 merlinofchaos Exp $

/**
 * Field handler to present a term edit link .
 */
class views_handler_field_term_link_edit extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['tid'] = 'tid';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    if (user_access('administer taxonomy')) {
      $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
      $tid = $this->get_value($values, 'tid');
      return l($text, 'admin/content/taxonomy/edit/term/'. $tid, array('query' => drupal_get_destination()));
    }
  }
}
