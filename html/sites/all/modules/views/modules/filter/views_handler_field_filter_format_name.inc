<?php
// $Id: views_handler_field_filter_format_name.inc,v 1.1.2.2 2011/01/10 23:14:26 merlinofchaos Exp $
/**
 * Field handler to output the name of an input format.
 */
class views_handler_field_filter_format_name extends views_handler_field {
  function construct() {
    parent::construct();
    // Be explicit about the table we are using.
    $this->additional_fields['name'] = array('table' => 'filter_formats', 'field' => 'name');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function get_value($values) {
    return $values->{$this->aliases['name']};
  }

  function render($values) {
    $format_name = $this->get_value($values);
    if (!$format_name) {
      // Default or invalid input format.
      // filter_formats() will reliably return the default format even if the
      // current user is unprivileged.
      $format = filter_formats(variable_get('filter_default_format', 1));
      return $this->sanitize_value($format->name);
    }
    return $this->sanitize_value($format_name);
  }
}
