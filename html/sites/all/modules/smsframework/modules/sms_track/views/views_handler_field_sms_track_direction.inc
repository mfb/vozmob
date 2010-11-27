<?php
// $Id: views_handler_field_sms_track_direction.inc,v 1.1.4.2 2010/11/18 11:06:15 univate Exp $
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
class views_handler_field_sms_track_direction extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    switch ($value) {
      case 0:
        return t('Out');
      case 1:
        return t('In');
    }
    //else
    return $value;
  }
}
