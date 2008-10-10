<?php

/**
 * @file
 * FileField hook_file callbacks.
 */

function filefield_file_references($file) {
  $references = 0;
  foreach(content_fields() as $field) {
    if ($field['type'] != 'file') {
      continue;
    }
    $references += field_file_references($file, $field);
  }
  return array('filefield' => $references);
}

function filefield_file_delete($file) {
  // foreach  field... remove items referencing $file.
}

