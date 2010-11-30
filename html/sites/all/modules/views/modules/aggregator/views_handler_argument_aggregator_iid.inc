<?php
// $Id: views_handler_argument_aggregator_iid.inc,v 1.1.2.1 2010/10/12 23:12:38 merlinofchaos Exp $

/**
 * Argument handler to accept an aggregator item id.
 */
class views_handler_argument_aggregator_iid extends views_handler_argument_numeric {
  /**
   * Override the behavior of title(). Get the title of the category.
   */
  function title_query() {
    $titles = array();
    $placeholders = implode(', ', array_fill(0, sizeof($this->value), '%d'));

    $result = db_query("SELECT i.title FROM {aggregator_item} i WHERE i.iid IN ($placeholders)", $this->value);
    while ($term = db_fetch_object($result)) {
      $titles[] = check_plain($term->title);
    }
    return $titles;
  }
}
