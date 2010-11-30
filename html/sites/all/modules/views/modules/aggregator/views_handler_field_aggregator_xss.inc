<?php
// $Id: views_handler_field_aggregator_xss.inc,v 1.1.4.2 2010/11/16 00:18:36 merlinofchaos Exp $
/**
 * @file
 *   Filters htmls tags from item.
 */

class views_handler_field_aggregator_xss extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    return aggregator_filter_xss($value);
  }

}