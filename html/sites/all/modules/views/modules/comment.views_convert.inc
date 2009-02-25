<?php
//$Id: comment.views_convert.inc,v 1.1 2009/02/23 19:05:56 merlinofchaos Exp $

/**
 * @file
 * Field conversion for fields handled by this module.
 */

/**
 * Implementation of hook_views_convert().
 *
 * Intervene to convert field values from the Views 1 format to the
 * Views 2 format. Intervene only if $view->add_item() won't produce
 * the right results, usually needed to set field options or values.
 */
function comment_views_convert($display, $type, &$view, $field, $id = NULL) {
  switch ($field['tablename']) { // Avoid conflicts with fields in tables from other modules.
    case 'comments':
      switch ($type) {
        case 'field':
          switch ($field['field']) {
            case 'add':
              $item = $view->get_item($display, 'field', $id);
              $item['field'] = 'replyto_comment';
              $view->set_item($display, 'field', $id, $item);
              break;
          }
          break;
      }
      break;
    case 'node_comment_statistics':
      switch ($type) {
        case 'field':
          switch ($field['field']) {
            case 'last_changed':
              $item = $view->get_item($display, 'field', $id);
              $item['field'] = 'last_updated';
              if (!empty($field['options'])) {
                $item['custom_date_format'] = $field['options'];
              }
              switch ($field['handler']) {
                case 'views_handler_field_date_small':
                  $item['date_format'] = 'small';
                  break;
                case 'views_handler_field_date':
                  $item['date_format'] = 'medium';
                  break;
                case 'views_handler_field_date_large':
                  $item['date_format'] = 'large';
                  break;
                case 'views_handler_field_date_custom':
                  $item['date_format'] = 'custom';
                  break;
                case 'views_handler_field_since':
                  $item['date_format'] = 'time ago';
                  break;
              }
              $view->set_item($display, 'field', $id, $item);
              break;
          }
          break;
      }
      break;
  }
}
