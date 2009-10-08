<?php
// $Id$

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_fr_orange_mailsave_clean(&$node, $result, $i, $header, $mailbox) {
  if (preg_match('/@orange.fr$/', $header->fromaddress)) {
    // Attachments are in table cells.
    preg_match_all('|<TD align="left">(.*?)</TD>|s', $node->body, $matches);
    if ($matches[1]) {
      $node->body = '';
      foreach ($matches[1] as $match) {
        // Non-text attachments contain an image tag.
        if (strpos($match, '<img src="') === FALSE) {
          $node->body .= $match;
        }
      }
      $node->teaser = node_teaser($node->body);
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
 * Report back module information when requested.
 */
function mms_fr_orange_mailsave_clean_info() {
  $items['mms_fr_orange'] = array(
    'provider'    => t('Orange'),
    'country'     => 'FR',
    'author'      => 'Mark Burdett',
    'description' => t('Clean up MMS messages sent via the orange.fr network.'),
  );
  return $items;
}
