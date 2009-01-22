<?php
// $Id: delete.action.inc,v 1.1.2.3 2009/01/20 21:28:25 kratib Exp $

function delete_action_info() {
  return array(
    'delete_node_action' => array(
      'type' => 'node',
      'description' => t('Delete node'),
    ),
    'delete_user_action' => array(
      'type' => 'user',
      'description' => t('Delete user'),
    ),
    'delete_comment_action' => array(
      'type' => 'comment',
      'description' => t('Delete comment'),
    ),
  );
}

function delete_node_action($node) {
  node_delete($node->nid);
}

function delete_user_action($user) {
  user_delete(null, $user->uid);
}

function delete_comment_action($comment) {
  require_once(drupal_get_path('module', 'comment') .'/comment.admin.inc');
  _comment_delete_thread($comment);
  _comment_update_node_statistics($comment->nid);
}

