<?php
// $Id$

/**
 * @file
 * Cleanup messages received from Gizmo.
 */

/**
 * Implementation of hook_mailsave_clean().
 */
function msg_format_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

    $node->teaser = node_teaser($node->body);

    if (empty($node->title) && empty($node->body)) {
      $node->title = t('New Story');
    }
    // Set node title and reset the body if the author just puts short text in the body
    if (empty($node->title) && !empty($node->body) && strlen($node->body) <= 60) {
      $node->title = urldecode(truncate_utf8($body, 60, TRUE, TRUE));
      $node->body = '';
    }

    // If there is no title and a long body, use body to set the title
    if (empty($node->title) && !empty($node->body)) {
      $node->title = urldecode(truncate_utf8($body, 60, TRUE, TRUE));
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
