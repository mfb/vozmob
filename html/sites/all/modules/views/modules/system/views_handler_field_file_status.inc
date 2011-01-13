<?php
// $Id: views_handler_field_file_status.inc,v 1.1.2.1 2011/01/10 23:14:27 merlinofchaos Exp $
/**
 * Field handler to translate a node type into its readable form.
 */
class views_handler_field_file_status extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    return _views_file_status($value);
  }
}
