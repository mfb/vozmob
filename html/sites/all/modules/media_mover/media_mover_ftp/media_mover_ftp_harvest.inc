<?php

// $Id: media_mover_ftp_harvest.inc,v 1.1.2.9.2.17 2009/05/21 15:14:02 arthuregg Exp $;


/**
 * Run the FTP harvest.
 * @param $configuration
 *   array, current media mover configuration data
 * @return array of files
 *
 * */
function media_mover_ftp_harvest($configuration, $test = false) {
  // can we connect
  if (! $connection = media_mover_ftp_connect($configuration)) {
    return;
  }
  // make sure we have files being handed back
  if (! count($connection['ftp_files'])) {
    return;
  }
  
  // get the full list of files
  $files = media_mover_ftp_harvest_file_recurse($connection['ftp_connection'], $connection['ftp_files'], $directory, $configuration, $test);

  ftp_close($connection['ftp_connection']);
  return $files;
}


/**
 * 
 * @param $ftp_connection
 * @param $ftp_files
 *   array, list of ftp files
 * @param $configuration
 *   array, media mover configuration
 * @param $test
 *   boolean, is this in testing mode
 * @return unknown_type
 */
function media_mover_ftp_harvest_file_recurse(&$ftp_connection, $ftp_files, $directory, $configuration, $test) {
	// first get this file, then recurse if necessary
	if (count($ftp_files)) {
		$files = array();
		foreach($ftp_files as $ftp_file) {
			if ($ftp_file['dir']) {				
				// merge files together
				$files = array_merge($files, media_mover_ftp_harvest_file_recurse($ftp_connection, $ftp_file['dir'], $directory .'/'. $ftp_file['filename'], $configuration, $test));
			}
			else {
			  if ($new_file =  media_mover_ftp_harvest_ftp_file($ftp_connection, $ftp_file, $configuration, $test)) {
			  	$files[] = $new_file;
			  }
			}
		}
		return $files;	
	}
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
function media_mover_ftp_harvest_ftp_file(&$ftp_connection, $ftp_file, $configuration, $test = false) {
  // Get each item and save it locally
  if ($file = media_mover_ftp_create_file($ftp_connection, $ftp_file, $configuration, $test)) {
    // should we delete the file after we've harvested it?
    if ($configuration['mm_ftp_delete_files']) {
      if (! ftp_delete($ftp_connection, $ftp_file['filepath'])) {
        $message = 'Error deleting file: @file';
        $params = array('@file' => $file['filepath']);
        // log or display errors
        if ($test) { drupal_set_message(t($message, $params)); }
        else { watchdog(MM_FTP_WATCHDOG_TYPE, t($message, $params), WATCHDOG_ERROR); }
        return FALSE;
      }
      if ($test) {drupal_set_message(t('Removed the file from the FTP server.')); }
    } 
    // return the item if we have it
    if ($file) {
      return $file;
    }
  }
}


/**
 * function for saving the file from the FTP server to the local drupal directory
 * @param object $ftp_conn is the ftp connection
 * @param array $configuration is the media mover configuration
 * @param $test
 *   boolean, display or log errors
 * @return array $item is a item array
 */
function media_mover_ftp_create_file(&$ftp_connection, $ftp_file, $configuration, $test) {
  // If this is a folder, do nothing
  if (is_array($ftp_file['dir'])) {
    return;
  }
  
  // does this file have a token, or are we allowed ot harvest files without tokens?
  if (! $ftp_file['token'] && ! $configuration['mm_ftp_harvest_without_tokens'] ) {
    // should we delete files that do not have a token from the ftp server?
    if ($configuration['mm_ftp_delete_files_no_token']) {
      ftp_delete($ftp_connection, $ftp_file['filepath']);
      watchdog(MM_FTP_WATCHDOG_TYPE, 'Deleted FTP file without token.', array(), WATCHDOG_NOTICE);
    }
    // now we need to return because we are not supposed to harvest this file type
   // return false;
  }
  
  // which file path should be used?
  $local_file_path = $ftp_file['filepath_no_token'] ? $ftp_file['filepath_no_token'] : $ftp_file['filename'];
  
  // create the complete path
  $local_file_path = file_create_filename($local_file_path,  media_mover_api_dir_path() . MM_FTP_FILES_SUBDIRECTORY);

  // only harvested file types that match the requested types
  if (! media_mover_api_file_validate_extensions($local_file_path, $configuration['mm_ftp_allowed_files'])) {
    $message = 'FTP file harvest: unsupported file extension !file';
    $params = array('!file' => $local_file_path);
    // display errors
    if ($test) { drupal_set_message(t($message, $params), 'error'); }
    else { watchdog(MM_FTP_WATCHDOG_TYPE, t($message, $params)); }
    return false;
  }

  // make sure directory exists
  @mkdir(dirname($local_file_path), 0775, true);

  // download the file via FTP
  if (! ftp_get($ftp_connection, $local_file_path, $ftp_file['filepath'], FTP_BINARY)) {
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

  $file = array();
  $file['filemime'] = $mime;
  $file['filesize'] = filesize($local_file_path);
  $file['filename'] = basename($local_file_path);
  $file['filepath'] =  $local_file_path;
  $file['harvest_file'] = $local_file_path;
  $file['node']->title = $local_file_path;
  
  // try to associate data with a user name
  media_mover_ftp_extract_user($ftp_file, $file, $configuration);
  
  return $file;
}
