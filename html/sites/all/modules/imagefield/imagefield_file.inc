<?php
// $Id: imagefield_file.inc,v 1.9 2009/03/11 01:41:57 quicksketch Exp $

/**
 * @file
 * hook_file and imagefield file functions.
 */

/**
 * Implementation of hook_file_references().
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

/**
 * Implementation of hook_file_insert().
 *
 * Create an image thumbnail to be used as a preview when editing a node.
 */
function imagefield_file_insert($file) {
  if (imagefield_file_is_image($file)) {
    $info = image_get_info($file->filepath);
    if ($info['width'] <= 100 || $info['height'] <= 100) {
      $newfile = drupal_clone($file);
      file_copy($newfile->filepath, imagefield_file_admin_thumb_path($newfile));
    }
    else {
      image_scale($file->filepath, imagefield_file_admin_thumb_path($file), 100, 100);
    }
  }
}

/**
 * Implementation of hook_file_delete().
 *
 * Delete the admin thumbnail when the original is deleted.
 */
function imagefield_file_delete($file) {
  if (imagefield_file_is_image($file)) {
    file_delete(imagefield_file_admin_thumb_path($file));
  }
}

/**
 * Simple utility function to check if a file is an image.
 */
function imagefield_file_is_image($file) {
  $file = (object)$file;
  return in_array($file->filemime, array('image/jpg', 'image/pjpeg', 'image/jpeg', 'image/png', 'image/gif'));
}

/**
 * Given a file, return the path the image thumbnail used while editing.
 */
function imagefield_file_admin_thumb_path($file) {
  $file = (object)$file;
  $extension_dot = strrpos($file->filepath, '.');
  $extension = substr($file->filepath, $extension_dot + 1);
  $basepath = substr($file->filepath, 0, $extension_dot);
  return $basepath .'.thumb.'. $extension;
}
