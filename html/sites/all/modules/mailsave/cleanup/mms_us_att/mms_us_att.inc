<?php
// $Id$

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_us_att_mailsave_clean(&$node, $result, $i, $header, $mailbox) {
  $pattern = '@\.att\.net$@';
  if (preg_match($pattern, $header->fromaddress)) {
    if (empty($node->title)) {
      $node->title = t('New story');
    }
    // Remove AT&T spam from message and trim whitespace.
    $node->body = trim(preg_replace('/--.*?This mobile text message is brought to you by AT&T/s', '', $node->body));
    // Remove HTML <title>.
    $node->body = preg_replace('@<title>.*</title>@', '', $node->body);
    $node->teaser = node_teaser($node->body);

    // Discard empty messages.
    if (empty($node->title) && empty($node->body) && empty($node->mailsave_attachments)) {
      $node = FALSE;
    }

    // Add extra text parts into node body.
    foreach ($node->mimeparts as $key => $mimepart) {
      if ($key > 1 && strpos($mimepart->filemime, 'TEXT/') === 0 && $mimepart->filemime != 'TEXT/X-VCARD' && strpos($node->body, $mimepart->data) === FALSE) {
        $node->body .= "\n" . $mimepart->data;
      }
    }
  }
}

/**
 * Report back module information when requested.
 */
function mms_us_att_mailsave_clean_info() {
  return array('mms_us_att' => array(
    'provider'    => t('AT&amp;T'),
    'country'     => 'US',
    'author'      => 'Mark Burdett',
    'description' => t('Clean up MMS messages sent via the AT&amp;T US network.'),
  ));
}
