<?php
// $Id: filefield_formatter.inc,v 1.5 2008/07/31 05:25:11 dopry Exp $
/**
 * @file
 * FileField: Defines a CCK file field type.
 *
 * Uses content.module to store the fid and field specific metadata,
 * and Drupal's {files} table to store the actual file data.
 *
 * This file contains CCK formatter related functionality.
 */

/**
 * Theme function for the 'default' filefield formatter.
 */
function theme_filefield_formatter_default($element) {
  $file = $element['#item'];
  $field = content_fields($element['#field_name']);
  $output = theme('filefield_item', $file, $field);
  return $output;
}

/**
 * Theme function for any file that is managed by filefield.
 * It doesn't really format stuff by itself but rather redirects to other
 * formatters that are telling us they want to handle the concerned file.
 *
 * This function checks if the file may be shown and returns an empty string
 * if viewing the file is not allowed for any reason. If you need to display it
 * in any case, please use theme('filefield') instead.
 */
function theme_filefield_item($file, $field) {
  if (!filefield_view_access($field['field_name'])) {
    return '<!-- you do not have access to view this file -->';
  }

  if (filefield_file_listed($file, $field)) {
    return theme('filefield_file', $file);
  }
  return '';
}

/**
 * return whether a file is set to listed taking into consideration
 * widget default value settings.
 *
 * @param $file a populated filefield item.
 * @param $field a cck field instance array.
 */
function filefield_file_listed($file, $field) {
  if ($field['force_list_default']) return (bool)$field['list_default'];
  return (bool)$file['list'];
}




/**
 * Theme function for the 'generic' single file formatter.
 */
function theme_filefield_file($file) {
  $path = $file['filepath'];
  $url = file_create_url($path);
  $icon = theme('filefield_icon', $file);
  return '<div class="filefield-file clear-block">'. $icon . l($file['filename'], $url) .'</div>';
}
