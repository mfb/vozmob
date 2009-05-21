<?php
// $Id$

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_us_att_mailsave_clean(&$node, $result, $i, $header, $mailbox) {
  $pattern = '@\.att\.net$@';
  if (preg_match($pattern, $header->fromaddress)) {
    // Remove AT&T spam from message and trim whitespace.
    $node->body = trim(preg_replace('/--.*?This mobile text message is brought to you by AT&T/s', '', $node->body));
    $node->teaser = node_teaser($node->body);

    if (empty($node->title)) {
      // Discard empty messages.
      if (empty($node->body) && empty($node->mailsave_attachments)) {
        $node = FALSE;
      }
      // If there is no body, set a default title.
      elseif (empty($node->body)) {
        $node->title = t('Mobile Submission');
      }
      // If there is a body use it to set a title.
      else {
        $node->title = truncate_utf8($node->body, 60, TRUE, TRUE);
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
