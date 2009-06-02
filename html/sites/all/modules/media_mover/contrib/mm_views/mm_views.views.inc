<?php
// $Id: mm_views.views.inc,v 1.1.2.1 2009/05/14 20:53:39 arthuregg Exp $
/**
 * @file
 *
 * Provide views data and handlers for upload tables that are not represented by
 * their own module.
 * 
 * @NOTE this is a direct copy from the views funcions, just needed one small change
 */



/**
 * Implementation of hook_views_data_alter()
 */
function mm_views_views_data_alter(&$data) {
  $data['node']['mm_upload_fid'] = array(
    'group' => t('Media Mover'),
    'title' => t('Node attached files - file path'),
    'help' => t('All files attached to a node with upload.module.'),
    'real field' => 'vid',
    'field' => array(
      'handler' => 'views_handler_field_mm_upload_fid',
    ),
  );
}


/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function mm_views_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'mm_views') . '/upload',
    ),
    'handlers' => array(
      'views_handler_field_mm_upload_fid' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * @}
 */
