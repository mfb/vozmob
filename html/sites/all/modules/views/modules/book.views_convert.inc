<?php
// $Id: book.views_convert.inc,v 1.1 2009/04/07 23:24:55 merlinofchaos Exp $

/**
 * @file
 * Field conversion for fields handled by this module.
 */

/**
 * Implementation of hook_views_convert().
 */
function book_views_convert($display, $type, &$view, $field, $id = NULL) {
  switch ($type) {
    case 'argument':
      switch ($field['type']) {
        case 'book_parent':
          $actions = array('ignore', 'not found', 'ignore', 'summary asc', 'summary asc', 'summary desc', 'summary asc', 'empty');
          $options['default_action'] = $actions[$field['argdefault']];
          if (!empty($field['title'])) {
            $options['title'] = $field['title'];
          }
          if (!empty($field['wildcard'])) {
            $options['wildcard'] = $field['wildcard'];
          }
          if (!empty($field['wildcard_substitution'])) {
            $options['wildcard_substitution'] = $field['wildcard_substitution'];
          }
          $options['relationship'] = $view->add_item($display, 'relationship', 'book_parent', 'nid', array(), 'book_parent_nid');
          $view->add_item($display, 'argument', 'node', 'nid', $options, $id);
          break;
      }
      break;
  }
}
