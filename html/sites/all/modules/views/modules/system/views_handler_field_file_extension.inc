<?php
// $Id: views_handler_field_file_extension.inc,v 1.1.2.2 2011/01/10 23:14:27 merlinofchaos Exp $
/**
 * @file
 * Returns a pure file extension of the file, for example 'module'.
 */

class views_handler_field_file_extension extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    if (preg_match('/\.([^\.]+)$/', $value, $match)) {
      return $this->sanitize_value($match[1]);
    }
  }
}
