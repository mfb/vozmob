<?php
// $Id: mms_ie_meteor.inc,v 1.1 2007/05/27 23:45:42 stuartgreenfield Exp $

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_ie_meteor_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

  // Build regex to see if this was from O2
  $pattern = '@mms.mymeteor.ie$@';

  // Test message against patten
  if (preg_match($pattern, $header->fromaddress)) {

    // If no title set on this message then set a generic one
    if (!$node->title) {
      $node->title = 'MMS from Meteor Ireland';
    }

    // Empty the body and teaser
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
function mms_ie_meteor_mailsave_clean_info() {
  $items['mms_ie_meteor'] = array(
    'provider'     => t('Meteor'),
    'country'      => 'IE',
    'author'       => 'Stuart Greenfield',
    'description'  => t('Clean up MMS messages sent via the Meteor IE network.'),
  );

  return $items;
}
