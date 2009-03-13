<?php
// $Id: filefield_formatter.inc,v 1.10 2009/03/12 03:35:27 quicksketch Exp $
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
 * Return whether a file should be listed when viewing the node.
 *
 * @param $file
 *   A populated FileField item.
 * @param $field
 *   A CCK field instance array.
 */
function filefield_file_listed($file, $field) {
  if ($field['list_field']) {
    return (bool)$file['list'];
  }
  return TRUE;
}

/**
 * Theme function for the 'generic' single file formatter.
 */
function theme_filefield_file($file) {
  // Views may call this function with a NULL value, return an empty string.
  if (empty($file['fid'])) {
    return '';
  }

  $path = $file['filepath'];
  $url = file_create_url($path);
  $icon = theme('filefield_icon', $file);

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  // TODO: Possibly move to until I move to the more complex format described 
  // at http://darrelopry.com/story/microformats-and-media-rfc-if-you-js-or-css
  $options = array(
    'attributes' => array(
      'type' => $file['filemime'],
      'length' => $file['filesize'],
    ),
  );

  // Use the description as the link text if available.
  if (empty($file['data']['description'])) {
    $link_text = $file['filename'];
  }
  else {
    $link_text = $file['data']['description'];
    $options['attributes']['title'] = $file['filename'];
  }

  return '<div class="filefield-file clear-block">'. $icon . l($link_text, $url, $options) .'</div>';
}
