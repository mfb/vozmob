<?php
// $Id$

/**
 * @file
 * Cleanup messages received from miscellaneous sources.
 */

/**
 * Implementation of hook_mailsave_clean().
 */
function msg_format_mailsave_clean(&$node, $result, $i, $header, $mailbox) {
  // Strip <title> tag.
  $node->body = preg_replace('@<title>.*</title>@', '', $node->body);
  $node->teaser = node_teaser($node->body);
  // Set node title.
  if (empty($node->title)) {
    $node->title = truncate_utf8(decode_entities(strip_tags($node->body)), 60, TRUE, TRUE);
  }
  if (empty($node->title)) {
    $node->title = t('New story');
  }
}

function msg_format_mailsave_clean_info() {
  $items['msg_format'] = array(
    'provider'    => t('Formating the message'),
    'country'     => 'US',
    'author'      => 'Gabriela Rodriguez',
    'description' => t('Clean up MMS messages.'),
  );
  return $items;
}
