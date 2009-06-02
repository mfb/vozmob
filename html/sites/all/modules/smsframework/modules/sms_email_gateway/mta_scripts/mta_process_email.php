#!/usr/bin/php
<?php

// $Id: mta_process_email.php,v 1.1.2.2 2009/04/23 15:34:24 thehunmonkgroup Exp $

/**
 * E-mail processing script for the SMS email gateway module.
 *
 * This script processes an incoming email message, determines the appropriate
 * address code and identifier for the message, and sends the information to
 * the configured Drupal site via XML-RPC.
 *
 * MAIL SERVER SETUP:
 *
 *   1. Place the entire 'mta_scripts' directory from the sms_email_gateway
 *      module somewhere on the mail server, where the mail server has
 *      read/execute access.
 *
 *   2. Set up PHP on the mail server.
 *
 *   3. Make sure the first line of the mta_process_email.php script points to
 *      the location of the PHP executable on the server, and that the script is
 *      executable.
 *
 *   2. Configure the mail server to send the necessary addresses to the script
 *      for processing.
 *
 *      EXAMPLE:
 *
 *      This example is for Postfix.  For other MTA's, you're on your own...  ;)
 *
 *      To pipe any messages matching the regex /^ex\+.*?@example\.com$/ to the
 *      email gateway script:
 *
 *        a. Create a PCRE map file with the following line:
 *             /^ex\+.*?@example\.com$/ ex
 *
 *           Don't forget to add the PCRE map file to the virtual_alias_maps
 *           parameter in main.cf.  If the file is /etc/postfix/virtual_pcre,
 *           then 'virtual_alias_maps = pcre:/etc/postfix/virtual_pcre' would
 *           work.
 *
 *           See http://www.postfix.org/virtual.5.html and
 *           http://www.postfix.org/pcre_table.5.html for help configuring a
 *           PCRE map file.
 *
 *        b. Add the following to /etc/aliases:
 *             ex: |/full/path/to/mta_process_email.php
 *
 *        c. Run newaliases to load the new alias.
 *
 *   3. In the configuration section of mta_process_email.php, configure the
 *      Drupal site to send the results to.
 *
 * It's totally possible to have one mail server processing messages for
 * multiple Drupal sites, by making different copies of the
 * mta_process_email.php script, and mapping the various emails to the different
 * scripts.
 */


/**
 * BEGIN CONFIGURATION.
 */

// The URL of the Drupal site, no trailing slash -- ex. www.example.com
// If you set this parameter to an empty string, then the domain that the email
// is sent to will be used.
define('SITE_URL', '');

// For sites that use SSL, set this to TRUE.
define('USE_SSL', FALSE);

// Set this if you need to send to a custom server port (defaults to port 80).
define('SERVER_PORT', '');

// Set this if you need to send custom query string (useful for debugging).
define('SERVER_QUERY_STRING', '');

// Set to TRUE to turn on script debugging.
define('DEBUG', FALSE);
// File to dump debug info to.
define('DEBUG_FILE', '/tmp/mta_process_email.debug');

// These should not need to be adjusted if you followed the installation
// instructions.
require_once('./xmlrpc.inc');
require_once('./support.inc');

/**
 * END CONFIGURATION.
 */

// Read e-mail from stdin.
$fd = fopen('php://stdin', 'r');
$email = '';
while (!feof($fd)) {
    $email .= fread($fd, 1024);
}
fclose($fd);

if (SITE_URL) {
  $domain = SITE_URL;
}
// No site URL defined, so look for one in the headers.
else {
  // Break up the lines.
  $lines = explode("\n", $email);

  // Loop through the email one line at a time.  As soon as we find the To: line,
  // or the headers end, then stop looking.
  for ($i = 0; $i < count($lines); $i++) {
    if (preg_match('/^To:(.*)/', $lines[$i], $matches)) {
      // For combo name/email addresses, parse out the email address.
      $to = parse_email(trim($matches[1]));
      $to = explode('@', $to);
      if (!empty($to[1])) {
        $domain = $to[1];
      }
      break;
    }
    // Empty line, header section has ended.
    elseif (trim($lines[$i]) == '') {
      break;
    }
  }
}

if (!empty($domain)) {
  // Compose URL.
  $transport = USE_SSL ? 'https://' : 'http://';
  $port = SERVER_PORT ? ':' . SERVER_PORT : '';
  $query = SERVER_QUERY_STRING ? '?' . SERVER_QUERY_STRING : '';
  $url = $transport . $domain . $port . "/xmlrpc.php" . $query;

  $result = xmlrpc($url, 'emailGateway.send', $email);
}



// Debugging section.
if (DEBUG) {
  $debug = "*****************************************************************\n";
  if ($result === 0) {
    $debug .= "Message delivered.\n";
  }
  else {
    $xml_return_output = print_r($result, TRUE);
    $debug .= "XML-RPC Error -- return value follows:\n[start return value]$xml_return_output\n[end return value]\n";
  }
  $debug .= "URL: $url\n";
  $debug .= "Full message:\n$email\n\n\n";
  $fh = fopen(DEBUG_FILE, 'a');
  fwrite($fh, $debug);
  fclose($fh);
}

return $result;

/**
 * Parses out email address from addresses with human-readable names.
 *
 * @param $full_address The full email address.
 * @return The email address.
 */
function parse_email($full_address) {
  if ($left = strpos($full_address, '<')) {
    $right = strpos($full_address, '>');
    $address = substr($full_address, $left + 1, $right - $left - 1);
  }
  else {
    $address = $full_address;
  }

  return $address;
}
