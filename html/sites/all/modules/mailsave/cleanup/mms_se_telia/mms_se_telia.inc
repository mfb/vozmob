<?php
// $Id: mms_se_telia.inc,v 1.2 2007/10/18 21:30:39 stuartgreenfield Exp $

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_se_telia_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

  // Build regex to see if this was from telia
  $pattern = '@mms.telia.com$@';
  if (preg_match($pattern, $header->fromaddress)) {

    // Telia have a weird way of passing the filename
    // We have to extract it from Content-Location and amend the mailsave_attachments array
    $body = imap_body($result, $i);
    preg_match_all('@Content-Location: (.*)\b@', $body, $matches);
    $image_filename = urldecode($matches[1][1]);
    $node->mailsave_attachments[0]['filename'] = $image_filename;

  }

  return;
}


/**
 * Report back module information when requested
 */
function mms_se_telia_mailsave_clean_info() {
  $items['mms_se_telia'] = array(
    'provider'     => t('Telia'),
    'country'      => 'SE',
    'author'       => 'Stuart Greenfield',
    'description'  => t('Clean up MMS messages sent via the Telia Sweden network.'),
  );

  return $items;
}
