<?php
// $Id: sms_track.views_default.inc,v 1.1.4.2 2010/11/18 11:06:15 univate Exp $
/**
 * @file
 * SMS Framework Message Tracking feature module: views
 *
 * @package sms
 * @subpackage sms_track
 */


/**
 * Implement hook_views_default_views()
 */
function sms_track_views_default_views() {
  $view = new view;
  $view->name = 'sms_track';
  $view->description = 'All archived SMS messages - in and out';
  $view->tag = 'sms';
  $view->view_php = '';
  $view->base_table = 'sms_track';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Default', 'default');
  $handler->override_option('fields', array(
    'dir' => array(
      'label' => 'Direction',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'dir',
      'table' => 'sms_track',
      'field' => 'dir',
      'relationship' => 'none',
    ),
    'number' => array(
      'id' => 'number',
      'table' => 'sms_track',
      'field' => 'number',
    ),
    'message' => array(
      'label' => 'Message',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'message',
      'table' => 'sms_track',
      'field' => 'message',
      'relationship' => 'none',
    ),
    'created' => array(
      'label' => 'Timestamp',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
      ),
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'created',
      'table' => 'sms_track',
      'field' => 'created',
      'relationship' => 'none',
    ),
    'local_number' => array(
      'label' => 'Local number',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'local_number',
      'table' => 'sms_track',
      'field' => 'local_number',
      'relationship' => 'none',
    ),
    'gateway' => array(
      'label' => 'Gateway',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'gateway',
      'table' => 'sms_track',
      'field' => 'gateway',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'role',
    'role' => array(
      '3' => 3,
    ),
  ));
  $handler->override_option('title', 'SMS Archive');
  $handler->override_option('empty', 'There are no SMS messages in the archive.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', 30);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'desc',
    'columns' => array(
      'dir' => 'dir',
      'number' => 'number',
      'message' => 'message',
      'created' => 'created',
      'local_number' => 'local_number',
      'gateway' => 'gateway',
    ),
    'info' => array(
      'dir' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'number' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'message' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'created' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'local_number' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'gateway' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'created',
  ));
  $views[$view->name] = $view;

  return $views;
}
