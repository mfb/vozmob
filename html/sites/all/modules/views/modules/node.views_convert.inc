<?php
//$Id: node.views_convert.inc,v 1.5 2009/04/07 23:21:00 merlinofchaos Exp $

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
  switch ($type) {
    case 'field':
      switch ($field['tablename']) {
        case 'node':
          switch ($field['field']) {
            case 'title':
              if ($field['options'] != 'no_link') {
                $view->set_item_option($display, 'field', $id, 'link_to_node', TRUE);
              }
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
              $view->set_item_option($display, 'field', $id, 'field', "$field[field]_node");
              if (!empty($field['options'])) {
                $view->set_item_option($display, 'field', $id, 'text', $field['options']);
              }
              break;
          }
          break;
      }
      break;
    case 'filter':
      switch ($field['tablename']) {
        case 'node':
          switch ($field['field']) {
            case 'created':
              $item = $view->get_item($display, 'filter', $id);
              $item['operator'] = $field['operator'];
              $item['value']['type'] = $field['value'] == 'now' ? 'offset' : 'date';
              $item['value']['value'] = $field['value'];
              if (!empty($field['options'])) {
                $item['value']['value'] = intval($field['options']) .' seconds';
              }
              $view->set_item($display, 'filter', $id, $item);
              break;
            case 'type':
              switch ($field['operator']) {
                case 'AND':
                case 'OR':
                  $view->set_item_option($display, 'filter', $id, 'operator', 'in');
                  break;
                case 'NOR':
                  $view->set_item_option($display, 'filter', $id, 'operator', 'not in');
                  break;
              }
              $view->set_item_option($display, 'filter', $id, 'value', $field['value']);
              break;
            case 'status':
            case 'promote':
            case 'sticky':
              $view->set_item_option($display, 'filter', $id, 'value', $field['value']);
              break;
            case 'distinct':
              $view->display_handler->set_option('distinct', $field['operator'] == '=' && $field['value'] == 'distinct');
              $view->set_item($display, 'filter', $id, NULL);
              break;
          }
          break;
      }
      break;
    case 'argument':
      switch ($field['type']) {
        case 'nodetype':
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
          $view->add_item($display, 'argument', 'node', 'type', array(), $id);
          break;
        case 'nodeletter':
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
          if (!empty($field['options'])) {
            $options['glossary'] = TRUE;
            $options['limit'] = $field['options'];
          }
          $view->add_item($display, 'argument', 'node', 'title', array(), $id);
          break;
        case 'year':
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
          $view->add_item($display, 'argument', 'node', 'created_year', array(), $id);
          break;
        case 'month':
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
          $view->add_item($display, 'argument', 'node', 'created_month', array(), $id);
          break;
        case 'week':
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
          $view->add_item($display, 'argument', 'node', 'created_week', array(), $id);
          break;
        case 'monthyear':
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
          $view->add_item($display, 'argument', 'node', 'created_year_month', array(), $id);
          break;
        case 'fulldate':
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
          $view->add_item($display, 'argument', 'node', 'created_fulldate', array(), $id);
          break;
        case 'nid':
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
          if (!empty($field['options'])) {
            $options['not'] = TRUE;
          }
          $view->add_item($display, 'argument', 'node', 'nid', array(), $id);
          break;
      }
      break;
  }
}
