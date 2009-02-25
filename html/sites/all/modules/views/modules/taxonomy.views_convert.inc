<?php
//$Id: taxonomy.views_convert.inc,v 1.1 2009/02/23 19:05:56 merlinofchaos Exp $

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
  if ($field['tablename'] == 'term_node' || !strncmp($field['tablename'], 'term_node_', 10)) { // Avoid conflicts with fields in tables from other modules.
    switch ($type) {
      case 'field':
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
        break;
    }
  }
}
