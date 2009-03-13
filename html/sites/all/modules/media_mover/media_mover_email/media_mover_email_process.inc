<?php
// $Id: media_mover_email_process.inc,v 1.14 2008/02/29 16:41:22 bdragon Exp $

/**
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*
* The authors can be contacted at:
*  - arthur [at] civicactions (dot) com
*  - jonathan [at] civicactions (dot) com
*
* @license http://www.affero.org/oagpl.html
* @link    http://civications.com
*/

/**
 * open questions:
 *
 * log files?
 * settings - does media mover already have an admin
 *
 * @param array $file
 * @param array $configuration
 *
 * @return $files
 *
 *
 */

require_once 'media_mover_email_helpers.inc';

function _media_mover_email_harvest($action, $config) {
  // Ensure mail library is loaded.
  require_once 'media_mover_email_pop3.inc';

  $mailserver_uri = $config['mm_email_account_server'];
  $mailserver_port = $config['mm_email_account_port'];
  $mailserver_login = $config['mm_email_account_name'];
  $mailserver_password = $config['mm_email_account_password'];
  $mm_email_delete_emails = $config['mm_email_delete_emails'];

  $mm_email_confirm_send_emails = $config['mm_email_confirm_send_emails'];
  $mm_email_confirm_from = $config['mm_email_confirm_from'];
  $mm_email_confirm_reply_to = $config['mm_email_confirm_reply_to'];
  $mm_email_confirm_subject = $config['mm_email_confirm_subject'];
  $mm_email_confirm_text = $config['mm_email_confirm_text'];

  $mm_email_bounce_send_emails = $config['mm_email_bounce_send_emails'];
  $mm_email_bounce_reply_to = $config['mm_email_bounce_reply_to'];
  $mm_email_bounce_subject = $config['mm_email_bounce_subject'];
  $mm_email_bounce_text = $config['mm_email_bounce_text'];
  $file_types = $config['mm_email_allowed_files'];

  //variable get? or faster way
  $admin_email = '';

  $pop3 = new POP3();
  if (!$pop3->connect($mailserver_uri, $mailserver_port)) {
    watchdog('media_mover', 'media_mover_email error: %error', array('%error' => $pop3->ERROR), WATCHDOG_ERROR);
    return;
  }

  $items = array();

  $count = $pop3->login($mailserver_login, $mailserver_password);
  watchdog('media_mover', '%count message(s) at %mailserver', array('%count' => $count, '%mailserver' => $mailserver_login), WATCHDOG_INFO);

  for ($i = 1; $i <= $count; $i++) {
    $message = $pop3->getstring($i);

    $success = _media_mover_email_process_email($items, $config, $message, $file_types);
    if ($success && $mm_email_delete_emails) {
      if (!$pop3->delete($i)) {
        watchdog('media_mover', 'Email process error: %error', array('%error' => $pop3->ERROR), WATCHDOG_ERROR);
        $pop3->reset();
      }
    }
    if ($success && $mm_email_confirm_send_emails) {
     // @@@ @TODO make this use drupal's mail function drupal_mail!
     mail($success, $mm_email_confirm_subject, $mm_email_confirm_text, "Reply-To: \nFrom: \n");
    }
  }
  $pop3->quit();
  return $items;
}

/**
 * this loops through the message, finds the files.
 */
function _media_mover_email_process_email(&$items, $config, &$message, $file_types) {
  $match = array();
  $content = '';
  $content_type = '';
  $boundary = '';
  $bodysignal = 0;
  $phone_delim = '::';
  $content_types = array();

  $headers = _media_mover_rfc822_headers($message);

  $mime = FALSE;
  if (isset($headers['MIME-Version'])) {
    $mime = $headers['MIME-Version'];
  }
  elseif (isset($headers['Mime-Version'])) {
    $mime = $headers['Mime-Version'];
  }
  if ($mime != '1.0') {
    // Not a mime email...
    // @@@ Do something?
  }

  list($type, $subtype, $params) = _media_mover_content_type($headers);

  // Top level stuff

  // Determine email subject.
  if (isset($headers['Subject'])) {
    // @@@ Make this more flexible..
    $subject = _media_mover_email_process_subject($headers['Subject'], $phone_delim);
  }

  // Determine the sender.
  if (isset($headers['Reply-To'])) {
    $from_email = '';
    $from = _media_mover_email_process_user($headers['Reply-To'], $from_email);
  }
  elseif (isset($headers['From'])) {
    $from_email = '';
    $from = _media_mover_email_process_user($headers['From'], $from_email);
  }

  // Determine the date.
  if (isset($headers['Date'])) {
    // of the form '20 Mar 2002 20:32:37'
    $sent = _media_mover_email_process_date($headers['Date']);
  }

  // @@@ Temporary.
  $content =& $message;

  // Template for all $item[]s generated.
  $template = array(
    'node' => array(
      'title' => $subject,
      'created' => $sent,
      'body' => '', // Init.
      //'id' => md5($subject . $sent . $from_email . $from),
      // @@@ Shouldn't these be in the save as node bailiwick?
      'promote' => 0,
      'moderate' => 0,
      'sticky' => 0,
      'status'=> 0,
    ),
    // @@@ Shouldn't this go in node?
    'nid' => NULL,
    'user' => array(
      'mail' => $from_email,
      'from' => $from,
    ),
  );

  $final_success = TRUE;

  // See RFC 2046, Sec. 5.1.
  if ($type == 'multipart') {
    $boundary = $params['boundary'];
  }

  _media_mover_email_process_email_recurse($message, $headers, $items, $body_parts, $template, $config, $file_types, $final_success);

  // Let's see if we can find a body to use.
  if (isset($body_parts['html'])) {
    // We got an html one? Great!
    foreach ($items as $key => $v) {
      $items[$key]['data']['body'] = $body_parts['html'];
    }
  }
  elseif (isset($body_parts['plain'])) {
    // We got a text one? Sounds ok!
    foreach ($items as $key => $v) {
      $items[$key]['data']['body'] = $body_parts['plain'];
    }
  }

  // Allow for custom code.
  $code = trim($config['mm_email_custom_parser_php']);
  if ($code) {
    ob_start();
    // Not using drupal_eval because we *want* to enable side effects.
    eval($code);
    ob_end_clean();
  }

  return $final_success;
}

function _media_mover_email_process_email_recurse(&$message, $headers, &$items, &$body_parts, $template, $config, $file_types, &$final_success) {
  list($type, $subtype, $params) = _media_mover_content_type($headers);

  if ($type == 'multipart') {
    // This section is multipart.
    // See RFC 2046, Sec. 5.1.
    $boundary = $params['boundary'];

    // Discard up to first boundary start.
    $message = substr($message, strpos($message, "\r\n--$boundary\r\n"));
    // Read to end of multipart into $mypart.
    list($myparts, $message) = explode("\r\n--${boundary}--\r\n", $message);
    // Split mypart into sections.
    $myparts = explode("\r\n--$boundary\r\n", $myparts);
    foreach ($myparts as $section) {
      $myheaders = _media_mover_rfc822_headers($section);
      // Recurse.
      _media_mover_email_process_email_recurse($section, $myheaders, $items, $body_parts, $template, $config, $file_types, $final_success);
    }
  }
  else {
    // This section is not multipart.
    // @@@ Code the general case for header match!
    if (isset($headers['Content-Transfer-Encoding'])) {
      if ($headers['Content-Transfer-Encoding'] == 'base64') {
        // Strip newlines.
        $message = str_replace("\r", '', $message);
        $message = str_replace("\n", '', $message);
        //$message = str_replace(' ', '', $message);
        $tmp = base64_decode($message);
        $message = $tmp;
      }
    }
    if ($type == 'text') {
      // Save this section for later.
      $body_parts[$subtype] = $message;
    }
    else {
      list($disp_type, $dparams) = _media_mover_content_disposition($headers);
      $filename = NULL;
      if (isset($dparams['filename'])) {
        // Content-Disposition filename.
        $filename = $dparams['filename'];
      }
      elseif (isset($params['name'])) {
        // Content-Type name.
        $filename = $params['name'];
      }
      if ($filename) {
        $item = $template; // Start an item based on the template.
        $item['filemime'] = $type .'/'. $subtype;
        $success = _media_mover_email_create_file($item, $message, $filename, $file_types);
        if (isset($item['filesize'])) {
          $items[] = $item;
        }
        //track that these files were successfully created
        $final_success = $success && $final_success;
      }
      else {
/*
        // Filename was not specified.
        $tmpfile = tempnam(realpath(file_directory_temp()), 'file');
        $fp = fopen($tmpfile, 'wb');
        fwrite($fp, $segment);
        fclose($fp);
        watchdog('media_mover', 'Unable to parse filename from email! Segment data saved to %file', array('%file' => $tmpfile), WATCHDOG_WARNING);
*/
      }
    }
  }
}

/**
 * Save a file from an email.
 */
function _media_mover_email_create_file(&$file_array, &$data, $filename, $types) {
  // @@@ Drupal file routines?
  // get file path from media mover.
  $file_location = media_mover_api_dir_path() . MM_EMAIL_ATTACHMENTS_SUBDIRECTORY;
  // build the file
  $the_file = file_create_filename($filename, $file_location);

  $fh = @fopen($the_file, 'wb');
  @fwrite($fh, $data);
  @fclose($fh);

  $file_array['filesize'] = filesize($the_file) | 0;

  // build the list of file types
  $file_types = explode(' ', strtolower($types));

  if ($success = _media_mover_email_security($the_file, $file_types)) {
    watchdog('media_mover', 'Email attachment %file harvested.', array('%file' => $the_file), WATCHDOG_INFO);
  }

  // append the file to the file array
  $file_array['filename'] = $the_file;

  // set the harvested file path
  $file_array['filepath'] =  $the_file;
  $file_array['harvest']['file'] = $file_array['filepath'];

  return $success;
}
