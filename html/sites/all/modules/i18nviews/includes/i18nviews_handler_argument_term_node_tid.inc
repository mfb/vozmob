<?php
// $Id: i18nviews_handler_argument_term_node_tid.inc,v 1.1.4.2 2010/07/20 17:16:39 mirodietiker Exp $

/**
 * Allow taxonomy term ID(s) as argument
 */
class i18nviews_handler_argument_term_node_tid extends views_handler_argument_term_node_tid {
  function title_query() {
    $titles = array();
    $placeholders = implode(', ', array_fill(0, sizeof($this->value), '%d'));

    $result = db_query("SELECT name, tid FROM {term_data} WHERE tid IN ($placeholders)", $this->value);
    while ($term = db_fetch_object($result)) {
      $titles[] = check_plain(tt('taxonomy:term:'. $term->tid .':name', $term->name));
    }
    return $titles;
  }
}

