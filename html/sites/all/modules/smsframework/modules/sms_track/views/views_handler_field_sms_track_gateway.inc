<?php
// $Id: views_handler_field_sms_track_gateway.inc,v 1.1.4.2 2010/11/18 11:06:15 univate Exp $
/**
 * @file
 * SMS Framework Message Tracking feature module: views
 *
 * @package sms
 * @subpackage sms_track
 */


/**
 * Implement hook_handler_field()
 */
class views_handler_field_sms_track_gateway extends views_handler_field {
  function render($values) {
    $options_z = $values->{$this->field_alias};
    if (!empty($options_z)) {
      $options = unserialize($options_z);
      if (is_array($options) && array_key_exists('gateway', $options)) {
        return $options['gateway'];
      }
      else {
        return t('n/a');
      }
    }
  }
}
