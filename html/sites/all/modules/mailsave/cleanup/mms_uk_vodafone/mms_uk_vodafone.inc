<?php
// $Id: mms_uk_vodafone.inc,v 1.1 2007/05/27 23:45:40 stuartgreenfield Exp $

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_uk_vodafone_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

  // Build regex to see if this was from vodafone
  $pattern = '@mms.vodafone.co.uk>$@';
  if (preg_match($pattern, $header->fromaddress)) {

    // Discard the vodafone logo
    mailsave_discard_attachment($node, 'vodafone.gif');

    // Build a regex to extract message body
    // This regex identifies the actual message text in the email
    $start_pattern = preg_quote('<font size="2" color="#404040">');
    $end_pattern = preg_quote('</font>');
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
function mms_uk_vodafone_mailsave_clean_info() {
  $items['mms_uk_vodafone'] = array(
    'provider'     => t('Vodafone'),
    'country'      => 'UK',
    'author'       => 'Stuart Greenfield',
    'description'  => t('Clean up MMS messages sent via the Vodafone UK network.'),
  );

  return $items;
}
