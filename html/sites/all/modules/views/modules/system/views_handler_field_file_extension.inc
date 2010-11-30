<?php
// $Id: views_handler_field_file_extension.inc,v 1.1.2.1 2010/10/12 22:08:09 merlinofchaos Exp $
/**
 * @file
 * Returns a pure file extension of the file, for example 'module'.
 */

class views_handler_field_file_extension extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    if (preg_match('/\.([^\.]+)$/', $value, $match)) {
      return check_plain($match[1]);
    }
  }
}
