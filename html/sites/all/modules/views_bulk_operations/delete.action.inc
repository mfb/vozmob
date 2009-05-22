<?php
// $Id: delete.action.inc,v 1.1.2.6 2009/05/11 00:27:23 kratib Exp $

function views_bulk_operations_delete_action_info() {
  return array(
    'views_bulk_operations_delete_node_action' => array(
      'type' => 'node',
      'description' => t('Delete node'),
      'configurable' => FALSE,
      'behavior' => array('deletes_node_property'),
    ),
    'views_bulk_operations_delete_user_action' => array(
      'type' => 'user',
      'description' => t('Delete user'),
      'configurable' => FALSE,
      'behavior' => array('deletes_node_property'),
    ),
    'views_bulk_operations_delete_comment_action' => array(
      'type' => 'comment',
      'description' => t('Delete comment'),
      'configurable' => FALSE,
      'behavior' => array('deletes_node_property'),
    ),
  );
}

function views_bulk_operations_delete_node_action($node) {
  node_delete($node->nid);
}

function views_bulk_operations_delete_user_action($user) {
  user_delete(null, $user->uid);
}

function views_bulk_operations_delete_comment_action($comment) {
  require_once(drupal_get_path('module', 'comment') .'/comment.admin.inc');
  _comment_delete_thread($comment);
  _comment_update_node_statistics($comment->nid);
}

