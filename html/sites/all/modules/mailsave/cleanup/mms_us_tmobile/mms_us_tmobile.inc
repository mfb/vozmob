<?php
// $Id$

/**
 * Implementation of hook_mailsave_clean().
 */
function mms_us_tmobile_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

  // Build regex to see if this was from vodafone
  $pattern = '@tmomail.net$@';
  if (preg_match($pattern, $header->fromaddress)) {
  
    // Discard the tmobile logo
    mailsave_discard_attachment($node, 'logo.jpg');
   
    // Discard the dottedline350
    mailsave_discard_attachment($node, 'dottedline350.gif');

    // Discard the dottedline600
    mailsave_discard_attachment($node, 'dottedline600.gif');
 
     // Discard the tmobilelogo
    mailsave_discard_attachment($node, 'tmobilelogo.gif');   
    
    // Discard the tmobilespace
    mailsave_discard_attachment($node, 'tmobilespace.gif');   
  
    // Discard the text file
    mailsave_discard_attachment($node, 'Temp.txt');

      // Get the new body as a string
      $body = $node->body;

      // Strip surplus white space
      $body = preg_replace('@\s\s+@', ' ', $body);
      
      // Strip t mobile text
      $body = preg_replace('@T-Mobile@', ' ', $body);

      // Store the amended body
      $node->body = $body;
      $node->teaser = node_teaser($node->body);

      // Set node title and reset the body if the author just puts short text in the body
      if (empty($node->title) && !empty($node->body) && strlen($node->body) <= 60) {
        $node->title = truncate_utf8($body, 60, TRUE, TRUE);
        $node->body = '';
      }

      // If there is no title and a long body, use body to set the title
      if (empty($node->title) && !empty($node->body)) {
        $node->title = truncate_utf8($body, 60, TRUE, TRUE);
      }

      // If there is no title and no body, create a generic title
      // Maybe this needs to be generic in mailsave or mailsave_to_image?
      if (empty($node->title) && empty($node->body)) {
        $node->title = t('Mobile Submission');
      }

      // Get rid of extra text attachments
      foreach ($node->mailsave_attachments as $key => $file) {
        if ($file['filemime'] == 'text/plain') {
          unset($node->mailsave_attachments[$key]);
        }
      }

    
  }

  return;
}


/**
 * Report back module information when requested
 */
function mms_us_tmobile_mailsave_clean_info() {
  $items['mms_us_tmobile'] = array(
    'provider'     => t('T-Mobile'),
    'country'      => 'US',
    'author'       => 'Stuart Greenfield',
    'description'  => t('Clean up MMS messages sent via the T-Mobile USA network.'),
  );

  return $items;
}