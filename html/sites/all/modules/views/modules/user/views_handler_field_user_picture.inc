<?php
// $Id: views_handler_field_user_picture.inc,v 1.1.2.4 2011/01/10 23:14:27 merlinofchaos Exp $

/**
 * Field handler to provide simple renderer that allows using a themed user link
 */
class views_handler_field_user_picture extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'uid';
    $this->additional_fields['name'] = 'name';
    $this->additional_fields['mail'] = 'mail';   
  }

  function element_type() {
    return 'div';
  }

  function render($values) {
    // Fake an account object.
    $account = new stdClass();
    $account->uid = $this->get_value($values, 'uid');
    $account->name = $this->get_value($values, 'name');
    $account->mail = $this->get_value($values, 'mail');
    $account->picture = $this->get_value($values);

    return theme('user_picture', $account);
  }
}
