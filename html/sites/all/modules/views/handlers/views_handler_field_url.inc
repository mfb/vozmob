<?php
// $Id: views_handler_field_url.inc,v 1.1.2.2 2011/01/10 23:14:26 merlinofchaos Exp $

/**
 * Field handler to provide simple renderer that turns a URL into a clickable link.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_url extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['display_as_link'] = array('default' => TRUE);

    return $options;
  }

  /**
   * Provide link to the page being visited.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['display_as_link'] = array(
      '#title' => t('Display as link'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['display_as_link']),
    );
  }

  function render($values) {
    $value = $this->get_value($values);
    if (!empty($this->options['display_as_link'])) {
      return l($this->sanitize_value($value), $value, array('html' => TRUE));
    }
    else {
      return $this->sanitize_value($value);
    }
  }
}
