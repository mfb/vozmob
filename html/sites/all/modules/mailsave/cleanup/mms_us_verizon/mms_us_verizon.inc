<?php
// $Id: mms_us_verizon.inc,v 1.2 2007/10/28 13:36:08 stuartgreenfield Exp $

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_us_verizon_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

  // Build regex to see if this was from vodafone
  $pattern = '@vzwpix.com$@';
  if (preg_match($pattern, $header->fromaddress)) {

    // Build a regex to extract message body
    // This regex identifies the actual message text in the email
    $end_pattern = preg_quote('This message was sent using PIX-FLIX Messaging service from Verizon Wireless!');
    $pattern = '@^(.*)' . $end_pattern . '@Us';

    // Check for regex success in case the provider changes their format
    // Worst case is that match fails and nothing is changed
    if (preg_match($pattern, $node->body, $matches)) {

      // Get the new body as a string
      $body = $matches[1];

      // Strip surplus white space
      $body = preg_replace('@\s\s+@', ' ', $body);

      // Store the amended body
      $node->body = $body;
      $node->teaser = node_teaser($node->body);

      // Set node title and reset the body if the author just puts short text in the body
      if (empty($node->title) && !empty($node->body) && strlen($node->body) <= 60) {
        $node->title = truncate_utf8($body, 60, TRUE, TRUE);
        $node->body = '';
      }

      // If there is no title and a long body, use body to set the title
      if (empty($node->title) && !empty($node->body)) {
        $node->title = truncate_utf8($body, 60, TRUE, TRUE);
      }

      // If there is no title and no body, create a generic title
      // Maybe this needs to be generic in mailsave or mailsave_to_image?
      if (empty($node->title) && empty($node->body)) {
        $node->title = t('Mobile Submission');
      }

      // Get rid of extra text attachments
      foreach ($node->mailsave_attachments as $key => $file) {
        if ($file['filemime'] == 'text/plain') {
          unset($node->mailsave_attachments[$key]);
        }
      }

    }
  }

  return;
}


/**
 * Report back module information when requested
 */
function mms_us_verizon_mailsave_clean_info() {
  $items['mms_us_verizon'] = array(
    'provider'    => t('Verizon'),
    'country'     => 'US',
    'author'      => 'Stuart Greenfield',
    'description' => t('Clean up MMS messages sent via the Verizon US network.'),
  );

  return $items;
}
