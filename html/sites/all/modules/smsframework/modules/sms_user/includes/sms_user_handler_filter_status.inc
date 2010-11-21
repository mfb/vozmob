<?php
// $Id: sms_user_handler_filter_status.inc,v 1.1.2.1 2010/11/18 06:14:34 univate Exp $

/**
* @file
* Views handler to filter sms number status.
*/

/**
 * Filter by phone validation status
 */
class sms_user_handler_filter_status extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Status');
      $this->value_options = array(
        SMS_USER_PENDING    => t('Pending'),
        SMS_USER_CONFIRMED  => t('Confirmed'),
      );
    }
  }
  // Only values from that defined ones
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    $form['value']['#type'] = 'select';
  }
}
