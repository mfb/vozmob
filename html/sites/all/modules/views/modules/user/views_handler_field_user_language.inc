<?php
// $Id: views_handler_field_user_language.inc,v 1.1.2.3 2011/01/10 23:14:27 merlinofchaos Exp $
/**
 * @file
 *   Views field handler for userlanguage.
 */

class views_handler_field_user_language extends views_handler_field_user {

  function render_link($data, $values) {
    $uid = $this->get_value($values, 'uid');
    if (!empty($this->options['link_to_user']) && user_access('access user profiles') && $uid) {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = 'user/' . $uid;
    }
    if (empty($data)) {
      $lang = language_default();
    }
    else {
      $lang = language_list();
      $lang = $lang[$data];
    }

    return $this->sanitize_value($lang->name);
  }

  function render($values) {
    $value = $this->get_value();
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
