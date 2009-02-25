<?php
//$Id: upload.views_convert.inc,v 1.1 2009/02/23 19:05:56 merlinofchaos Exp $

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
function upload_views_convert($display, $type, &$view, $field, $id = NULL) {
  switch ($field['tablename']) { // Avoid conflicts with fields in tables from other modules.
    case 'files':
      switch ($type) {
        case 'field':
          switch ($field['field']) {
            case 'all_files':
              $item = $view->get_item($display, 'field', $id);
              switch ($field['options']) {
                case 'link':
                  $item['link_to_file'] = TRUE;
                  break;
                case 'linkdesc':
                  $item['link_to_file'] = TRUE;
                case 'nolinkdesc':
                  $item['alter']['alter_text'] = TRUE;
                  $item['alter']['text'] = '['. $view->add_item($display, 'field', 'upload', 'description', array('exclude' => TRUE)) .']';
                  break;
              }
              if ($field['handler'] == 'views_handler_file_listed_files') {
                $item['only_listed'] = TRUE;
              }
              $view->set_item($display, 'field', $id, NULL);
              $view->add_item($display, 'field', 'node', 'upload_fid', $item, $id);
              break;
          }
          break;
      }
      break;
  }
}
