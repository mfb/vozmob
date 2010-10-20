<?php


/**
 * Establish IMAP stream connection to specified mailbox.
 * 
 * @param $mailbox
 *   Array of mailbox configuration
 * @return IMAP stream
 */
function mailhandler_open_mailbox($mailbox) {
  if ($mailbox['domain']) {
    if ($mailbox['imap'] == 1) {
      $box = '{'. $mailbox['domain'] .':'. $mailbox['port'] . $mailbox['extraimap'] .'}'. $mailbox['folder'];
    }
    else {
      $box = '{'. $mailbox['domain'] .':'. $mailbox['port'] .'/pop3'. $mailbox['extraimap'] .'}'. $mailbox['folder'];
    }
    $result = imap_open($box, $mailbox['name'], $mailbox['pass']);
    $err = 'domain';
  }
  else {
    $box = $mailbox['folder'];
    $result = imap_open($box, '', '');
  }
  return $result;
}

/**
 * Returns the first part with the specified mime_type
 *
 * USAGE EXAMPLES - from php manual: imap_fetch_structure() comments
 * $data = get_part($stream, $msg_number, "TEXT/PLAIN"); // get plain text
 * $data = get_part($stream, $msg_number, "TEXT/HTML"); // get HTML text
 */
function mailhandler_get_part($stream, $msg_number, $mime_type, $structure = false, $part_number = false) {

  if (!$structure) {
    $structure = imap_fetchstructure($stream, $msg_number, FT_UID);
  }
  if ($structure) {
    $encoding = variable_get('mailhandler_default_encoding', 'UTF-8');
    foreach ($structure->parameters as $parameter) {
      if (strtoupper($parameter->attribute) == 'CHARSET') {
        $encoding = $parameter->value;
      }
    }
    if ($mime_type == mailhandler_get_mime_type($structure)) {
      if (!$part_number) {
        $part_number = '1';
      }
      $text = imap_fetchbody($stream, $msg_number, $part_number, FT_UID);
      if ($structure->encoding == ENCBASE64) {
        return drupal_convert_to_utf8(imap_base64($text), $encoding);
      }
      else if ($structure->encoding == ENCQUOTEDPRINTABLE) {
        return drupal_convert_to_utf8(quoted_printable_decode($text), $encoding);
      }
      else {
        return drupal_convert_to_utf8($text, $encoding);
      }
    }
    if ($structure->type == TYPEMULTIPART) { /* multipart */
      $prefix = '';
      while (list($index, $sub_structure) = each ($structure->parts)) {
        if ($part_number) {
          $prefix = $part_number .'.';
        }
        $data = mailhandler_get_part($stream, $msg_number, $mime_type, $sub_structure, $prefix . ($index + 1));
        if ($data) {
          return $data;
        }
      }
    }
  }

  return false;
}


/**
 * Returns an array of parts as file objects
 *
 * @param
 * @param $structure
 *   A message structure, usually used to recurse into specific parts
 * @param $max_depth
 *   Maximum Depth to recurse into parts.
 * @param $depth
 *   The current recursion depth.
 * @param $part_number
 *   A message part number to track position in a message during recursion.
 * @return
 *   An array of file objects.
 */
function mailhandler_get_parts($stream, $msg_number, $max_depth = 10, $depth = 0, $structure = FALSE, $part_number = FALSE) {
  $parts = array();

  // Load Structure.
  if (!$structure && !$structure = imap_fetchstructure($stream, $msg_number, FT_UID)) {
    watchdog('mailhandler', 'Could not fetch structure for message number %msg_number', array('%msg_number' => $msg_number), WATCHDOG_NOTICE);
    return $parts;
  }

  // Recurse into multipart messages.
  if ($structure->type == TYPEMULTIPART) {
    // Restrict recursion depth.
    if ($depth >= $max_depth) {
      watchdog('mailhandler', 'Maximum recursion depths met in mailhander_get_structure_part for message number %msg_number.',  array('%msg_number' => $msg_number), WATCHDOG_NOTICE);
      return $parts;
    }
    $prefix = '';
    foreach($structure->parts as $index => $sub_structure) {
      // If a part number was passed in and we are a multitype message, prefix the
      // the part number for the recursive call to match the imap4 dot seperated part indexing.
      if ($part_number) {
        $prefix = $part_number .'.';
      }
      $sub_parts =  mailhandler_get_parts($stream, $msg_number, $max_depth, $depth + 1,
        $sub_structure, $prefix . ($index + 1));
      $parts = array_merge($parts, $sub_parts);
    }
    return $parts;
  }

  // Per Part Parsing.

  // Initalize file object like part structure.
  $part = new StdClass();
  $part->attributes = array();
  $part->filename = 'unnamed_attachment';
  if (!$part->filemime = mailhandler_get_mime_type($structure)) {
    watchdog('mailhandler', 'Could not fetch mime type for message part. Defaulting to application/octet-stream.', array(), WATCHDOG_NOTICE);
    $part->filemime = 'application/octet-stream';
  }

  if ($structure->ifparameters) {
    foreach ($structure->parameters as $parameter) {
      switch (strtoupper($parameter->attribute)) {
        case 'NAME':
        case 'FILENAME':
          $part->filename = $parameter->value;
          break;
        default:
          // put every thing else in the attributes array;
          $part->attributes[$parameter->attribute] = $parameter->value;
      }
    }
  }

  // Handle Content-Disposition parameters for non-text types.
  if ($structure->type != TYPETEXT && $structure->ifdparameters) {
    foreach ($structure->dparameters as $parameter) {
      switch (strtoupper($parameter->attribute)) {
        case 'NAME':
        case 'FILENAME':
          $part->filename = $parameter->value;
          break;
        // put every thing else in the attributes array;
        default:
          $part->attributes[$parameter->attribute] = $parameter->value;
      }
    }
  }

  // Retrieve part and convert MIME encoding to UTF-8
  if(!$part->data = imap_fetchbody($stream, $msg_number, $part_number, FT_UID)) {
    watchdog('mailhandler', 'No Data!!', array(), WATCHDOG_ERROR);
    return $parts;
  }

  // Decode as necessary.
  if ($structure->encoding == ENCBASE64) {
    $part->data = imap_base64($part->data);
  }
  elseif ($structure->encoding == ENCQUOTEDPRINTABLE) {
    $part->data = quoted_printable_decode($part->data);
  }
  // Convert text attachment to UTF-8.
  elseif ($structure->type == TYPETEXT) {
    $part->data = imap_utf8($part->data);
  }

  //always return an array to satisfy array_merge in recursion catch, and array return value.
  $parts[] = $part;
  return $parts;
}


/**
 * Retrieve MIME type of the message structure.
 */
function mailhandler_get_mime_type(&$structure) {
  static $primary_mime_type = array('TEXT', 'MULTIPART', 'MESSAGE', 'APPLICATION', 'AUDIO', 'IMAGE', 'VIDEO', 'OTHER');
  $type_id = (int)$structure->type;
  if (isset($primary_mime_type[$type_id]) && !empty($structure->subtype)) {
    return $primary_mime_type[$type_id] .'/'. $structure->subtype;
  }
  return 'TEXT/PLAIN';
}

function mailhandler_commands_parse($body, $sep) {
  $commands = array();
  // Collect the commands and locate signature
  $lines = explode("\n", $body);
  for ($i = 0; $i < count($lines); $i++) {
    $line = trim($lines[$i]);
    $words = explode(' ', $line);
    // Look for a command line. if not present, note which line number is the boundary
    if (substr($words[0], -1) == ':' && !isset($endcommands)) {
      // Looks like a name: value pair
      $commands[$i] = explode(': ', $line, 2);
    }
    else {
      if (!isset($endcommands)) $endcommands = $i;
    }
    // Stop when we encounter the sig. we'll discard all remaining text.
    $start = substr($lines[$i], 0, strlen($sep)+3);
    if ($sep && strstr($start, $sep)) { // mail clients sometimes prefix replies with ' >'
      break;
    }
  }
  return array('commands' => $commands, 'lines' => $lines, 'i' => $i, 'endcommands' => $endcommands);
}

/**
 * Defines and executes message authentication methods
 * 
 * Message authentication methods can be defined using mailhandler_authenticate_info() which can 
 * take one of two $op's:
 *   - info, which is used to define an authentication plugin
 *   - execute, which is used to execute an authentication plugin
 * 
 * @param $op
 *   String info or execute
 * @param $name
 *   String identifier for authentication method
 * @param $args
 *   Array of arguments to pass in to the authentication method callback
 */
function mailhandler_mailhandler_authenticate($op, $name = NULL, $args = array()) {
  $methods = array();
  switch ($op) {
    case 'info':
      foreach (module_list() as $module) {
        if (module_hook($module, 'mailhandler_authenticate_info')) {
          $function = $module .'_mailhandler_authenticate_info';
          $methods[] = $function();
        }
      }
      return $methods;
    case 'execute':
      foreach (module_list() as $module) {
        if (module_hook($module, 'mailhandler_authenticate_info')) {
          $function = $module .'_mailhandler_authenticate_info';
          $methods = $function('info');
          // TODO - May not be found, like if providing module is disabled.  Must fail gracefully
          foreach ($methods as $key => $method) {
            if ($name == $key) {
              if ($method['extension'] && $method['basename']) {
                module_load_include($method['extension'], $method['module'], $method['basename']);
                return call_user_func_array($method['callback'], $args);
              }
              else {
                drupal_load('module', $method['module']);
                return call_user_func_array($method['callback'], $args);
              }
              break 2;
            }
          }
        }
      }
      // Return FALSE if callback is not found.
      return FALSE;
      break;
  }
}

/**
 * Switch from original user to mail submision user and back.
 *
 * Note: You first need to run mailhandler_switch_user without
 * argument to store the current user. Call mailhandler_switch_user
 * without argument to set the user back to the original user.
 *
 * @param $uid The user ID to switch to
 *
 */
function mailhandler_switch_user($uid = NULL) {
  global $user, $language;
  static $orig_user = array(), $orig_language = array();

  if (isset($uid)) {
    session_save_session(FALSE);
    $user = user_load(array('uid' => $uid));
    drupal_init_language();
  }
  // retrieve the initial user, can be called multiple times
  else if (count($orig_user)) {
    $user = array_shift($orig_user);
    $language = array_shift($orig_language);
    session_save_session(TRUE);
    array_unshift($orig_user, $user);
    array_unshift($orig_language, $language);
  }
  // store the initial user
  else {
    $orig_user[] = $user;
    $orig_language[] = $language;
  }
}

function mailhandler_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural(count($results), '1 message retrieved for %mailbox.', '@count messages retrieved for %mailbox.', array('%mailbox' => $results[0]['mailbox']['mail']));
    drupal_set_message($message);
  }
  if (!empty($results)) {
    // Return the results to any asking modules
    foreach (module_list() as $name) {
      if (module_hook($name, 'mailhandler_batch_results')) {
        $function = $name .'_mailhandler_batch_results';
        if (!($messages = $function($results))) {
          // Exit if a module has handled the submitted data.
          break;
        }
      }
    }
  }
}

/**
 * Obtain the number of unread messages for an imap stream
 * 
 * @param $result
 *   IMAP stream - as opened by imap_open
 * @return 
 *   Array, values contain message numbers
 */
function mailhandler_get_unread_messages($result) {
  $unread_messages = array();
  $number_of_messages = imap_num_msg($result);
  for ($i = 1; $i <= $number_of_messages; $i++) {
    $header = imap_header($result, $i);
    // only process new messages
    if ($header->Unseen != 'U' && $header->Recent != 'N') {
      continue;
    }
    $unread_messages[] = imap_uid($result, $i);
  }
  return $unread_messages;
}

/**
 * Retrieve individual messages from an IMAP result
 * 
 * @param $result
 *   IMAP stream
 * @param $mailbox
 *   Array of mailbox configuration
 * @param $i
 *   Int message number
 * @param $context
 *   Array used by batch API
 * @return unknown_type
 */
function mailhandler_retrieve_message($result, $mailbox, $i, &$context) {
  // Required for batch API.
  if (!$result) {
    $result = mailhandler_open_mailbox($mailbox);
  }
  $header = imap_header($result, imap_msgno($result, $i));
  // Initialize the subject in case it's missing.
  if (!isset($header->subject)) {
    $header->subject = '';
  }
  $mime = explode(',', $mailbox['mime']);
  // Get the first text part - this will be the node body
  $origbody = mailhandler_get_part($result, $i, $mime[0]);
  // If we didn't get a body from our first attempt, try the alternate format (HTML or PLAIN)
  if (!$origbody) {
    $origbody = mailhandler_get_part($result, $i, $mime[1]);
  }
  // Parse MIME parts, so all mailhandler modules have access to
  // the full array of mime parts without having to process the email.
  $mimeparts = mailhandler_get_parts($result, $i);
  // Is this an empty message with no body and no mimeparts?
  if (!$origbody && !$mimeparts) {
    // @TODO: Log that we got an empty email?
    // TODO: We should not just close here, need to keep open in case of cron/auto
    imap_close($result);
    return;
  }
  // Don't delete while we're only getting new messages
  if ($mailbox['delete_after_read']) {
    imap_delete($result, $i, FT_UID);
  }
  $message = array('header' => $header, 'origbody' => $origbody, 'mimeparts' => $mimeparts, 'mailbox' => $mailbox);
  // If using batch API, must close imap stream.  Cron uses single stream.
  if (!empty($context) && array_key_exists('sandbox', $context)) {
    $context['results'][] = $message;
    imap_close($result, CL_EXPUNGE);
  }
  else {
    return $message;
  }
}

/**
 * Connect to mailbox and run message retrieval
 * 
 * @param $mailbox
 *   Array of mailbox configuration
 * @param $mode
 *   String, the retrieval mode, via the ui/batch system, or automated/cron/queue
 * @param $limit
 *   Int - the maximim number of messages to fetch on retrieval, only for 'auto' mode
 */
function mailhandler_retrieve($mailbox, $mode, $limit = 0) {
  // This is cast as string in hook_menu, otherwise the url argument would get used.
  $limit = (int) $limit;
  if ($result = mailhandler_open_mailbox($mailbox)) {
    $new = mailhandler_get_unread_messages($result);
  }
  switch ($mode) {
    case 'ui':
      if ($result) {
        // Batch does not support using a single stream because it makes multiple page calls.
        // The stream will be opened within mailhandler_retrieve_message
        imap_close($result, CL_EXPUNGE);  $result = 0;
        if (!empty($new)) {
          foreach ($new as $message) {
            $message_number = !$mailbox['imap'] ? 1 : $message;
            $operations[] = array('mailhandler_retrieve_message', array($result, $mailbox, $message_number));
          }
          $batch = array(
            'title' => 'Mailhandler retrieve',
            'operations' => $operations,
            'finished' => 'mailhandler_batch_finished',
            'init_message' => format_plural(count($new), 'Preparing to retrieve 1 message...', 'Preparing to retrieve @count messages...'),
            'progress_message' => t('Retrieving message @current of @total.'),
            'file' => drupal_get_path('module', 'mailhandler') .'/mailhandler.retrieve.inc',
          );
          batch_set($batch);
          // Make 'progressive' mode work.  Hack due to bug http://drupal.org/node/638712
          $batch =& batch_get();
          $batch['progressive'] = FALSE;
          batch_process('admin/content/mailhandler');
        }
        else {
          drupal_set_message(t('There are no messages to retrieve for %mail.', array('%mail' => $mailbox['mail'])));
        }
      }
      else {
        drupal_set_message(t('Unable to connect to %mail.', array('%mail' => $mailbox['mail'])));
      }
      drupal_goto('admin/content/mailhandler');
      break;
    case 'auto':
      if ($result) {
        if (!empty($new)) {
          $messages = array();
          $retrieved = 0;
          while ($new && (!$limit || $retrieved < $limit)) {
            $messages[] = mailhandler_retrieve_message($result, $mailbox, array_shift($new), $context = array());
            $retrieved++;
          }
          imap_close($result, CL_EXPUNGE);
          return $messages;
        }
      }
      else {
        watchdog('mailhandler', 'Unable to connect to %mail', array('%mail' => $mailbox['mail']), WATCHDOG_ERROR);
      }
      break;
  }
}
