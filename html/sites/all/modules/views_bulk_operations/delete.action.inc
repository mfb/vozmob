<?php
// $Id: delete.action.inc,v 1.1.2.5 2009/02/17 22:16:59 kratib Exp $

function views_bulk_operations_delete_action_info() {
  return array(
    'views_bulk_operations_delete_node_action' => array(
      'type' => 'node',
      'description' => t('Delete node'),
      'configurable' => FALSE,
    ),
    'views_bulk_operations_delete_user_action' => array(
      'type' => 'user',
      'description' => t('Delete user'),
      'configurable' => FALSE,
    ),
    'views_bulk_operations_delete_comment_action' => array(
      'type' => 'comment',
      'description' => t('Delete comment'),
      'configurable' => FALSE,
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

