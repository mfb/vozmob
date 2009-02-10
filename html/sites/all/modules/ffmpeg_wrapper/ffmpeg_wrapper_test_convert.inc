<?php

// $Id: ffmpeg_wrapper_test_convert.inc,v 1.1.2.4 2008/12/18 15:19:37 arthuregg Exp $

/**
 * @FILE This file provides the transcoding testing capabilities
 *       for ffmpeg_wrapper
 */


/* ************************************************** */
/* TEST CONVERSION FORM FUNCTIONS                     */
/* ************************************************** */


/**
 * This creates a form for testing files. If data is being passed back into this
 * form it will use that data to attempt to convert the file.
 *
 * @NOTE This is a LARGE potential security risk. Only admins
 *       should have access to this function.
 *
 * @param string $path
 *   path to the file to manipulate
 * @param string $type
 *   type of item passing file (node, media_mover, etc)
 * @param int $id
 *   $id related to the $type, used to pass item back
 * @return array
 *   drupal form array
 */
function ffmpeg_wrapper_ffmpeg_test_form($form_state) {
  // If we have incoming files we are ready to process the files for conversion
  if (! empty($form_state['values'])) {
    $ffmpeg_object = $form_state['ffmpeg_object'];
    // set messages- we do this instead of drupal_message()
    $form['messages'] = array(
      '#type' => 'markup',
      '#value' => $ffmpeg_object->errors ? t('There were errors during the conversion!') : t('Your file was successfully transcoded!'),
      '#prefix' => '<h3>',
      '#suffix' => '</h3>',
    );
    // display the conversion output
    $form['data'] = array(
      '#type' => 'fieldset',
      '#title' => t('Conversion data'),
    );
    if ($ffmpeg_object->errors){
      $form['data']['errors'] = array(
        '#type' => 'markup',
        '#value' => t('Errors found: !errors', array('!errors' => implode('<br />', $ffmpeg_object->errors))),
        '#suffix' => '<br />',
      );
    }
    $form['data']['command'] = array(
      '#type' => 'markup',
      '#value' => t('<b>Command run was:</b> !command', array('!command' => $ffmpeg_object->command)),
      '#suffix' => '<br />',
    );
    $form['data']['output_file'] = array(
      '#type' => 'markup',
      '#value' => t('<b>Output file is:</b> !command', array('!command' => l($ffmpeg_object->output_file, file_create_url($ffmpeg_object->output_file)))),
      '#suffix' => '<br />',
    );
    $form['data']['ffmpeg_output'] = array(
      '#type' => 'fieldset',
      '#title' => t('FFmpeg Output'),
      '#collapsible' => true,
      '#collapsed' => true,
    );
    $form['data']['ffmpeg_output']['output'] = array(
      '#type' => 'markup',
      '#value' => '<pre>'. $ffmpeg_object->output .'</pre>',
    );
  }

  // header at the top of the page
  $form['info'] = array(
    '#type' => 'markup',
    '#value' => t('This allows you to test a configuration on a file that you specify as the input file. Additional files can be added in the directory: <br /> %path', array('%path' =>$path)),
  );

  // ------------------------------------------
  // AHAH form building

  // make sure we have files before we build the file data
  if ($files = ffmpeg_wrapper_test_build_files()) {
    // if FFmpeg was run, get the ID of the file so that
    // the form value for the radios can be set to the new file
    $output_file_id = ffmpeg_wrapper_test_file_id($files, $ffmpeg_object->output_file);

    $form['files'] = array(
      '#theme' => 'ffmpeg_wrapper_files_radios',
    );

    // build the radio form element that the admin can choose from to process a
    // file, option is tied to AHAH.
    $form['files']['data'] = array(
      '#type' => 'radios',
      '#default_value' => $output_file_id ? $output_file_id: $form_state['values']['files']['data'],
      '#options' => $files,
      '#suffix' => '<div id="ffmpeg_wrapper_file_data_display"></div>',
      '#ahah' => array(
        'event' => 'change',
        'path' => 'ffmpeg_wrapper/file_data',
        'wrapper' => 'ffmpeg_wrapper_file_data_display',
        'method' => 'replace',
        ),
    );

    // build the form elements for attaching a file back to a node
    $form['files']['attach_files'] = array(
      '#type' => 'radios',
      '#default_value' => $output_file_id ? $output_file_id: $form_state['values']['files']['attach'],
      '#options' => $files,
    );

    // build the options for the files and sub elements
    foreach ($files as $id => $file) {
      $file_name = $files[$id];
      $form['files'][$id]['name'] = array('#type' => 'markup',  '#value' => check_plain(basename($file_name)));
      $form['files'][$id]['size'] = array('#type' => 'markup',  '#value' => format_size(filesize($file_name)));
      $form['files'][$id]['mime'] = array('#type' => 'markup',  '#value' => file_get_mimetype($file_name));
    }

    $form['files_lookup'] = array(
      '#type' => 'value',
      '#value' => $files,
    );

  }

  // There were no files passed in, give the default path
  else {
    drupal_set_message(t('Sorry there are no files to test. Please place some files to test in !path',
      array('!path' => variable_get('ffmpeg_wrapper_test_test', drupal_get_path('module', 'ffmpeg_wrapper') .'/test'))));
    return;
  }

  // set the nid for the attach option
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $_GET['nid'] ? mysql_escape_string($_GET['nid']) : ($form_values['nid'] ? $form_values['nid'] : null),
  );

  // -----------------------------------------------------
  // FFmpeg form building

  // get the FFmpeg form
  $form['ffmpeg'] = ffmpeg_wrapper_configuration_form($form_state['values']);

  // add a submit button
  $form['submit'] = array(
    '#value' => t('Transcode'),
    '#type' => 'submit',
  );

  // add a submit button
  $form['attach'] = array(
    '#value' => t('Attach'),
    '#type' => 'submit',
    '#submit' => array('ffmpeg_wrapper_ffmpeg_test_file_attach'),
  );

  // This part is important!
  $form['#multistep'] = TRUE;
  $form['#redirect'] = FALSE;

  return $form;
}


/**
 * Checks to make sure data coming in is sane.
 * @param string $form_id
 * @param array $form_values
 */
function ffmpeg_wrapper_ffmpeg_test_form_validate($form_id, &$form_state) {
  // did the user transcode or attach the file?
  if ($form_state['values']['op'] == t('Transcode')) {
    // make sure an output type is set
    if (! $form_state['values']['ffmpeg_output_type']) {
      form_set_error('ffmpeg_output_type', t('You must specify an output type'));
    }
    // make sure a file was selected
    if (! isset($form_state['values']['data'])) {
      form_set_error('files', t('You must choose a file to transcode'));
    }
  }

  if ($form_state['clicked_button']['#post']['op'] == t('Attach')) {
    // get the path to the file
    $path = $form_state['values']['files_lookup'][$form_state['values']['attach_files']];
    // make sure we have an NID
    if (! $form_state['values']['nid']) {
      form_set_error('', t('Sorry, there is no node to attach this file back to.'));
    }
    // make sure a file was selected
    elseif (! isset($form_state['values']['attach_files'])) {
      form_set_error('attach', t('You must choose a file to attach'));
    }
    elseif(! file_exists($path)) {
      form_set_error('', t('The file you are trying to attach does not seem to exist. Maybe this was a bad transcode?'));
    }
    elseif(filesize($path) < 10) {
      form_set_error('', t('The file you are trying to attach a file does not seem to have data. Maybe this was a bad transcode?'));
    }
  }
}

function ffmpeg_wrapper_ffmpeg_test_form_submit($form, &$form_state) {
  // get the path to the file
  $path = $form_state['values']['files_lookup'][$form_state['values']['data']];
  // run the conversion process with the settings
  $form_state['ffmpeg_object'] = ffmpeg_wrapper_ffmpeg_test_form_run($form_state['values'], $path);
  // we will rebuild the form.
  $form_state['rebuild'] = TRUE;
}


/**
 * build the confirm form for removing all the files generated by the tests
 */
function ffmpeg_wrapper_test_cleanup_form() {
  $form['status'] = array(
    '#type' => 'markup',
    '#value' => t('When testing, FFmpeg Wrapper creates output files as well as copying input files. This allows you to remove them and reclaim disk space. '),
  );
  return confirm_form(
    $form,
    t('Are you sure you want to clean up the test directories? This will delete any of the test files that you have created.'),
    'admin/settings/ffmpeg_wrapper',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}


/**
 * deletes the test directories
 * @param string $form_id
 * @param array $form_values
 */
function ffmpeg_wrapper_test_cleanup_form_submit($form_id, &$form_state) {
  // set the paths for removal
  $paths = array(
    file_directory_path() .'/ffmpeg_wrapper_test_output',
    file_directory_path() .'/ffmpeg_wrapper_test_input',
  );

  // get a list of the files that are currently in the directories
  foreach ($paths as $path) {
    if (is_dir($path)) {
      $files = file_scan_directory($path, '.*$');
      if (count($files)) {
        foreach ($files as $file) {
          file_delete($file->filename);
        }
      }
      // now delete the directory
      rmdir($path);
    }
  }

  // clear the cached file
  cache_clear_all('ffmpeg_wrapper_test_file', 'cache');
  drupal_set_message(t('The test data has been deleted'));
  return 'admin/settings/ffmpeg_wrapper';
}


/* ************************************************** */
/* TEST CONVERSION FUNCTIONS                          */
/* ************************************************** */


/**
 * This function builds out the list of files used in the
 * landing form
 * @return array
 */
function ffmpeg_wrapper_test_build_files() {
  // -------------------------------------------------
  // Start building the normal form
  $paths = array();
  // set the path to our directories
  $paths['test'] = variable_get('ffmpeg_wrapper_test_test', drupal_get_path('module', 'ffmpeg_wrapper') .'/test');
  $paths['out'] = variable_get('ffmpeg_wrapper_test_out', file_directory_path() .'/ffmpeg_wrapper_test_output');
  $paths['in'] = variable_get('ffmpeg_wrapper_test_in', file_directory_path() .'/ffmpeg_wrapper_test_input');

  // build the file list from these directories
  // get a list of files that can be processed
  $files = array();
  foreach ($paths as $path) {
    $files = array_merge($files, file_scan_directory($path, '.*$'));
  }

  // get the files into shape for displaying
  if (count($files)) {
    // extract the filenames
    $return = array();
    foreach ($files as $file) {
      $return[] = check_plain($file->filename);
    }
  }

  // If we have an incoming file copy that has not matched other files
  // copy it into the test directory
  if ($file_path = mysql_escape_string($_GET['path'])) {
    if (! file_exists($file_path)) {
      drupal_set_message(t('Sorry, that file does not exist'), 'error');
    }
    else {
      // loop through the list of files and look for the requested file
      $found = false;
      foreach ($return as $file) {
        if (basename($file) == basename($file_path)) {
          // the file was found
          $found = true;
        }
      }
      if (! $found ) {
        file_check_directory($paths['in'], FILE_CREATE_DIRECTORY);
        // we make a copy of the file for testing
        if (file_copy($file_path, $paths['in']) ) {
          drupal_set_message(t('Copied your file into the test directory'));
          // add the new file to the array
          $return[] = $file_path;
        }
        else {
          drupal_set_message(t('Could not copy your file into the test directory'), 'error');
        }
      }
    }
  }
  return $return;
}


/**
 * Get the ID of the incoming file from the $files array
 * if there is one?
 * @param $files
 *   array of files
 * @param $path
 *   path to look for
 * @return unknown_type
 */
function ffmpeg_wrapper_test_file_id($files = null, $path = null) {
  if ($files && $path) {
    foreach($files as $id => $file) {
      if ($path == $file) {
        return $id;
      }
    }
  }
  return false;
}


/**
 * This runs FFmpeg based on the form data passed into it.
 * @param string $input_file
 *   path to the file to operate on
 * @param array $params
 *   configuration options in the format set in the ffmpeg_wrapper_configuration_form()
 * @param string $output_file_path
 *   where to place the file, assumes same dir as $input_file. No trailing slash
 * @param object $ffmpeg_object
 *   contains debug information that calling functions can utilize
 * @return string
 *
 */
function ffmpeg_wrapper_configuration_form_convert($input_file, $params, $output_file_path = null, &$ffmpeg_object) {
  // first error check, make sure that we can decode this kind of file
  if (! ffmpeg_wrapper_can_decode($input_file)) {
    $message = 'FFmpeg Wrapper can not decode this file: !file';
    $variables = array('!file' => l($input_file, file_create_url($input_file)));
    watchdog('media_mover', $message, $variables, WATCHDOG_ERROR);
    $ffmpeg_object->errors[] = $message;
    return false;
  }

  // build the output file path if we don't have one. Use the output type as the extension.
  $output_file = file_create_filename(basename($input_file) .'.'. $params['ffmpeg_output_type'], ($output_file_path ? $output_file_path : dirname($input_file)));

  // did the admin define a specific FFmpeg comand to run?
  //  we only run what the admin specified
  if ($params['ffmpeg_video_custom']) {
    $options[] = str_replace(array('%in_file', '%out_file'), array($input_file, $output_file), $params['ffmpeg_video_custom_command']);
  }
  // build a standard configuration
  else {
    // build the ffmpeg command structure out
    $options = array();

    // input file
    $options[] = "-i '". $input_file ."'";

    // build the watermark config
    if ($params['ffmpeg_video_wm']) {
      $options[] = "-vhook '". ffmpeg_wrapper_path_to_vhook('watermark.so') ." -f ". $params['ffmpeg_video_wm_file'] ."'";
    }

    // build the audio config
    if ($params['ffmpeg_audio_advanced']) {

      // use a specifc codec?
      if ($params['ffmpeg_audio_acodec']) {
        $options[] =  '-acodec '. $params['ffmpeg_audio_acodec'];
      }

      // use a specific sample rate?
      if ($params['ffmpeg_audio_ar'] ) {
        $options[] = '-ar '. $params['ffmpeg_audio_ar'];
      }

      // use a specific bit rate?
      if ($params['ffmpeg_audio_ab']) {
        $options[] = '-ab '. $params['ffmpeg_audio_ab'];
      }
    }

    // build the video config
    if ($params['ffmpeg_video_advanced']) {

      // is codec set?
      if ($params['ffmpeg_video_vcodec']) {
        $options[] = '-vcodec '. $params['ffmpeg_video_vcodec'];
      }

      // is frame size set?
      if ($params['ffmpeg_video_size']) {
        $options[] = '-s '. $params[$params['ffmpeg_video_size'] == 'other' ? 'ffmpeg_video_size_other' : 'ffmpeg_video_size'];
      }

      // is the bit rate set?
      if ($params['ffmpeg_video_br']) {
        $options[] = '-b '. $params['ffmpeg_video_br'];
      }

      // is frame rate set?
      if ($params['ffmpeg_video_fps']) {
        $options[] = '-r '. $params['ffmpeg_video_fps'];
      }
    }

    // implement truncating
    if ($params['ffmpeg_time_advanced']) {
      $options[] = '-t '. $params['ffmpeg_time'];
    }

    // add the output file
    $options[] = "'". $output_file ."'";
  }

  $ffmpeg_object->command = implode(" ", $options);

  // run ffmpeg with error checking
  if (! $success = ffmpeg_wrapper_run_command($ffmpeg_object->command, true, null, $ffmpeg_object)) {
    return;
  }

  // successful convert, make a note in the log
  $message = 'FFmpeg converted this file: !file';
  $message .= '<br />'. 'FFmpeg ran this command: <br /><pre> !command </pre>';
  $variables = array('@file' => $output_file, !command =>  $ffmpeg_object->command);
  watchdog('FFmpeg', $message, $variables, WATCHDOG_NOTICE);

  $ffmpeg_object->output_file = $output_file;

  // return the completed file path
  return $output_file;
}


/**
 * This function is called after the form is submitted and when the form data returns
 * to the form function
 * @param $form_values
 * @return array
 */
function ffmpeg_wrapper_ffmpeg_test_form_run($form_values, $file_path) {
  $ffmpeg_object = new StdClass;
  // create a test directory if there is not already one
  $path = file_directory_path() .'/ffmpeg_wrapper_test_output';
  // make sure the directory exists
  file_check_directory($path, FILE_CREATE_DIRECTORY);
  // run ffmpeg on the file and configuration
  ffmpeg_wrapper_configuration_form_convert($file_path, $form_values, $path, $ffmpeg_object);
  return $ffmpeg_object;
}


/**
 * Takes the file chosen for attachment and attaches it to a node
 * @param $form_values
 * @return unknown_type
 */
function ffmpeg_wrapper_ffmpeg_test_file_attach($form_id, &$form_state) {
  // get the file to attach from the data
  $attach_file = $form_state['values']['files_lookup'][$form_state['values']['attach_files']];
  // load the node object
  $node = node_load($form_state['values']['nid']);
  // build the file object
  $file = new stdClass();
  $file->new = true;
  $file->fid = 'new';
  $file->uid = $node->uid;
  $file->filename = basename($attach_file);
  $file->filepath = file_create_filename(basename($attach_file), file_directory_path());
  $file->filemime = file_get_mimetype($attach_file);
  $file->filesize = filesize($attach_file);
  $file->timestamp = time();
  $file->list = variable_get('upload_list_default', 1);
  $file->status = 1;
  // save the file
  drupal_write_record('files', $file);
  // add the file to the node object
  $node->files[$file->fid] = $file;
  // save the modified node
  node_save($node);
  // alert the user
  drupal_set_message(t('Attached your transcoded file'));
  drupal_goto('node/'. $node->nid .'/edit');
}


/**
 * returns a json array of file type data
 * @return string
 */
function ffmpeg_wrapper_file_type_ahah() {
  // Get the cache form data
  $cached_form = form_get_cache($_POST['form_build_id'], &$cached_form_state);

  // Did we find a file that we can use?
  if ($file = $cached_form['files']['data']['#options'][$_POST['data']]) {
    if (! file_exists($file)){
      drupal_json(t('File does not exist'));
      exit();
    }

    // get the ffmpeg file data
    $file_data = ffmpeg_wrapper_file_data($file);

    // theme the output
    $meta = array(
      t('File: !file', array('!file' => check_plain(basename($file)))),
      t('Format: !format', array('!format' => $file_data['format'])),
      t('Duration: !duration (seconds)', array('!duration' => $file_data['duration'])),
      t('Total bitrate: !rate', array('!rate' => $file_data['bitrate'])),
    );

    $video = array(
      t('Video codec: !codec', array('!codec' => $file_data['video']['codec'])),
      t('Frame size: !s', array('!s' => $file_data['video']['s'])),
      t('Bit rate: !br', array('!br' => $file_data['video']['br'])),
      null,
    );

    $audio = array(
      t('Audio codec: !codec', array('!codec' => $file_data['audio']['codec'])),
      t('Sample rate: !ar', array('!ar' => $file_data['audio']['ar'])),
      t('Bit rate: !br', array('!br' => $file_data['audio']['ab'])),
      t('Chanels: !ac', array('!ac' => $file_data['audio']['ac'])),
    );

    // build the rows for the display
    $rows = array($meta, $video, $audio);
    $output = theme('table', array(t('File information')), $rows);

    // return the html in json format
    drupal_json($output);
    exit();
  }
}

/* ************************************************** */
/* TEST CONVERSION THEME FUNCTIONS                    */
/* ************************************************** */

/**
 * Themes the list of radio buttons and other form data
 * @param $form
 *   drupal form
 * @return unknown_type
 */
function theme_ffmpeg_wrapper_files_radios(&$form) {
  $rows = array();
  $header = array(t('File Name'), t('Use'), t('Attach'), t('Mime type'), t('Size'));
  // select each of the options from the $files form and build from the #options
  foreach (element_children($form['data']) as $key) {
    $row = array();
    // name of the file
    $row[] = drupal_render($form[$key]['name']);
    // render the radio
    unset($form['data'][$key]['#title']);
    $row[] = drupal_render($form['data'][$key]);
    // create the attach files radio, remove the title
    unset($form['attach_files'][$key]['#title']);
    $row[] = drupal_render($form['attach_files'][$key]);
    $row[] = drupal_render($form[$key]['mime']);
    $row[] = drupal_render($form[$key]['size']);
    $rows[] = $row;
  }
  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}