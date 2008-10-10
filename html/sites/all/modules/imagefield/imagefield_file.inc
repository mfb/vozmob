<?php

/**
 * @file
 * hook_file and imagefield file functions.
 */

/**
 * Implementation of hook_file().
 */
function imagefield_file_references($file) {
  $references = 0;
  foreach(content_fields() as $field) {
    if ($field['type'] != 'imagefield') {
      continue;
    }
    $references += field_file_references($file, $field);
  }
  return array('imagefield' => $references);
}

function imagefield_file_insert($file) {
  // create and admin thumbnail.
  if (imagefield_file_is_image($file)) {
    $info = image_get_info($file->filepath);
    if($info['width'] < 100 || $info['height'] < 100) {
      $newfile = drupal_clone($file);
      file_copy($newfile->filepath, imagefield_file_admin_thumb_path($newfile));
    }
    else {
      image_scale($file->filepath, imagefield_file_admin_thumb_path($file), 100, 100);
    }
  }
}

function imagefield_file_delete($file) {
  // delete admin thumbnail.
  if (imagefield_file_is_image($file))
    file_delete(imagefield_file_admin_thumb_path($file));
}


// test if a file is an image.
function imagefield_file_is_image($file) {
  $file = (object)$file;
  //dsm($file);
  return in_array($file->filemime, array('image/jpg', 'image/pjpeg', 'image/jpeg', 'image/png', 'image/gif'));
}

// create the path to and admin thumbnail.
function imagefield_file_admin_thumb_path($file) {
  //dsm($file);
  $file = (object)$file;
  return $file->filepath .'.thumb.jpg';
}


