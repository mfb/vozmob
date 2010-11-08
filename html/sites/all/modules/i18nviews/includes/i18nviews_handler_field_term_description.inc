<?php
// $Id: i18nviews_handler_field_term_description.inc,v 1.1.4.2 2010/07/20 17:16:39 mirodietiker Exp $

/**
 * Field handler to provide simple renderer that allows linking to a taxonomy
 * term.
 */
class i18nviews_handler_field_term_description extends views_handler_field_markup {
  function pre_render($values) {
    foreach ($values as $key => $value) {
      if (isset($value->term_data_description)) {
        $tid = (isset($value->term_data_tid)) ? $value->term_data_tid : $value->tid;
        $values[$key]->term_data_description = tt('taxonomy:term:'. $tid .':description', $value->term_data_description);
      }
    }
  }
}