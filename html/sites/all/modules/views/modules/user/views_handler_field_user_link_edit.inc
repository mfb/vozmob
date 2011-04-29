<?php
// $Id: views_handler_field_user_link_edit.inc,v 1.1.2.3 2011/01/10 23:14:27 merlinofchaos Exp $
/**
 * Field handler to present a link to user edit.
 */
class views_handler_field_user_link_edit extends views_handler_field_user_link {
  function render_link($data, $values) {
    // Build a pseudo account object to be able to check the access.
    $account = new stdClass();
    $account->uid = $data;

    if ($data && user_edit_access($account)) {
      $this->options['alter']['make_link'] = TRUE;

      $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');

      $this->options['alter']['path'] = "user/$data/edit";
      $this->options['alter']['query'] = drupal_get_destination();

      return $text;
    }
  }
}
