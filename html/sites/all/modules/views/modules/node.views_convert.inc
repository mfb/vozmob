<?php
//$Id: node.views_convert.inc,v 1.3 2009/02/23 18:58:18 merlinofchaos Exp $

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
function node_views_convert($display, $type, &$view, $field, $id = NULL) {
  switch ($field['tablename']) { // Avoid conflicts with fields in tables from other modules.
    case 'node':
      switch ($type) {
        case 'field':
          switch ($field['field']) {
            case 'title':
              $item = $view->get_item($display, 'field', $id);
              if ($field['options'] != 'no_link') {
                $item['link_to_node'] = TRUE;
              }
              $view->set_item($display, 'field', $id, $item);
              break;
            case 'body':
              $item = $view->get_item($display, 'field', $id);
              $item['table'] = 'node_revisions';
              $item['field'] = $field['handler'] == 'views_handler_field_teaser' ? 'teaser' : $item['field'];
              $view->set_item($display, 'field', $id, $item);
              break;
            case 'link':
              $field['field'] = 'view';
            case 'edit':
            case 'delete':
            case 'view':
              $item = $view->get_item($display, 'field', $id);
              $item['field'] = "$field[field]_node";
              if (!empty($field['options'])) {
                $item['text'] = $field['options'];
              }
              $view->set_item($display, 'field', $id, $item);
              break;
          }
          break;
        case 'filter':
          switch ($field['field']) {
            case 'type':
              $view->display[$display]->display_options['filters'][$field['field']]['value'] = drupal_map_assoc($field['value']);
              break;
            case 'status':
            case 'promote':
            case 'sticky':
              $view->display[$display]->display_options['filters'][$field['field']]['value'] = $field['value'];
              break;
          }
          break;
      }
      break;
  }
}
