<?php
// $Id: views_handler_field_mm_upload_fid.inc,v 1.1.2.1 2009/05/14 20:53:39 arthuregg Exp $
/**
 * Field handler to provide a list of roles.
 */
class views_handler_field_mm_upload_fid extends views_handler_field{
  
  function options_form(&$form, &$form_state) {

  }
  
  function pre_render($values) {
    $vids = array();
    $this->items = array();

    $data = array();
    foreach ($values as $result) {
      $vids[] = $result->{$this->field_alias};
    }

    if ($vids) {
      // Support "only listed files" option.
      $where = '';
      if (!empty($this->options['only_listed'])) {
        $where = " AND u.list <> 0";
      }
      $result = db_query("SELECT f.filepath, f.fid  FROM {upload} u LEFT JOIN {files} f ON f.fid = u.fid WHERE u.vid IN (" . implode(', ', $vids) . ")$where ORDER BY u.weight");
      while ($file = db_fetch_object($result)) {
        $this->items[$file->fid] = $file->filepath;
      }
    }
   // drupal_set_message('<pre>' . print_R($this->items, true) . '</pre>');
  }
  
  function render($values) {
    $field = $values->{$this->field_alias};
    return $this->items[$field];
  }

}
