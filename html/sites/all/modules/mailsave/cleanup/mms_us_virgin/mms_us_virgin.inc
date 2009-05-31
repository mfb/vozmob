<?php
// $Id$

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_us_virgin_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

  // Build regex to see if this was from virgin mobile
  $pattern = '@vmpix.com$@';

  if (preg_match($pattern, $header->fromaddress)) {

    watchdog('debug', 'inside the virgin mobile message');
     // Build a regex to extract message body
    // This regex identifies the actual message text in the email
    $end_pattern = preg_quote('This message is from a Virgin Mobile user. Enjoy.');
    $pattern = '@^(.*)' . $end_pattern . '@Us';

    // Check for regex success in case the provider changes their format
    // Worst case is that match fails and nothing is changed
    if (preg_match($pattern, $node->body)) {

    // Find the HTML element containing user-supplied text.
      preg_match_all('!<p>(.*?)<p></center></body></html>!s', $node->body, $matches);
      // Get the new body as a string
      $body = $matches[1][1];

      // Store the amended body
      $node->body = $body;
      $node->teaser = node_teaser($node->body);

    watchdog('debug', print_r($body, TRUE));
    }

    // Set node title and reset the body if the author just puts short text in the body
    if (empty($node->title) && !empty($node->body) && strlen($node->body) <= 60) {
      $node->title = truncate_utf8(strip_tags($body), 60, TRUE, TRUE);
      $node->body = '';
    }

    // Get rid of extra text attachments
    foreach ($node->mailsave_attachments as $key => $file) {
      if ($file['filemime'] == 'text/plain') {
        unset($node->mailsave_attachments[$key]);
      }
    }
  }
}


/**
 * Report back module information when requested
 */
function mms_us_virgin_mailsave_clean_info() {
  $items['mms_us_tmobile'] = array(
    'provider'     => t('Virgin Mobile'),
    'country'      => 'US',
    'author'       => 'Gabriela Rodriguez',
    'description'  => t('Clean up MMS messages sent via the Virgin Mobile USA network.'),
  );

  return $items;
}
