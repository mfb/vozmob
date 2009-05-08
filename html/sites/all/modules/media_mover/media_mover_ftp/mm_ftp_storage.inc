<?php

// $Id: mm_ftp_storage.inc,v 1.1.4.3 2009/03/05 22:37:04 arthuregg Exp $

/**
 * @file
 * This provides storage integration with FTP servers
 */


/**
 * Does the storage operation of the current file
 * @param $file
 *   array, media mover file array
 * @param $configuration
 *   array, media mover configuration data
 * @param $test
 *   boolean, testing mode
 * @return unknown_type
 */
function mm_ftp_storage(&$file, $configuration, $test = false) {
  // can we connect
  if (! $connection = media_mover_ftp_connect($configuration)) {
    return;
  }

  // extract out the connection
  extract($connection);

  // get the current file path. If we are testing, $file is a string, not an array
  if ($test) {
    $file_local_path = $file;
  }
  // not testing, get the file from the media mover file array
  else {
    $file_local_path = media_mover_api_config_current_file($file);
  }

  // open the file that is being moved
  $put_file = fopen($file_local_path, 'r');

  // build the FTP file path
  $file_ftp_path = $configuration['mm_ftp_dir'] .'/'. basename($file_local_path);

  // get a unique file name for this file
  $file_name = mm_ftp_file_check($file_ftp_path, $ftp_file_list, $configuration);

  // put this file on the ftp server
  if (! ftp_fput($ftp_connection, $file_name, $put_file, FTP_BINARY)) {
    $message = 'Unable to copy file to FTP server';
    if ($test) {
      drupal_set_message($message);
    }
    else {
      watchdog(MM_FTP_WATCHDOG_TYPE, $message, WATCHDOG_ERROR);
    }
    return false;
  }

  // close the connections
  fclose($put_file);
  ftp_close($ftp_connection);

  // what kind of file path are we returning?
  if ($configuration['mm_ftp_url'] == '<none>') {
    $file_path = 'ftp://'. $file_ftp_path;
  }
  else {
    $file_path = $configuration['mm_ftp_url'] . $file_ftp_path;
  }

  // @TODO
  // do we want to delete the files here?

  return $file_path;
}



/* ************************************************ */
/* INTERNAL FUNCTIONS                               */
/* ************************************************ */

function mm_ftp_file_check($file_path, $ftp_file_list, $configuration) {
  static $files, $i;

  // build a list of files
  if (! $files) {
    $files = array();
    // sort through all of the files found and look for the current file path
    if ($ftp_file_list) {
      foreach ($ftp_file_list as $file_item) {
        $file_item = mm_ftp_parse_file($file_item, $configuration);
        $files[$file_item['name']] = $file_item;
      }
      $i = 0;
    }
  }

  // check to see if we have this file name
  if ($files[basename($file_path)] ) {
    $info = pathinfo($file_path);
    $file_path = $info['dirname'] .'/'. $info['basename'] .'_'. $i .'.'. $info['extension'];
    // recurse till we have a unique file
    $file_path = mm_ftp_file_check($file_path, $ftp_file_list, $configuration);
  }

  return $file_path;
}