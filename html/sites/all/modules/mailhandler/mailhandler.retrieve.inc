<?php


function mailhandler_admin_retrieve($mid) {

  // Retrieve mailbox details for specified mailbox number
  $mailbox = mailhandler_get_mailbox($mid);

  // Check to see if there are new messages requiring retrieval
  // If messages available, set batch and begin processing
  if ($messages = mailhandler_get_unread_messages($mailbox)) {
    foreach ($messages as $message_number) {
      $operations[] = array(
        'mailhandler_retrieve_message',
        array($result, $mailbox, $message_number),
      );
    }

    // Create an array of batch operations
    $batch = array(
      'title' => 'Mailhandler retrieve',
      'operations' => $operations,
      'finished' => 'mailhandler_batch_finished',
      'init_message' => format_plural(count($messages), 'Preparing to retrieve 1 message...', 'Preparing to retrieve @count messages...'),
      'progress_message' => t('Retrieving message @current of @total.'),
      'file' => drupal_get_path('module', 'mailhandler') .'/mailhandler.retrieve.inc',
    );

    // Set the batch
    batch_set($batch);

    // Begin batch processing
    batch_process('admin/content/mailhandler');
  }
  else {
    // If there are no new messages, set a message
    drupal_set_message(t('There are no messages to retrieve for %mail.', array('%mail' => $mailbox['mail'])));
  }

  // Redirect to mailhandler overview form upon completion
  drupal_goto('admin/content/mailhandler');

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
    foreach ($structure->parameters as $parameter) {
      if (strtoupper($parameter->attribute) == 'CHARSET') {
        $encoding = $parameter->value;
        //watchdog('mailhandler', 'Encoding is ' . $encoding);
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


/**
 * Append default commands. Separate commands from body. Strip signature.
 * Return a node object.
 */
function mailhandler_process_message($header, $body, $mailbox) {

  // Initialise a node object
  $node = new stdClass();
  $node->pass = NULL;

  // Initialize parameters
  $sep = $mailbox['sigseparator'];

  // Copy any name/value pairs from In-Reply-To or References e-mail headers to $node. Useful for maintaining threading info.
  if (!empty($header->references)) {
    // we want the final element in references header, watching out for white space
    $threading = substr(strrchr($header->references, '<'), 0);
  }
  else if (!empty($header->in_reply_to)) {
    $threading = str_replace(strstr($header->in_reply_to, '>'), '>', $header->in_reply_to); // Some MUAs send more info in that header.
  }
  if (isset($threading) && $threading = rtrim(ltrim($threading, '<'), '>')) { //strip angle brackets
    if ($threading) $node->threading = $threading;
    parse_str($threading, $tmp);
    if ($tmp['host']) {
      $tmp['host'] = ltrim($tmp['host'], '@'); // strip unnecessary @ from 'host' element
    }
    foreach ($tmp as $key => $value) {
      $node->$key = $value;
    }
  }

  // Prepend the default commands for this mailbox
  if ($mailbox['commands']) {
    $body = trim($mailbox['commands']) ."\n". $body;
  }

  // Set a default type if none provided
  if (!$node->type) $node->type = mailhandler_default_type();

  // Apply defaults to the $node object, and allow modules to add default values
  require_once($base_path . 'modules/node/node.pages.inc');
  node_object_prepare($node);

  // Reset $node->taxonomy
  $node->taxonomy = array();

  // Process the commands and the body
  $lines = explode("\n", $body);
  for ($i = 0; $i < count($lines); $i++) {
    $line = trim($lines[$i]);
    $words = explode(' ', $line);

    // Look for a command line. if not present, note which line number is the boundary
    if (substr($words[0], -1) == ':' && !isset($endcommands)) {

      // Looks like a name: value pair
      $data = explode(': ', $line, 2);

      // TODO: allow for nested arrays in commands ... Possibly trim() values after explode().
      // If needed, turn this command value into an array
      if (substr($data[1], 0, 1) == '[' && substr($data[1], -1, 1) == ']') {
        $data[1] = rtrim(ltrim($data[1], '['), ']'); //strip brackets
        $data[1] = explode(",", $data[1]);
      }
      $data[0] = strtolower(str_replace(' ', '_', $data[0]));
      // if needed, map term names into IDs. this should move to taxonomy_mailhandler()
      if ($data[0] == 'taxonomy' && !is_numeric($data[1][0])) {
        array_walk($data[1], 'mailhandler_term_map');
        $node->taxonomy = array_merge($node->taxonomy, $data[1]);
        unset($data[0]);
      }
      else if (substr($data[0], 0, 9) == 'taxonomy[' && substr($data[0], -1, 1) == ']'){
        // make sure a valid vid is passed in:
        $vid = substr($data[0], 9, -1);
        $vocabulary = taxonomy_vocabulary_load($vid);
        // if the vocabulary is not activated for that node type, unset $data[0], so the command will be ommited from $node
        // TODO: add an error message
        if (!in_array($node->type, $vocabulary->nodes)) {
          unset($data[0]);
        }
        else if (!$vocabulary->tags) {
          array_walk($data[1], 'mailhandler_term_map');
          $node->taxonomy = array_merge($node->taxonomy, $data[1]);
          unset($data[0]);
        }
        else if ($vocabulary->tags) {
          // for freetagging vocabularies, we just pass the list of terms
          $node->taxonomy['tags'][$vid] = implode(',', $data[1]);
          unset($data[0]); // unset, so it won't be included when populating the node object
        }
      }
      if (!empty($data[0])) {
        $node->$data[0] = $data[1];
      }
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

  // Isolate the body from the commands and the sig
  $tmp = array_slice($lines, $endcommands, $i - $endcommands);
  // flatten and assign the body to node object. note that filter() is called within node_save() [tested with blog post]
  $node->body = implode("\n", $tmp);

  if (empty($node->teaser)) $node->teaser = node_teaser($node->body);

  // decode encoded subject line
  $subjectarr = imap_mime_header_decode($header->subject);
  for ($i = 0; $i < count($subjectarr); $i++) {
    if ($subjectarr[$i]->charset != 'default')
      $node->title .= drupal_convert_to_utf8($subjectarr[$i]->text, $subjectarr[$i]->charset);
    else
      $node->title .= $subjectarr[$i]->text;
  }

  $node->date = $node->changed = format_date($header->udate, 'custom', 'Y-m-d H:i:s O');
  $node->format = $mailbox['format'];
  
  // If an nid command was supplied, and type is not 'comment', append the revision number
  if ($node->nid && $node->type != 'comment') {
    $vid = db_result(db_query('SELECT n.vid FROM {node} n WHERE n.nid = %d', $node->nid));
    if ($vid) {
      $node->revision = $node->vid = $vid;
    }
  }
  
  return $node;
}


/**
 * Determine who is the author of the upcoming node.
 */
function mailhandler_authenticate($node, $header, $origbody, $mailbox) {

  // $fromaddress really refers to the mail header which is authoritative for authentication
  list($fromaddress, $fromname) = mailhandler_get_fromaddress($header, $mailbox);
  if ($from_user = mailhandler_user_load($fromaddress, $node->pass, $mailbox)) {
    $node->uid = $from_user->uid; // success!
    $node->name = $from_user->name;
  }
  else if (function_exists('mailalias_user')) { // since $fromaddress failed, try e-mail aliases
    $result = db_query("SELECT mail FROM {users} WHERE data LIKE '%%%s%%'", $fromaddress);
    while ($alias = db_result($result)) {
      if ($from_user = mailhandler_user_load($alias, $node->pass, $mailbox)) {
        $node->uid = $from_user->uid; // success!
        $node->name = $from_user->name;
        break;
      }
    }
  }
  if (!$from_user) {
    // failed authentication. we will still try to submit anonymously.
    $node->uid = 0;
    $node->name = $fromname; // use the name supplied in email headers
  }
  return $node;
}


/**
 * Create the comment.
 */
function mailhandler_comment_submit($node, $header, $mailbox, $origbody) {
  global $user;
  if (!$node->subject) $node->subject = $node->title;
  // When submitting comments, 'comment' means actualy the comment's body, and not the comments status for a node.
  // We need to reset the comment's body, so it doesn't colide with a default 'comment' command. 
  $node->comment = $node->body;
  // We want the comment to have the email time, not the current time
  $node->timestamp = $node->created;
  // comment_save gets an array
  $edit = (array)$node;

  // post the comment. if unable, send a failure email when so configured
  $cid = comment_save($edit);
  if (!$cid && $mailbox['replies']) {
    // $fromaddress really refers to the mail header which is authoritative for authentication
    list($fromaddress, $fromname) = mailhandler_get_fromaddress($header, $mailbox);
    $error_text = t('Sorry, your comment experienced an error and was not posted. Possible reasons are that you have insufficient permission to post comments or the node is no longer open for comments.');
    $params = array('body' => $origbody, 'error_messages' => array(), 'error_text' => $error_text, 'from' => $fromaddress, 'header' => $header, 'node' => $node);
    drupal_mail('mailhandler', 'mailhandler_error_comment', $fromaddress, user_preferred_language($user), $params);
    watchdog('mailhandler', 'Comment submission failure: %subject.', array('%subject' => $edit['subject']), WATCHDOG_ERROR);
  }
  
  return $cid;
}


/**
 * Create the node.
 */
function mailhandler_node_submit($node, $header, $mailbox, $origbody) {
  global $user;
  list($fromaddress, $fromname) = mailhandler_get_fromaddress($header, $mailbox);

  // Drupal 5.x & 6.x don't support multiple validations: each node_validate()
  // call will ADD error messages to previous ones, so if some validation error
  // occours in one message it will be reported in all messages after it.
  // Since there is no way to reset form errors, the only method to avoid this
  // problem is working with $_SESSION['messages'], used by form_set_error().
  // See http://drupal.org/node/271975 for more info.
  // Warning: with this method, if the same error message is reported for 2+ different
  // fields it will be detected only for the last one.
  if (!isset($_SESSION['messages'])) {
    $_SESSION['messages'] = array();
  }
  $saved_errors = isset($_SESSION['messages']['error']) ? $_SESSION['messages']['error'] : array();
  $_SESSION['messages']['error'] = array();
  node_validate($node);
  $error_messages = array();
  if (count($_SESSION['messages']['error'])) {
    $allerrors = form_get_errors();
    foreach ($_SESSION['messages']['error'] as $message) {
      $keys = array_keys($allerrors, $message);
      if (!$keys || !count($keys)) {
        // Not a validation error (but an error, i'll print it)
        $saved_errors[] = $message;
      } else {
        // This is a validation error, i take the last field with it (previous fields
        // should be about previous validations)
        $error_messages[$keys[count($keys) - 1]] = $message;
      }
    }
  }
  if (is_array($saved_errors) && count($saved_errors)) {
     $_SESSION['messages']['error'] = $saved_errors;
  }
  else {
    unset($_SESSION['messages']['error']);
  }

  if (!$error_messages) {
    // Prepare the node for save and allow modules make changes
    $node = node_submit($node);
    // Save the node
    if (!empty($node->nid)) {
      if (node_access('update', $node)) {
        node_save($node);
        watchdog('mailhandler', 'Updated %title by %from.', array('%title' => $node->title, '%from' => $fromaddress));
      }
      else {
        $error_text = t('The e-mail address !from may not update !type items.', array('!from' => $fromaddress, '!type' => $node->type));
        watchdog('mailhandler', 'Node submission failure: %from may not update %type items.', array('%from' => $fromaddress, '%type' => $node->type), WATCHDOG_WARNING);
      }
    }
    else {
      if (node_access('create', $node)) {
        node_save($node);
        watchdog('mailhandler', 'Added %title by %from.', array('%title' => $node->title, '%from' => $fromaddress));
      }
      else {
        $error_text = t('The e-mail address !from may not create !type items.', array('!from' => $fromaddress, '!type' => $node->type));
        watchdog('mailhandler', 'Node submission failure: %from may not create %type items.', array('%from' => $fromaddress, '%type' => $node->type), WATCHDOG_WARNING);
      }
    }
    // Return the node is successfully saved
    if (!isset($error_text)) {
      return $node;
    } 
  }
  else {
    $error_text = t('Your submission is invalid:');
    watchdog('mailhandler', 'Node submission failure: validation error.', array(), WATCHDOG_WARNING);
  }

  if (isset($error_text)) {
    if ($mailbox['replies']) {
      $params = array('body' => $origbody, 'error_messages' => $error_messages, 'error_text' => $error_text, 'from' => $fromaddress, 'header' => $header, 'node' => $node);
      drupal_mail('mailhandler', 'mailhandler_error_node', $fromaddress, user_preferred_language($user), $params);
    }
  }
  
  // return FALSE if the node was not successfully saved
  return FALSE;
}


/**
 * Implementation of hook_mail().
 */
function mailhandler_mail($key, &$message, $params) {
  $variables = array(
    '!body' => $params['body'],
    '!from' => $params['from'],
    '!site_name' => variable_get('site_name', 'Drupal'),
    '!subject' => $params['header']->subject,
    '!type' => $params['node']->type,
  );
  $message['subject'] = t('Email submission to !site_name failed - !subject', $variables);
  $message['body'][] = $params['error_text'];
  foreach ($params['error_messages'] as $key => $error) {
    $message['body'][$key] = decode_entities(strip_tags($error));
  }
  $message['body'][] = t("You sent:\n\nFrom: !from\nSubject: !subject\nBody:\n!body", $variables);
}


/**
 * Accept a taxonomy term name and replace with a tid. this belongs in taxonomy.module.
 */
function mailhandler_term_map(&$term) {
  // provide case insensitive and trimmed map so as to maximize likelihood of successful mapping
  $term = db_result(db_query("SELECT tid FROM {term_data} WHERE LOWER('%s') LIKE LOWER(name)", trim($term)));
}


/**
 * Retrieve user information from his email address.
 */
function mailhandler_user_load($mail, $pass, $mailbox) {
  if ($mailbox['security'] == 1) {
    return user_load(array('mail' => $mail, 'pass' => $pass));
  }
  else {
    return user_load(array('mail' => $mail));
  }
}


/**
 * If available, use the mail header specified in mailbox config. otherwise use From: header
 */
function mailhandler_get_fromaddress($header, $mailbox) {
  if (($fromheader = strtolower($mailbox['fromheader'])) && isset($header->$fromheader)) {
    $from = $header->$fromheader;
  }
  else {
    $from = $header->from;
  }
  return array($from[0]->mailbox .'@'. $from[0]->host, $from[0]->personal);
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
  global $user;
  static $orig_user = array();

  if (isset($uid)) {
    session_save_session(FALSE);
    $user = user_load(array('uid' => $uid));
  }
  // retrieve the initial user, can be called multiple times
  else if (count($orig_user)) {
    $user = array_shift($orig_user);
    session_save_session(TRUE);
    array_unshift($orig_user, $user);
  }
  // store the initial user
  else {
    $orig_user[] = $user;
  }
}


function mailhandler_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural(count($results), '1 message retrieved for %mailbox.', '@count messages retrieved for %mailbox.', array('%mailbox' => $results[0]));
    drupal_set_message($message);
  }
}


function mailhandler_get_unread_messages($mailbox) {

  $unread_messages = array();
  $result = mailhandler_open_mailbox($mailbox);

  if ($result) {

    $number_of_messages = imap_num_msg($result);
    for ($i = 1; $i <= $number_of_messages; $i++) {
      $header = imap_header($result, $i);

      // only process new messages
      if ($header->Unseen != 'U' && $header->Recent != 'N') {
        continue;
      }

      $unread_messages[] = imap_uid($result, $i);
    }
  }

  imap_close($result);
  return $unread_messages;
}

function mailhandler_retrieve_message(&$result = FALSE, $mailbox, $i, &$context) {
  // Required for batch API.
  if (!$result) {
    $result = mailhandler_open_mailbox($mailbox);
  }
  mailhandler_switch_user();

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
    imap_close($result);
    return;
  }

  // we must process before authenticating because the password may be in Commands
  $node = mailhandler_process_message($header, $origbody, $mailbox);

  // check if mail originates from an authenticated user
  $node = mailhandler_authenticate($node, $header, $origbody, $mailbox);

  // Put $mimeparts on the node
  $node->mimeparts = $mimeparts;

  // we need to change the current user
  // this has to be done here to allow modules
  // to create users
  mailhandler_switch_user($node->uid);

  // modules may override node elements before submitting. they do so by returning the node.
  foreach (module_list() as $name) {
    if (module_hook($name, 'mailhandler')) {
      $function = $name .'_mailhandler';
      if (!($node = $function($node, $result, $i, $header, $mailbox))) {
        // Exit if a module has handled the submitted data.
        break;
      }
    }
  }

  if ($node) {
    if ($node->type == 'comment') {
      $nid = mailhandler_comment_submit($node, $header, $mailbox, $origbody);
      $type = 'comment';
    }
    else {
      $nid = mailhandler_node_submit($node, $header, $mailbox, $origbody);
      $type = 'node';
    }
  }
  
  // Invoke a second hook for modules to operate on the newly created/edited node/comment.
  foreach (module_list() as $name) {
    if (module_hook($name, 'mailhandler_post_save')) {
      $function = $name .'_mailhandler_post_save';
      // Pass in the $nid (which could be a $cid, depending on $node->type)
      $function($nid, $type);
    }
  }
  
  // don't delete while we're only getting new messages
  if ($mailbox['delete_after_read']) {
    imap_delete($result, $i, FT_UID);
  }

  // switch back to original user
  mailhandler_switch_user();

  // Put something in the results array for the counter in the batch finished callback
  $context['results'][] = $mailbox['mail'];

  mailhandler_switch_user();

  // If using batch API, must close imap stream.  Cron uses single stream.
  $args = func_get_args();
  if (!$args[0]) {
    imap_close($result, CL_EXPUNGE);
  }
}


/**
 * Retrieve messages when triggered by cron
 *
 * To prevent timeouts when handling large messages we can limit the number of messages processed on each
 * cron run, using a setting on the configuration page. A setting of zero means no limit is applied.
 *
 * Flagging messages as unseen only works for IMAP so for POP mail to use limited downloads messages must
 * be deleted after retrieval to prevent repeated downloading of the same messages on each run
 */
function mailhandler_cron_retrieve($mailbox) {

  if ($result = mailhandler_open_mailbox($mailbox)) {
    // Find out how many messages need retrieval
    $new_messages = mailhandler_get_unread_messages($mailbox);

    // Initialise counters for maximum message retrieval
    $max_messages = variable_get('mailhandler_max_retrieval', 0);
    $retrieved_messages = 0;

    // Begin retrieval of messages
    while ($new_messages && (!$max_messages || $retrieved_messages < $max_messages)) {
      mailhandler_retrieve_message($result, $mailbox, array_shift($new_messages), $context);
      $retrieved_messages++;
    }
    imap_close($result, CL_EXPUNGE);
  }
}
