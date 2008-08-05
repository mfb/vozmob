<?php
// $Id: mms_uk_o2.inc,v 1.1 2007/05/27 23:45:40 stuartgreenfield Exp $

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_uk_o2_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

  // Build regex to see if this was from O2
  $pattern = '@mediamessaging.o2.co.uk$@';

  // Test message against pattern
  if (preg_match($pattern, $header->fromaddress)) {

    // If no title set on this message then set a generic one
    if (!$node->title) {
      $node->title = 'MMS from O2 UK';
    }

    // Empty the body and teaser in case there is no body (this discards O2 junk)
    $node->body = '';
    $node->teaser = '';

    // Look for body attachment on this node (Txt*.txt)
    foreach ($node->mailsave_attachments as $key => $attachment) {

      // Intercept the first attachment in text format
      if ($attachment['filemime'] == 'text/plain') {

        // We already saved the file, so read it back in to the body and set teaser
        $node->body = (file_get_contents($attachment['filepath']));
        $node->teaser = node_teaser($node->body);

        // We don't want to save this attachment now, so discard it by unsetting
        // It is saved as a temporary file so it will be purged upon completion
        unset($node->mailsave_attachments[$key]);

        // Don't continue through this loop any further
        break;
      }
    }
  }

  return;
}


/**
 * Report back module information when requested
 */
function mms_uk_o2_mailsave_clean_info() {
  $items['mms_uk_o2'] = array(
    'provider'     => t('O2'),
    'country'      => 'UK',
    'author'       => 'Stuart Greenfield',
    'description'  => t('Clean up MMS messages sent via the O2 UK network.'),
  );

  return $items;
}
