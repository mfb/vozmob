 <?php

// $Id: mm_s3_test.inc,v 1.1.2.1.2.7 2009/03/22 20:44:33 arthuregg Exp $

/**
 * This file provides some testing configuration for the s3 module
 * files is based on http://undesigned.org.za/2007/10/22/amazon-s3-php-class
 */

/* **************************************************** */
/* Forms */
/* **************************************************** */


/**
 * build a form that will allow testing of Media Mover configurations
 * ftp settings
 * @return unknown_type
 */
function mm_s3_test_form() {
  // find all the media mover configurations that us FTP
  if (! $options = media_mover_api_get_configurations_by_module('mm_s3')) {
    drupal_set_message(t('You need to have Media Mover configurations which use S3 in order to test your connection'));
  }

  $form['display'] = array(
    '#type' => 'markup',
    '#value' => t('This test moves a file to Amazon S3 server, copies the file back, and then removes the file from S3'),
  );

  $form['cid'] = array(
    '#type' => 'select',
    '#title' => t('Select a configuration to test'),
    '#options' => $options,
    '#description' => t('Choose a configuration to test sending and receiving a file from.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Test'),
  );

  return $form;
}


/**
 * implement the testing functionality
 */
function mm_s3_test_form_submit($form, &$form_state) {

  // -------------------------------------------------
  // POSSIBLE CONFIGURATION
  // get the file we will push to s3
  $test_upload = drupal_get_path('module', 'mm_s3'). '/test.png';
  // file name is the file name
  $test_upload_name = basename($test_upload);
  // path to the download file. drupal files should be writeable
  $test_download = file_directory_path() . '/s3_download_'. $test_upload_name;

  // get the configuration data
  $configuration = media_mover_api_get_configuration($form_state['values']['cid']);
  // find the FTP settings
  foreach (media_mover_api_verbs() as $verb) {
    if ($configuration->{$verb}->module == 'mm_s3') {
      $conf = $configuration->{$verb}->configuration;
      break;
    }
  }


  /* **************************************************** */

  // Check if our upload file exists
  if (!file_exists($test_upload) || !is_file($test_upload))
    exit("\nERROR: No such file: $test_upload\n\n");

  // are the keys good?
  if ($s3 = mm_s3_class_check()) {
    // List your buckets:
    drupal_set_message("Listing all buckets for your S3 account:<br />". theme('item_list', $s3->listBuckets()));
    // does the requested bucket exist?
    if (mm_s3_bucket_exists($conf['mm_s3_bucket'], $s3)) {
      drupal_set_message(t('We found your bucket: !bucket on your account on the Amazon server', array('!bucket' => $conf['mm_s3_bucket'])));
    }
    else {
      $output[] = t('We did not find your bucket on your account on the Amazon server. Attempting to create it.');
      // Create a bucket with public read access
      if (! $s3->putBucket($test_bucket, $conf['mm_s3_perm'], false, false)) {
         drupal_set_mesage(t("Could not create your bucket: ". $conf['mm_s3_bucket'] .",  probably already exists"), 'error');
      }
      else {
        drupal_set_mesage(t("Created bucket ". $conf['mm_s3_bucket']));
      }
    }

    // Get the contents of our bucket
    if ($contents = $s3->getBucket($conf['mm_s3_bucket'])) {
      // get a usable list of files
      foreach($contents as $id => $file) {
        $list[] = $id;
      }
      drupal_set_message(t('S3::getBucket(): Files in bucket !bucket: !items', array(
        '!bucket' => $conf['mm_s3_bucket'],
        '!items' => theme('item_list', $list)
      )));
    }

    // Put our file with the read access defined
    if ($s3->putObjectFile($test_upload, $conf['mm_s3_bucket'], baseName($test_upload), $conf['mm_s3_perm'])) {
      drupal_set_message(t('S3::putObjectFile(): File copied to !path', array(
        '!path' => $conf['mm_s3_bucket'] .'/'. baseName($test_upload)
      )));
    }
    else {
      drupal_set_message(t('S3::putObjectFile(): Failed to copy file\n'), 'error');
    }

    // Get object info
    $info = $s3->getObjectInfo($conf['mm_s3_bucket'], baseName($conf['mm_s3_bucket']));
    drupal_set_message(t('S3::getObjecInfo(): Info for !file: !items', array(
      '!file' => $conf['mm_s3_bucket'] .'/'. baseName($conf['mm_s3_bucket']),
      '!items' => theme('item_list', $info)
    )));

    // Delete our file
    if ($s3->deleteObject($conf['mm_s3_bucket'], baseName($test_upload))) {
      drupal_set_message(t('S3::deleteObject(): Deleted file'));
    }
    else {
      drupal_set_message(t('S3::deleteObject(): Failed to delete file'), 'error');
    }

    // Delete the bucket we created (a bucket has to be empty to be deleted)
    if ($s3->deleteBucket($conf['mm_s3_bucket'])) {
      drupal_set_message(t('s3::deleteBucket(): Deleted bucket !bucket', array(
        '!bucket' => $conf['mm_s3_bucket']
      )));
    }
    else {
      drupal_set_message(t('S3::deleteBucket(): Failed to delete bucket- it may not be empty'), 'error');
    }
  }
  return;
}