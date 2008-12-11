<?php
// $Id: views_bulk_operations.views.inc,v 1.1.2.8 2008/09/21 07:13:30 kratib Exp $

function views_bulk_operations_views_plugins() {
  return array(
    'style' => array(
      'bulk' => array(
        'title' => t('Bulk Operations'),
        'help' => t('Displays rows with checkmarks for bulk actions.'),
        'handler' => 'views_bulk_operations_plugin_style',
        'parent' => 'table',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

