<?php
// $Id: delete.action.inc,v 1.1.2.2 2008/12/25 20:41:06 kratib Exp $

function delete_action_info() {
  return array('delete_action' => array(
    'type' => 'node',
    'description' => t('Delete node'),
  ));
}

function delete_action($node) {
  node_delete($node->nid);
}

