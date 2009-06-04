<?php
// $Id$

/**
 * @file
 * Cleanup messages received from sprint.
 */


/**
 * callback function to for extracting name/value pairs from html input attributes
 */

function mms_us_sprint_build_zip_get($item, $key, &$zip_get) {
    //we're building the mms_us_sprint_get array.
    // $key is the input element's name attribute
    // $value is the input element's value attribute
    //ideally we would use preg_filter to extract the useful name/value pairs but it's a new php feature so not everywhere

    $key = preg_replace('!(.+)(name=")(.*?)(".*)(>?)(.*)!',"\\3\n", $item);
    $value = preg_replace('!(.+)(value=")(.*?)(".*)(>?)(.*)!',"\\3\n", $item);
    //only add key/value to the mms_us_sprint_get if preg_replace extracted some value
    if(strcmp($value, $item))
        $zip_get[] = trim($key).'='.trim($value);
}



/**
 * Implementation of hook_mailsave_clean().
 */
function mms_us_sprint_mailsave_clean(&$node, $result, $i, $header, $mailbox) {
// Check if this message was from sprint
  if (preg_match('!@pm.sprint.com.?$!i', $header->fromaddress)) {
    if (preg_match('!(http://pictures\.sprintpcs\.com/share\.do\?invite=.*)"!', $node->body, $matches)) {
      // extract the voice memo url
      $url = preg_replace('!(.+)href="(.+)">Play Voice Memo</a>(.+)!i', '\\2', preg_replace('!\n!','',$node->body));
      // create mailsave_attachments array, used for voice .wav then for zipped media files
      $node->mailsave_attachments = array();
      // fetch and save voice memo if url is found
      if(preg_match('!^http(.+)!', $url)) {
        $attachments = drupal_http_request($url, array('User-Agent' => 'Wget/1.11.2'));
        $temp_file = tempnam(file_directory_temp(), 'sprint');
        $filepath = file_save_data($attachments->data, $temp_file);
        $path_parts = pathinfo($filepath);
	$filesize = filesize($filepath);
        $filename = md5(uniqid(mt_rand(), TRUE)) . '.wav'; // . $path_parts['extension'];
        $node->mailsave_attachments[] = array(
          'filename' => _mailsave_sanitise_filename($filename),
          'filepath' => $filepath,
          'filemime' => 'audio/x-wav',
          'filesize' => $filesize,
        );
      }
      // replace & entity
      $url = preg_replace('/&amp;/i', '&', $matches[1]);
      // Sprint sends HTTP 500 error if user agent is PHP, so we use wget :)
      $message_view = drupal_http_request($url, array('User-Agent'=>'Wget/1.11.2'));
      // break up the page by form delimiter so we can toss any other forms & nested preg_replace to get rid of newlines
      $forms_exploded = preg_split("!<form !i",
                                   preg_replace("!\n!", "", $message_view->data));
      // preg_grep returns only the array elements which contain the form name (listForm), so we toss any other forms, then implode the remaining element back into a string
      $scraped_form = implode('', preg_grep("!listForm!", $forms_exploded));
      // Get the message subject.
      if (preg_match('!<strong>Subject:</strong> (.*)!', $message_view->data, $matches)) {
        $node->title = $matches[1];
      }
      //set the node body to the mms message body
      $node->body = preg_replace('!(.+)<pre id="guestTxtMemoDisplayId" class="guestTxtMemoDisplay">(.+)</pre>(.+)!i', '\\2', $scraped_form);
      $node->teaser = node_teaser($node->body);
      // build an array of input elements to form get string to pull zip file
      $input_element_array = preg_split("!<(input )|<(select )!i", $scraped_form);
      $zip_get = array();
      // the call back function will extract the first name/value pairs and build the mms_us_sprint_get array
      array_walk($input_element_array, 'mms_us_sprint_build_zip_get', &$zip_get);
      // implode the mms_us_sprint_get into a formatted GET string
      $url = 'http://pictures.sprintpcs.com/downloadmsgmedia/?' . implode('&',$zip_get);
      // Retrieve the ZIP file, currently by reading the entire file into a variable!
      // Use a working user agent header.
      $attachments = drupal_http_request($url, array('User-Agent' => 'Wget/1.11.2'));
      $temp_file = tempnam(file_directory_temp(), 'sprint');
      $filepath = file_save_data($attachments->data, $temp_file);
      $zip = zip_open($filepath);
      if (is_resource($zip)) {
//        $node->mailsave_attachments = array();
        while ($zip_entry = zip_read($zip)) {
          echo "\nzip_entry"; print_r(zip_entry_name($zip_entry));
          if (zip_entry_open($zip, $zip_entry)) {
            // File name is just a digit, so generate a more unique file name.
            $path_parts = pathinfo(zip_entry_name($zip_entry));
            $filename = md5(uniqid(mt_rand(), TRUE)) . '.' . $path_parts['extension'];
            $filesize = zip_entry_filesize($zip_entry);
            // Save the data to temporary file
            $temp_file = tempnam(file_directory_temp(), 'mail');
            $filepath = file_save_data(zip_entry_read($zip_entry, $filesize), $temp_file);
            // Add the file to the attachments array, and sanitise filename.
            $node->mailsave_attachments[] = array(
              'filename' => _mailsave_sanitise_filename($filename),
              'filepath' => $filepath,
              'filemime' => file_get_mimetype($filename),
              'filesize' => $filesize,
            );
            zip_entry_close($zip_entry);
          }
        }
        zip_close($zip);
      }else{
        watchdog('mms_us_sprint', 'zip is not a resource');
      }
    }
    else
    {
      echo 'WTF, THERE IS NO LINKS??!?';
    }
  }
}

/**
 * Report back module information when requested
 */
function mms_us_sprint_mailsave_clean_info() {
  $items['mms_us_sprint'] = array(
    'provider'    => t('Sprint'),
    'country'     => 'US',
    'author'      => 'Josh Haglund',
    'description' => t('Clean up MMS messages sent via the Sprint US network.'),
  );
  return $items;
}
