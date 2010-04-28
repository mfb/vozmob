<?php
// $Id: views_handler_field_aggregator_item_description.inc,v 1.1.2.1 2010/01/05 01:16:58 merlinofchaos Exp $
/**
 * @file
 *   Filters htmls tags from item description.
 */

class views_handler_field_aggregator_item_description extends views_handler_field_xss {
  function render($values) {
    $value = $values->{$this->field_alias};
    return aggregator_filter_xss($value);
  }
}
