<?php
// $Id: views_handler_field_node_language.inc,v 1.2.2.1 2011/01/10 23:14:27 merlinofchaos Exp $

/**
 * Field handler to translate a language into its readable form.
 */
class views_handler_field_node_language extends views_handler_field_node {
  function render($values) {
    $languages = locale_language_list();
    $value = $this->get_value($values);
    $value = isset($languages[$value]) ? $languages[$value] : '';
    return $this->render_link($value, $values);
  }
}
