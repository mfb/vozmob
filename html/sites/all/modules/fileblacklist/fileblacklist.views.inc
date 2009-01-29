<?php
// $Id$

/**
 * @file
 * Provide views data and handlers for fileblacklist table.
 */

/**
 * Implementation of hook_views_data()
 */
function fileblacklist_views_data() {
  $data['fileblacklist']['table']['group']  = t('File blacklist');
  $data['fileblacklist']['table']['join'] = array(
    'files' => array(
      'left_field' => 'fid',
      'field' => 'fid',
    ),
  );
  $data['fileblacklist']['sha256'] = array(
    'title' => t('Blacklist'),
    'help' => t('Whether or not the file is blacklisted.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Blacklist'),
      'type' => 'yes-no',
    ),
  );
  return $data;
}
