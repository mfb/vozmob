<?php
// $Id: views_handler_field_node_comment.inc,v 1.1.2.1 2011/01/10 23:14:26 merlinofchaos Exp $

/**
 * Display node comment status
 */
class views_handler_field_node_comment extends views_handler_field {
  function render($values) {
    $value = $this->get_value($values);
    switch ($value) {
      case COMMENT_NODE_DISABLED:
      default:
        return t('Disabled');
      case COMMENT_NODE_READ_ONLY:
        return t('Read only');
      case COMMENT_NODE_READ_WRITE:
        return t('Read/Write');
    }
  }
}
