<?php
// $Id$

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_ca_fido_mailsave_clean(&$node, $result, $i, $header, $mailbox) {
  if (preg_match('/@fido.ca$/', $header->fromaddress)) {
    if (is_int(strpos($node->body, 'received a picture message from your friend whose Fido'))) {
      $node->body = '';
      $node->teaser = node_teaser($node->body);
      $node->title = t('New story');
    }
  }
}

/**
 * Report back module information when requested.
 */
function mms_ca_fido_mailsave_clean_info() {
  $items['mms_ca_fido'] = array(
    'provider'    => t('Fido'),
    'country'     => 'CA',
    'author'      => 'Mark Burdett',
    'description' => t('Clean up MMS messages sent via the fido.ca network.'),
  );
  return $items;
}
