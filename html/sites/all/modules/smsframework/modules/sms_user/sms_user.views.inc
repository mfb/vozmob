<?php
// $Id: sms_user.views.inc,v 1.1.2.1 2010/11/18 06:14:34 univate Exp $

/**
 * basic views
 */
function sms_user_views_data() {
  $data['sms_user']['table']['group'] = t('SMS User');
  $data['sms_user']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );
  $data['sms_user']['number'] = array(
    'title' => t('Mobile Number'),
    'help' => t('Display the users Mobile number.'),
    'field' => array(
      'handler' => 'sms_user_handler_field_number',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    )
  );
  $data['sms_user']['status'] = array(
    'title' => t('Mobile Status'),
    'help' => t('Display the status of the users Mobile number.'),
    'field' => array(
      'handler' => 'sms_user_handler_field_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'sms_user_handler_filter_status',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    )
  );
  return $data;
}

/**
 * sms_user_views_handlers
 */
function sms_user_views_handlers() {
  return array(
    'info' => array(
        'path' => drupal_get_path('module', 'sms_user') . '/includes'),
    'handlers' => array(
      'sms_user_handler_field_number' => array(
        'parent' => 'views_handler_field',
      ),
      'sms_user_handler_field_status' => array(
        'parent' => 'views_handler_field',
      ),
      'sms_user_handler_filter_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}
