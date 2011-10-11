<?php
// $Id$

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_us_straight_mailsave_clean(&$node, $result, $i, $header, $mailbox) {
  // Build regex to see if this was from Straight Talk.
  $pattern = '@mypixmessages\.com$@';
  if (preg_match($pattern, $header->fromaddress)) {

    // Build a regex to extract spam from message.
    $node->body = preg_replace('/This picture message .*? select Minimum for faster download\./s', '', $node->body);
    // Message spam may have been cut off at invalid character.
    $node->body = preg_replace('/This picture message .*?To play video messages sent to email, Apple/s', '', $node->body);
    // Strip Verizon spam from message.
    $node->body = preg_replace('/This message has been sent using .* or higher is required\./s', '', $node->body);

    $node->teaser = node_teaser($node->body);

    // Set node title.
    if (empty($node->title)) {
      $node->title = truncate_utf8($node->body, 60, TRUE, TRUE);
    }

    // Add extra text parts into node body.
    foreach ($node->mimeparts as $key => $mimepart) {
      if ($key > 1 && strpos($mimepart->filemime, 'TEXT/') === 0 && strpos($node->body, $mimepart->data) === FALSE) {
        $node->body .= "\n" . $mimepart->data;
      }
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
function mms_us_straight_mailsave_clean_info() {
  $items['mms_us_straight'] = array(
    'provider'    => t('Straight Talk'),
    'country'     => 'US',
    'author'      => 'Mark Burdett',
    'description' => t('Clean up MMS messages sent via the Straight Talk US network.'),
  );
  return $items;
}
