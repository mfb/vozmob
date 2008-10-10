<?php
// $Id: content_handler_argument_many_to_one.inc,v 1.1.2.2 2008/10/04 13:14:21 karens Exp $

/**
 * @file
 * Handler for 'content_handler_argument_many_to_one' style.
 */
class content_handler_argument_many_to_one extends views_handler_argument_many_to_one {
  var $content_field;

  function construct() {
    parent::construct();

    $this->content_field = content_fields($this->definition['content_field_name']);
    $this->additional_fields = $this->definition['additional fields'];
  }

  function summary_name($data) {
    $options = $this->allowed_values();
    $value = $data->{$this->name_alias};
    if (isset($options[$value])) {
      $value = check_plain($options[$value]);
    }
    else {
      $value = parent::summary_name($data);
    }

    return $value;
  }

  function allowed_values() {
    $field = $this->content_field;
    $function = $field['module'] .'_allowed_values';
    $options = function_exists($function) ? $function($field) : content_allowed_values($field);
    return (array) $options;
  }
}