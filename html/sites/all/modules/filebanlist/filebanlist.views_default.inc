<?php
// $Id: filebanlist.views_default.inc,v 1.1.2.2 2009/06/12 01:41:05 mfb Exp $

/**
 * @file
 * Contains default views on behalf of the filebanlist module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function filebanlist_views_default_views() {
  // The following lines aren't indented but should be...

$view = new view;
$view->name = 'file_manager';
$view->description = 'File manager';
$view->tag = '';
$view->view_php = '';
$view->base_table = 'files';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'fid' => array(
    'label' => 'File ID',
    'link_to_file' => 1,
    'exclude' => 0,
    'id' => 'fid',
    'table' => 'files',
    'field' => 'fid',
    'relationship' => 'none',
  ),
  'filemime' => array(
    'id' => 'filemime',
    'table' => 'files',
    'field' => 'filemime',
  ),
  'filename' => array(
    'label' => 'Name',
    'link_to_file' => 1,
    'exclude' => 0,
    'id' => 'filename',
    'table' => 'files',
    'field' => 'filename',
    'relationship' => 'none',
  ),
  'filesize' => array(
    'id' => 'filesize',
    'table' => 'files',
    'field' => 'filesize',
  ),
  'status' => array(
    'id' => 'status',
    'table' => 'files',
    'field' => 'status',
  ),
  'timestamp' => array(
    'label' => 'Upload date',
    'date_format' => 'small',
    'custom_date_format' => '',
    'exclude' => 0,
    'id' => 'timestamp',
    'table' => 'files',
    'field' => 'timestamp',
    'relationship' => 'none',
  ),
  'sha256' => array(
    'label' => 'Ban list',
    'type' => 'yes-no',
    'not' => 0,
    'exclude' => 0,
    'id' => 'sha256',
    'table' => 'filebanlist',
    'field' => 'sha256',
    'relationship' => 'none',
  ),
));
$handler->override_option('filters', array(
  'filepath' => array(
    'operator' => 'contains',
    'value' => '',
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'filepath_op',
      'identifier' => 'filepath',
      'label' => 'Filter by file path substring',
      'optional' => 1,
      'remember' => 0,
    ),
    'case' => 0,
    'id' => 'filepath',
    'table' => 'files',
    'field' => 'filepath',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'perm',
  'perm' => 'administer file ban list',
));
$handler->override_option('title', 'File manager');
$handler->override_option('use_ajax', TRUE);
$handler->override_option('items_per_page', 20);
$handler->override_option('use_pager', '1');
$handler->override_option('style_plugin', 'bulk');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 1,
  'order' => 'desc',
  'columns' => array(
    'fid' => 'fid',
    'filemime' => 'filemime',
    'filename' => 'filename',
    'filesize' => 'filesize',
    'status' => 'status',
    'timestamp' => 'timestamp',
    'sha256' => 'sha256',
  ),
  'info' => array(
    'fid' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'filemime' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'filename' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'filesize' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'status' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'timestamp' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'sha256' => array(
      'sortable' => 1,
      'separator' => '',
    ),
  ),
  'default' => 'fid',
  'execution_type' => '1',
  'display_type' => '0',
  'skip_confirmation' => 0,
  'display_result' => 1,
  'merge_single_action' => 1,
  'selected_operations' => array(
    'd43ffcb316c846d7446f776ecf48b44a' => 'd43ffcb316c846d7446f776ecf48b44a',
    'ebb056ea879a18f8b7fe696f1a442622' => 'ebb056ea879a18f8b7fe696f1a442622',
    'e52c7d4b8ccc8fd7b8f7a59ca8e0e013' => 0,
    'e620912b6bfc590784848cf00c05b42f' => 0,
    'ffd4dd7c59102655f73303c040b08914' => 0,
    '2094a5f797e4c4f0f5ecc0cfcdae09de' => 0,
    '97cdd5da60cce28fec0e8a22e535938c' => 0,
    '0b086d2bd7e37bf66a3b8b0b9226249a' => 0,
  ),
));
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->override_option('path', 'admin/content/file');
$handler->override_option('menu', array(
  'type' => 'normal',
  'title' => 'File manager',
  'description' => 'Manage file ban list.',
  'weight' => '0',
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
));

  $views[$view->name] = $view;
  return $views;
}
