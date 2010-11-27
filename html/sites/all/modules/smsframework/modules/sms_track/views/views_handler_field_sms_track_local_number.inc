<?php
// $Id: views_handler_field_sms_track_local_number.inc,v 1.1.4.2 2010/11/18 11:06:15 univate Exp $
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
class views_handler_field_sms_track_local_number extends views_handler_field {
  function render($values) {
    $options_z = $values->{$this->field_alias};
    if (!empty($options_z)) {
      $options = unserialize($options_z);
      if (is_array($options)) {
        if (array_key_exists('receiver', $options)) {
          return $options['receiver'];
        }
        elseif (array_key_exists('sender', $options)) {
          return $options['sender'];
        }
      }
      return t('n/a');
    }
  }
}
