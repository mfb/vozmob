<?php
// $Id: mms_za_vodacom.inc,v 1.2 2007/10/18 21:06:33 stuartgreenfield Exp $

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_za_vodacom_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

  // Build regex to see if this was from vodafone
  $pattern = '@mms.vodacom4me.co.za>$@';
  if (preg_match($pattern, $header->fromaddress)) {

    // Build a regex to extract message body
    // This regex identifies the actual message text in the email
    $start_pattern = '\<img src\=\"cid.*\<p\>';
    $end_pattern = preg_quote('<p>');
    $pattern = '@' . $start_pattern . '(.*)' . $end_pattern . '@Us';

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

      // Replace generic title with portion of body
      $node->title = truncate_utf8($body, 60, TRUE, TRUE);
    }
  }

  return;
}


/**
 * Report back module information when requested
 */
function mms_za_vodacom_mailsave_clean_info() {
  $items['mms_za_vodacom'] = array(
    'provider'    => t('Vodacom'),
    'country'     => 'ZA',
    'author'      => 'Stuart Greenfield',
    'description' => t('Clean up MMS messages sent via the Vodacom ZA network.'),
  );

  return $items;
}
