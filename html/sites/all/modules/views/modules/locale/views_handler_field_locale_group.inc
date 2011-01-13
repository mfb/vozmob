<?php
// $Id: views_handler_field_locale_group.inc,v 1.1.2.1 2011/01/10 23:14:26 merlinofchaos Exp $

/**
 * Field handler to translate a group into its readable form.
 */
class views_handler_field_locale_group extends views_handler_field {
  function render($values) {
    $groups = module_invoke_all('locale', 'groups');
    // Sort the list.
    asort($groups);
    $value = $this->get_value($values);
    return isset($groups[$value]) ? $groups[$value] : '';
  }
}
