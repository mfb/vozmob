<?php
// $Id: mm_s3_test.inc,v 1.3 2008/01/14 23:41:57 bdragon Exp $

/**
 * This file provides some testing configuration for the s3 module
 * files is based on http://freshmeat.net/projects/storage3
 * original author is: apokalyptik@apokalyptik.com
 *
 */

/* **************************************************** */
/* Possible configuration */
/* **************************************************** */

$test_bucket = "mm_s3_bucket";

$test_upload = drupal_get_path('module', 'mm_s3') .'/test.png';

// file name is the file name
$test_upload_name = basename($test_upload);

// path to the download file. drupal files should be writeable
$test_download = file_directory_path() .'/s3_download_'. $test_upload_name;

// delete downloaded file?
$delete_test = false;

/* **************************************************** */


// get the s3 drivers
mm_s3_drivers_check();

$my_access_key_id = variable_get('mm_s3_key', null);
$my_secret_access_key = variable_get('mm_s3_skey', null);

// Create our Amazon S3 Service Interface
$s3 = new storage3($my_access_key_id, $my_secret_access_key, "https://s3.amazonaws.com/");

 /* Create a test bucket */
if ( $s3->mkBucket($test_bucket) ) {
  echo "Created bucket on s3! <br />";

   /* Put a test file to the service */
  if ( $s3->putFile($test_upload, $test_bucket, $test_upload_name) ) {
    print 'Put test file on S3 <br />';

     /* Fetch the file we just put into the amazon service */
    if ( $s3->saveFile($test_bucket, $test_upload_name, $test_download) ) {
      echo "Retrieved uploaded file from s3 successfully! <br />";

       /* Make sure that it contains the same data as the file we put in */
      $comp1 = md5_file($test_upload);
      $comp2 = substr($s3->responseHeaders['etag'], 1, -1);
      if ( $comp1 === $comp2 ) {
        echo "InFile == OutFile! <br /> ";
      }
      else {
        echo "InFile != OutFile <br /> Something is wrong with the signatures of these files";
      }

       /* Remove (recursively) the test bucket */
      $s3->rmBucket("$test_bucket", 1);

      /* clean up the downloaded file */
      if ($delete_test) {
        // @@@ This isn't referenced anywhere. --Bdragon
        //unlink($testOutFile);
      }

    }
    else {
      echo "Could not save file locally. Check write permissions \r\n";
      print_r($s3->why());
    }
  }
  else {
    echo "Could Not Put File\r\n";
    print_r($s3->why());
  }
}
else {
  echo "Could Not Make Bucket\r\n";
  print_r($s3->why());
  if ( $s3->rmBucket("$test_bucket", 1) ) {
    echo "Removed Bucket, Please Try Again\r\n";
  }
  else {
    echo "Could Not Remove Bucket\r\n";
    print_r($s3->why());
  }

}
