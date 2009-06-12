<?php
// $Id: filebanlist.views.inc,v 1.1.2.2 2009/06/12 01:41:05 mfb Exp $

/**
 * @file
 * Provide views data and handlers for filebanlist table.
 */

/**
 * Implementation of hook_views_data()
 */
function filebanlist_views_data() {
  $data['filebanlist']['table']['group']  = t('File ban list');
  $data['filebanlist']['table']['join'] = array(
    'files' => array(
      'left_field' => 'fid',
      'field' => 'fid',
    ),
  );
  $data['filebanlist']['sha256'] = array(
    'title' => t('Ban list'),
    'help' => t('Whether or not the file is banned.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Ban list'),
      'type' => 'yes-no',
    ),
  );
  return $data;
}
