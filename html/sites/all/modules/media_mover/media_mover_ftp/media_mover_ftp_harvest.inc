<?php

// $Id: media_mover_ftp_harvest.inc,v 1.1.2.9.2.9 2009/04/18 00:06:27 arthuregg Exp $;


/**
 * Run the FTP harvest.
 * @param $configuration
 *   array, current media mover configuration data
 * @return array of files
 *
 * */
function media_mover_ftp_harvest($configuration) {
  // can we connect
  if (! $connection = media_mover_ftp_connect($configuration)) {
    return;
  }

  // extract out the connection
  extract($connection);

  // make sure we have files being handed back
  if (! count($ftp_list)) {
    return;
  }

  $del = $configuration['mm_ftp_delete_files'];
  $file_types = explode(' ' , $configuration['mm_ftp_allowed_files']);

  foreach($ftp_list as $file_item) {
    $ftp_file_path = mm_ftp_parse_file($file_item, $configuration);
    // get each file and add it on to the items
    if ($item = media_mover_ftp_harvest_ftp_file($ftp_connection, $ftp_file_path, $configuration)) {
      $items[] = $item;
    }
  }
  ftp_close($ftp_connection);
  return $items;
}


/**
 * This goes through each of the files from the ftp server and harvests individual files
 * @param object $ftp_conn this is the connection to the ftp server
 * @param $ftp_file_path
 *   string, path to the file on the FTP server
 * @param array $configuration is the configuration currently running
 * @param $test
 *   boolean, should this log or display errors
 * @return array should be just a single item
 */
function media_mover_ftp_harvest_ftp_file(&$ftp_connection, $ftp_file_path, $configuration, $test = false) {
  // Get each item and save it locally
  if ($item = media_mover_ftp_create_file($ftp_connection, $ftp_file_path, $configuration, $test)) {
    // should we delete the file after we've harvested it?
    if ($configuration['mm_ftp_delete_files']) {
      if (! ftp_delete($ftp_connection, $ftp_file_path)) {
        $message = 'Error deleting file: @file';
        $params = array('@file' => $ftp_file_path);
        // log or display errors
        if ($test) { drupal_set_message(t($message, $params)); }
        else { watchdog(MM_FTP_WATCHDOG_TYPE, t($message, $params), WATCHDOG_ERROR); }
        return FALSE;
      }
      if ($test) {drupal_set_message(t('Removed the file from the FTP server.')); }
    }
    // return the item if we have it
    if ($item) {
      return $item;
    }
  }
  return FALSE;
}


/**
 * function for saving the file from the FTP server to the local drupal directory
 * @param object $ftp_conn is the ftp connection
 * @param array $configuration is the media mover configuration
 * @param $test
 *   boolean, display or log errors
 * @return array $item is a item array
 */
function media_mover_ftp_create_file(&$ftp_connection, $ftp_file_path, $configuration, $test) {

  // does this file have a token, or are we allowed ot harvest files without tokens?
  if (! media_mover_ftp_token_extract(basename($ftp_file_path), 'token') && ! $configuration['mm_ftp_harvest_without_tokens'] ) {
    // should we delete files that do not have a token from the ftp server?
    if ($configuration['mm_ftp_delete_files_no_token']) {
      ftp_delete($ftp_connection, $ftp_file_path);
      watchdog(MM_FTP_WATCHDOG_TYPE, 'Deleted FTP file without token.', array(), WATCHDOG_NOTICE);
    }
    // now we need to return because we are not supposed to harvest this file type
    return false;
  }

  // get the file name from the harvested file
  $local_file = media_mover_ftp_token_extract(basename($ftp_file_path), 'file');

  // create the complete path
  $local_file_path = file_create_filename($local_file,  media_mover_api_dir_path() . MM_FTP_FILES_SUBDIRECTORY);

  // only harvested file types that match the requested types
  if (! media_mover_api_file_validate_extensions($local_file, $configuration['mm_ftp_allowed_files'])) {
    $message = 'FTP file harvest failed: unsupported file extension !file';
    $params = array('!file' => $local_file_path);
    // display errors
    if ($test) { drupal_set_message(t($message, $params), 'error'); }
    else { watchdog(MM_FTP_WATCHDOG_TYPE, t($message, $params)); }
    return false;
  }

  // make sure directory exists
  file_check_directory(dirname($local_file_path), true);
  
  // download the file via FTP
  if (! ftp_get($ftp_connection, $local_file_path, $ftp_file_path, FTP_BINARY)) {
    $message = 'FTP file harvest failed to download file: !file';
    $params = array('!file' => $local_file_path);
    // display errors
    if ($test) { drupal_set_message(t($message, $params), 'error'); }
    else { watchdog(MM_FTP_WATCHDOG_TYPE, t($message, $params)); }
    return false;
  }

  // @TODO we may want to lock this file down more, not sure. This is a potential
  //       security risk because we don't really know what the file is that has been
  //       downloaded. We can use the mimedetect library to help us determine what kind
  //       of file it is and remove it if we think it's dangerous.

  // types we support
  $types = array('video', 'image', 'audio', 'application/octet-stream');
  // get the mime value of the file we downloaded
  $mime = file_get_mimetype($local_file);
  foreach($types as $type) {
    if (stristr($mime, $type)) {
      // we think this is a safe file
      $legal = true;
    }
  }
  if (! $legal) {
    // log the error
    watchdog(MM_FTP_WATCHDOG_TYPE, t('Harvested an illegal MIME type: @mime for @file', array('@mime' => $mime, '@file' => $local_file_path)));
    // delete the LOCAL file
    file_delete($local_file);
    return;
  }

  // log the harvested file
  watchdog(MM_FTP_WATCHDOG_TYPE, 'FTP file harvested: '. $local_file_path);

  $item['filemime'] = $mime;
  $item['filesize'] = filesize($local_file_path);
  $item['filename'] = basename($local_file_path);
  $item['filepath'] =  $local_file_path;
  $item['harvest_file'] = $local_file_path;
  $item['node']->title = $local_file;

  // get a user id from the token
  if ($user = media_mover_ftp_token_check(media_mover_ftp_token_extract($ftp_file_path, 'token'))) {
    $item['user'] = $user;
    $item['node']->uid = $user->uid;
  }

  return $item;
}
