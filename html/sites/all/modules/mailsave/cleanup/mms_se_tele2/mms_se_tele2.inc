<?php
// $Id: mms_se_tele2.inc,v 1.1 2007/10/28 13:45:27 stuartgreenfield Exp $

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_se_tele2_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

  // Build regex to see if this was from tele2
  $pattern = '@mms.tele2.se>$@';
  if (preg_match($pattern, $header->fromaddress)) {

    // Discard the text file
    mailsave_discard_attachment($node, 'main.smil');
    mailsave_discard_attachment($node, 'smil.txt');
 
  }

  return;
}


/**
 * Report back module information when requested
 */
function mms_se_tele2_mailsave_clean_info() {
  $items['mms_se_tele2'] = array(
    'provider'     => t('Tele2'),
    'country'      => 'SE',
    'author'       => 'Jimmy Hedman',
    'description'  => t('Clean up MMS messages sent via the Tele2 Sweden network.'),
  );

  return $items;
}
