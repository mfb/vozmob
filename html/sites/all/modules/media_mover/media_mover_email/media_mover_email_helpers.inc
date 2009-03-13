<?php
// $Id: media_mover_email_helpers.inc,v 1.8 2008/02/29 16:41:22 bdragon Exp $

/*
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

function _media_mover_email_process_date($line) {
  //$time_difference = get_settings('gmt_offset') * 3600;
  $time_difference = 0;

  $dmonths = array(
    'Jan',
    'Feb',
    'Mar',
    'Apr',
    'May',
    'Jun',
    'Jul',
    'Aug',
    'Sep',
    'Oct',
    'Nov',
    'Dec'
  );

  $ddate = trim($line);
  if (strpos($ddate, ',')) {
    $ddate = trim(substr($ddate, strpos($ddate, ',') + 1, strlen($ddate)));
  }
  $date_arr = explode(' ', $ddate);
  $date_time = explode(':', $date_arr[3]);

  $ddate_H = $date_time[0];
  $ddate_i = $date_time[1];
  $ddate_s = $date_time[2];

  $ddate_m = $date_arr[1];
  $ddate_d = $date_arr[0];
  $ddate_Y = $date_arr[2];
  for ($j = 0; $j < 12; $j++) {
    if ($ddate_m == $dmonths[$j]) {
      $ddate_m = $j +1;
    }
  }

  $time_zn = intval($date_arr[4]) * 36;
  $ddate_U = gmmktime($ddate_H, $ddate_i, $ddate_s, $ddate_m, $ddate_d, $ddate_Y);
  $ddate_U = $ddate_U - $time_zn;
  $post_date = gmdate('Y-m-d H:i:s', $ddate_U + $time_difference);
  $post_date_gmt = gmdate('Y-m-d H:i:s', $ddate_U);

  return $post_date_gmt;
}

/**
 * Process the subject line of an email.
 */
function _media_mover_email_process_subject($line, $phone_delim) {
  $subject = trim($line);
  if (!preg_match('#\=\?(.+)\?Q\?(.+)\?\=#i', $subject)) {
    //$subject = wp_iso_descrambler($subject);
  }
  // Captures any text in the subject before $phone_delim as the subject
  $subject = explode($phone_delim, $subject);
  return $subject[0];
}

/**
 * Process the Reply-To or From line of an email.
 */
function _media_mover_email_process_user($line, &$from_email) {
  // Set the author using the email address (To or Reply-To, the last used)
  // otherwise use the site admin
  // SDM: added slash to beginning to 2nd preg_match

  $author = trim($line);
  if (ereg("([a-zA-Z0-9\_\-\.]+@[\a-zA-z0-9\_\-\.]+)", $author, $regs)) {
//    $logfile .= "Author = {$regs[1]} \r\n";
  }
  $from_email = $regs[0];

  return trim($author);
}

/**
 * provide a minimal set of security
 * (assume this is safer on linux)
 */
function _media_mover_email_security($the_file, $allowed_extentions = null) {

  // is this sufficient?
  //@ chmod($the_file, 0600);
  if ($allowed_extentions === null) $allowed_extentions = array();

  preg_match('/\.\w+$/', $the_file, $extension);
  $the_extention = (isset($extension[0])) ? str_replace('.', '', $extension[0]) :'';
  $the_extention = strtolower($the_extention);

  if (!in_array($the_extention, $allowed_extentions)) {
    //del file.
    @unlink($the_file);
    watchdog('media_mover', 'media_mover_email: Forbidden extension %ext detected. %file removed.', array('%ext' => $the_extention, '%file' => $the_file), WATCHDOG_WARNING);
    return false;
  }
  else {
     // file is ok, set file perms
     media_mover_api_set_file_perm($the_file);
    return true;
  }
}

/**
 * Get RFC822 headers of a segment.
 */
function _media_mover_rfc822_headers(&$segment) {
  // Headers end at first "blank" line.
  $division = strpos($segment, "\r\n\r\n");
  $temp = substr($segment, 0, $division);
  // Strip headers from segment.
  $segment = substr($segment, $division+4);
  $headers = array();
  $temp = explode("\x0d\x0a", $temp);
  // Walk backwards through the headers and unfold them.
  for ($n = count($temp) - 1; $n > 0; $n--) {
    if ($temp[$n][0] == ' ' || $temp[$n][0] == "\t") {
      // Folded header. Join with previous.
      $temp[$n-1] .= $temp[$n];
      unset($temp[$n]);
    }
  }
  // Walk forwards and split into k/v pairs.
  foreach ($temp as $header) {
    if (strpos($header, ':') === FALSE) {
      continue;
    }
    list($k, $v) = explode(': ', $header, 2);
    // Duplicate header?
    if (isset($headers[$k])) {
      if (!is_array($headers[$k])) {
        $headers[$k] = array($headers[$k]);
      }
      $headers[$k][] = $v;
    }
    else {
      $headers[$k] = $v;
    }
  }
  // Defaults.
  if (!isset($headers['Content-Type'])) {
    $headers['Content-Type'] = 'text/plain; charset=US-ASCII';
  }

  // @Note: RFC822 and 2822 do not say header keys are case insensitive.
  // So, I'm going to assume here that they are in fact case sensitive.
  // If this ends up being problematic, we can normalize headers at this point.
  return $headers;
}

/**
 * Parse the content type header.
 * @return An array containing the type, the subtype, and a keyed array of parameters.
 */
function _media_mover_content_type($headers) {
  $t = array();
  // @@@ Fix character classes to be in line with RFC 822/2822/2045.
  preg_match('{^([\w\.]+)/([\w\.]+)(?:; )?(.*)}', $headers['Content-Type'], $t);
  $tt = explode('; ', $t[3]);
  $params = array();
  // Parse out the params.
  foreach ($tt as $param) {
    $ttt = array();
    preg_match('/([^=]+)=("?)(.*)\2/', $param, $ttt);
    if (!empty($ttt)) {
      $params[strtolower($ttt[1])] = isset($ttt[3]) ? $ttt[3] : TRUE;
    }
  }
  return array(strtolower($t[1]), strtolower($t[2]), $params);
}

/**
 * Parse the content disposition header (if available.)
 * @return An array containing the disposition-type and a keyed array of parameters.
 */
function _media_mover_content_disposition($headers) {
  // See RFC2183 for a description of Content-Disposition.
  $t = array();
  // @@@ Fix character classes to be in line with RFC 822/2822/2045.
  preg_match('{^([\w\.]+)(?:; )?(.*)}', $headers['Content-Disposition'], $t);
  $tt = explode('; ', $t[2]);
  $params = array();
  // Parse out the params.
  foreach ($tt as $param) {
    $ttt = array();
    preg_match('/([^=]+)=("?)(.*)\2/', $param, $ttt);
    $params[strtolower($ttt[1])] = $ttt[3];
  }
  return array(strtolower($t[1]), $params);
}
