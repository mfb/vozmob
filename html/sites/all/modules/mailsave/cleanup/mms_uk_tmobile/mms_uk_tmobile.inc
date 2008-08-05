<?php
// $Id: mms_uk_tmobile.inc,v 1.1 2007/05/27 23:45:41 stuartgreenfield Exp $

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_uk_tmobile_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

  // Build regex to see if this was from tmobile
  $pattern = '@mmsreply.t-mobile.co.uk>$@';
  if (preg_match($pattern, $header->fromaddress)) {

    // Discard the tmobile logo
    mailsave_discard_attachment($node, 'logo.jpg');

    // Discard the text file
    mailsave_discard_attachment($node, 'Temp.txt');

    // Build a regex to extract message body
    // This regex identifies the actual message text in the email
    $start_pattern = '\+[0-9]*\<p\>';
    $end_pattern = '\<\/p\>';
    $pattern = '@' . $start_pattern . '(.*)' . $end_pattern . '@Us';

    // Check for regex success, if it fails discard the content as it reveals
    // the phone number of the user
    if (preg_match($pattern, $node->body, $matches)) {

      // Get the new body as a string
      $body = $matches[1];

      // Strip surplus white space
      $body = preg_replace('@\s\s+@', ' ', $body);

      // Store the amended body
      $node->body = $body;
      $node->teaser = node_teaser($node->body);

    }
    else {
      $node->body = '';
      $node -> teaser = '';
    }

  }

  return;
}


/**
 * Report back module information when requested
 */
function mms_uk_tmobile_mailsave_clean_info() {
  $items['mms_uk_tmobile'] = array(
    'provider'     => t('T-Mobile'),
    'country'      => 'UK',
    'author'       => 'Stuart Greenfield',
    'description'  => t('Clean up MMS messages sent via the T-Mobile UK network.'),
  );

  return $items;
}
