<?php
// $Id: sms_track.views.inc,v 1.1.4.2 2010/11/18 11:06:15 univate Exp $
/**
 * @file
 * SMS Framework Message Tracking feature module: views
 *
 * @package sms
 * @subpackage sms_track
 */


/**
 * Implement hook_views_handlers()
 */
function sms_track_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'sms_track') . '/views',
    ),
    'handlers' => array(
      'views_handler_field_sms_track_direction' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_field_sms_track_local_number' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_field_sms_track_gateway' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}


/**
 * Implement hook_views_data()
 */
function sms_track_views_data() {
  //TODO: I have left id and options out of this until I can work out how the handlers work.
  return array(
    'sms_track' => array(
      'table' => array(
        'group' => t('SMS Archive'),
        'base' => array(
          'field' => 'id',
          'title' => t('SMS Archive'),
          'help' => t("An archive of sent and received SMS messages."),
          'weight' => 0,
        ),
      ),
      'created' => array(
        'title' => t('Timestamp'),
        'help' => t('Message creation timestamp.'),
        'field' => array(
          'handler' => 'views_handler_field_date',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_date',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort_date',
        ),
      ),
      'dir' => array(
        'title' => t('Direction'),
        'help' => t('Incoming or Outgoing.'),
        'field' => array(
          'handler' => 'views_handler_field_sms_track_direction',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'number' => array(
        'title' => t('Remote number'),
        'help' => t('Phone number of remote recipient/sender.'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'message' => array(
        'title' => t('Message'),
        'help' => t('The message body text.'),
        'field' => array(
          'handler' => 'views_handler_field',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'local_number' => array(
        'real field' => 'options',
        'title' => t('Local number'),
        'help' => t('Phone number or ID of remote recipient/sender.'),
        'field' => array(
          'handler' => 'views_handler_field_sms_track_local_number',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'gateway' => array(
        'real field' => 'options',
        'title' => t('Gateway'),
        'help' => t('SMS gateway module name.'),
        'field' => array(
          'handler' => 'views_handler_field_sms_track_gateway',
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => 'views_handler_argument',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
    ),
  );
}
