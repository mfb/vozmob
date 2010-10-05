<?php
// $Id$

/**
 * @file
 * Cleanup messages received from boost.
 */

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_us_boost_mailsave_clean(&$node, $result, $i, $header, $mailbox) {
  // Check if this message was from boost.
  if (preg_match('!@myboostmobile.com$!i', $header->fromaddress)) {
    if (preg_match('!(http://mediamail\.boostmobile\.com/share\.do\?invite=.*)"!', $node->body, $matches)) {
      // Connect via HTTPS to (help) protect private data.
      $url = str_replace('http://', 'https://', $matches[1]);
      // Boost sends HTTP 500 error if user agent is PHP, so we use Drupal :)
      $message_view = drupal_http_request($url);
      // Get the message subject.
      if (preg_match('!<strong>Subject:</strong> (.*)!', $message_view->data, $matches)) {
        $node->title = decode_entities(strip_tags($matches[1]));
      }
      $url = str_replace('share.do', 'guest/downloadMedia.do', $url);
      $attachment_view = drupal_http_request($url);
      if (preg_match('!<ul id="ul_items">.*?</ul>!s', $attachment_view->data, $matches)) {
        // Fix some bad XHTML.
        $html = str_replace(array('&', 'border=0'), array('&amp;', 'border="0"'), $matches[0]);
        if ($attachment_view = simplexml_load_string($html)) {
          $url = str_replace('guest/downloadMedia.do', 'mmsdownloadzip/', $url);
          foreach ($attachment_view->li as $attachment) {
            $url .= '&externalMediaIds=' . $attachment->input[0]->attributes()->value;
            preg_match('/_([0-9]+)$/', $attachment->input[0]->attributes()->value, $matches);
            $url .= '&' . $matches[1] . 'ext_mms=' . $attachment->input[1]->attributes()->value;
          }
          // Retrieve the ZIP file, currently by reading the entire file into a variable!
          // Use a working user agent header.
          $attachments = drupal_http_request($url, array('User-Agent' => 'Wget/1.11.2'));
          $temp_file = tempnam(file_directory_temp(), 'boost');
          $filepath = file_save_data($attachments->data, $temp_file);
          $zip = zip_open($filepath);
          if (is_resource($zip)) {
            // We got a zip file, let's nuke the (should be just one) low-res attachment.
            // Hopefully we aren't losing any text or anything else of importance here..?
            $node->mailsave_attachments = array();
            while ($zip_entry = zip_read($zip)) {
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
          }
        }
      }
    }
    // There seems to be nothing useful in the body so we nuke it!
    $node->body = '';
    $node->teaser = node_teaser($node->body);
  }
}

/**
 * Report back module information when requested
 */
function mms_us_boost_mailsave_clean_info() {
  $items['mms_us_boost'] = array(
    'provider'    => t('Boost'),
    'country'     => 'US',
    'author'      => 'Mark Burdett',
    'description' => t('Clean up MMS messages sent via the Boost US network.'),
  );
  return $items;
}
