<?php
// $Id: mms_us_metropcs.inc,v 1.2 2007/10/28 13:36:08 stuartgreenfield Exp $

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_us_metropcs_mailsave_clean(&$node, $result, $i, $header, $mailbox) {
  // Build regex to see if this was from MetroPCS.
  $pattern = '@mymetropcs\.com$@';
  if (preg_match($pattern, $header->fromaddress)) {

    // Remove MetroPCS spam.
    $node->body = str_replace('This message was sent using picture-talk messaging service from MetroPCS.', '', $node->body);

    // Find the HTML elements containing user-supplied text.
    preg_match_all('!<td colspan=1 align="Center">(.*?)</td>!s', $node->body, $matches);

    // Get the new body as a string
    $body = '';
    foreach ($matches[1] as $cell) {
      // Do not include links to attachments in the node body.
      if (substr($cell, 0, 8) != '<a href=') {
        $body .= $cell;
      }
    }
    $node->body = $body;

    // Remove IMG elements from body.
    $node->body = preg_replace('/<IMG.*?>/', '', $node->body);

    // Remove the default title.
    $node->body = str_replace('<title>New Page 1</title>', '', $node->body);

    $node->teaser = node_teaser($node->body);

    // Rebuild the title already built by Mailhandler from the original body.
    if (strpos($node->title, 'New Page 1') === 0) {
      $node->title = truncate_utf8(decode_entities(strip_tags($node->body)), 60, TRUE, TRUE);
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
 * Report back module information when requested
 */
function mms_us_metropcs_mailsave_clean_info() {
  $items['mms_us_metropcs'] = array(
    'provider'    => t('MetroPCS'),
    'country'     => 'US',
    'author'      => 'Mark Burdett',
    'description' => t('Clean up MMS messages sent via the MetroPCS US network.'),
  );
  return $items;
}
