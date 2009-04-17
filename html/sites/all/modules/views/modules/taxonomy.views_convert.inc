<?php
//$Id: taxonomy.views_convert.inc,v 1.2 2009/04/07 23:21:00 merlinofchaos Exp $

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
function taxonomy_views_convert($display, $type, &$view, $field, $id = NULL) {
  switch ($type) {
    case 'field':
      if ($field['tablename'] == 'term_node' || !strncmp($field['tablename'], 'term_node_', 10)) {
        switch ($field['field']) {
          case 'name':
            $item = $view->get_item($display, 'field', $id);
            $item['table'] = 'term_node';
            $item['field'] = 'tid';
            if ($field['options'] != 'nolink') {
              $item['link_to_taxonomy'] = TRUE;
            }
            if (!empty($field['vocabulary'])) {
              $item['limit'] = TRUE;
              $item['vids'] = array($field['vocabulary']);
            }
            $view->set_item($display, 'field', $id, $item);
            break;
        }
      }
      break;
    case 'filter':
      if ($field['tablename'] == 'term_node' || !strncmp($field['tablename'], 'term_node_', 10)) {
        switch ($field['field']) {
          case 'tid':
            $item = $view->get_item($display, 'filter', $id);
            $item['table'] = 'term_node';
            $item['field'] = 'tid';
            if ($vid = (integer) substr($field['tablename'], 10)) {
              $item['vid'] = $vid;
            }
            else {
              $item['limit'] = FALSE;
            }
            $item['operator'] = strtolower($field['operator']);
            $item['type'] = 'select';
            $item['value'] = $field['value'];
            $view->set_item($display, 'filter', $id, $item);
            break;
        }
      }
      break;
    case 'argument':
      switch ($field['type']) {
        case 'taxid':
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
            $options['depth'] = $field['options'];
          }
          $options['break_phrase'] = TRUE;
          $view->add_item($display, 'argument', 'node', 'term_node_tid_depth', $options, $id);
          break;
        case 'taxletter':
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
          $view->add_item($display, 'argument', 'term_data', 'name', $options, $id);
          break;
        case 'vocid':
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
          $view->add_item($display, 'argument', 'vocabulary', 'vid', $options, $id);
          break;
      }
      break;
  }
}
