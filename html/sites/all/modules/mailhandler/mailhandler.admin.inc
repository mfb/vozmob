<?php

/**
 * Return a form for editing or creating an individual mailbox.
 */
function mailhandler_add_edit_mailbox($form_state, $mid, $clone = FALSE) {
  module_load_include('inc', 'mailhandler', 'mailhandler.retrieve');
  $edit = $mid ? mailhandler_get_mailbox($mid) : array(
    'mail' => '',
    'mailto' => '',
    'imap' => 0,
    'domain' => '',
    'port' => '',
    'name' => '',
    'pass' => '',
    'extraimap' => '',
    'mime' => 'TEXT/HTML,TEXT/PLAIN',
    'security' => 0,
    'replies' => 1,
    'fromheader' => '',
    'commands' => '',
    'sigseparator' => '',
    'delete_after_read' => 1,
    'enabled' => 1,
    'format' => FILTER_FORMAT_DEFAULT,
    'authentication',
  );

  if ($clone) {
    unset($mid);
  }

  if (empty($edit['folder'])) {
    $edit['folder'] = 'INBOX';
  }

  $form['mail'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail address'),
    '#default_value' => $edit['mail'],
    '#description' => t('The e-mail address to which users should send their submissions.'),
    '#required' => TRUE,
  );

  $form['mailto'] = array(
    '#type' => 'textfield',
    '#title' => t('Second E-mail address'),
    '#default_value' => $edit['mailto'],
    '#description' => t('Optional. The e-mail address to which modules should send generated content.'),
  );

  $form['folder'] = array(
    '#type' => 'textfield',
    '#title' => t('Folder'),
    '#default_value' => $edit['folder'],
    '#description' => t('Optional. The folder where the mail is stored. If you want this mailbox to read from a local folder, give the full path. Leave domain, port, name, and pass empty below. Remember to set the folder to readable and writable by the webserver.'),
  );

  $form['imap'] = array(
    '#type' => 'select',
    '#title' => t('POP3 or IMAP Mailbox'),
    '#options' => array('POP3', 'IMAP'),
    '#default_value' => $edit['imap'],
    '#description' => t('If you wish to retrieve mail from a POP3 or IMAP mailbox instead of a Folder, select POP3 or IMAP. Also, complete the Mailbox items below.'),
  );

  $form['domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Mailbox domain'),
    '#default_value' => $edit['domain'],
    '#description' => t('The domain of the server used to collect mail.'),
  );

  $form['port'] = array(
    '#type' => 'textfield',
    '#title' => t('Mailbox port'),
    '#size' => 5, '#maxlength' => 5,
    '#default_value' => $edit['port'],
    '#description' => t('The port of the mailbox used to collect mail (usually 110 for POP3, 143 for IMAP).'),
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Mailbox username'),
    '#default_value' => $edit['name'],
    '#description' => t('This username is used while logging into this mailbox during mail retrieval.'),
  );

  $form['pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Mailbox password'),
    '#default_value' => $edit['pass'],
    '#description' => t('The password corresponding to the username above. Consider using a non-vital password, since this field is stored without encryption in the database.'),
  );

  // Allow administrators to configure the mailbox with extra IMAP commands (notls, novalidate-cert etc.)
  $form['extraimap'] = array(
    '#type' => 'textfield',
    '#title' => t('Extra commands'),
    '#default_value' => $edit['extraimap'],
    '#description' => t('Optional. In some circumstances you need to issue extra commands to connect to your mail server (e.g. "/notls", "/novalidate-cert" etc.). See documentation for <a href="http://php.net/imap_open">imap_open</a>. Begin the string with a "/", separating each subsequent command with another "/".'),
  );

  $form['mime'] = array(
    '#type' => 'select',
    '#title' => t('MIME preference'),
    '#options' => array(
      'TEXT/HTML,TEXT/PLAIN' => 'HTML',
      'TEXT/PLAIN,TEXT/HTML' => t('Plain text'),
    ),
    '#default_value' => $edit['mime'],
    '#description' => t('When a user sends an e-mail containing both HTML and plain text parts, use this part as the node body.'),
  );

  $form['security'] = array(
    '#type' => 'radios',
    '#title' => t('Security'),
    '#options' => array(t('Disabled'), t('Require password')),
    '#default_value' => isset($edit['security']) ? $edit['security'] : 0,
    '#description' => t('Disable security if your site does not require a password in the Commands section of incoming e-mails. Note: Security=Enabled and MIME preference=HTML is an unsupported combination.')
  );

  $form['replies'] = array('#type' => 'radios', '#title' => t('Send error replies'), '#options' => array(t('Disabled'), t('Enabled')), '#default_value' => $edit['replies'], '#description' => t('Send helpful replies to all unsuccessful e-mail submissions. Consider disabling when a listserv posts to this mailbox.'));

  $form['fromheader'] = array('#type' => 'textfield', '#title' => t('From header'), '#default_value' => $edit['fromheader'], '#description' => t('Use this e-mail header to determine the author of the resulting node. Admins usually leave this field blank (thus using the <strong>From</strong> header), but <strong>Sender</strong> is also useful when working with listservs.'));

  $form['commands'] = array('#type' => 'textarea', '#title' => t('Default commands'), '#default_value' => $edit['commands'], '#description' => t('A set of commands which are added to each message. One command per line. See <a href="@link">Commands</a>.', array('@link' => url('admin/help/mailhandler/#commands'))));

  $form['sigseparator'] = array('#type' => 'textfield', '#title' => t('Signature separator'), '#default_value' => $edit['sigseparator'], '#description' => t('All text after this string will be discarded. A typical value is <strong>"-- "</strong> that is two dashes followed by a blank in an otherwise empty line. Leave blank to include signature text in nodes.'));

  $form['delete_after_read'] = array('#type' => 'checkbox', '#title' => t('Delete messages after they are processed?'), '#default_value' => $edit['delete_after_read'], '#description' => t('Uncheck this box to leave read messages in the mailbox. They will not be processed again unless they become marked as unread.  If you selected "POP3" as your mailbox type, you must check this box.'));

  $form['enabled'] = array('#type' => 'radios', '#title' => t('Cron processing'), '#options' => array(t('Disabled'), t('Enabled')), '#default_value' => $edit['enabled'], '#description' => t('Select disable to temporarily stop cron processing for this mailbox.'));

  // Allow administrators to select the format of saved nodes/comments
  $form['format'] = filter_form($edit['format']);
  
  $options = array();
  $descriptions = '';
  $options[0] = t('Choose method');
  $methods = mailhandler_mailhandler_authenticate('info');
  foreach ($methods as $method) {
    foreach ($method as $name => $info) {
      $options[$name] = t($info['title']);
      // TODO: theme function
      $descriptions .= t($info['title']) .": ". t($info['description']) . "\n";
    }
  }
  $form['authentication'] = array(
    '#type' => 'select',
    '#title' => t('Message authentication method'),
    '#options' => $options,
    '#description' => t('Following is the name and description of each available authentication method') . "\n\n" . $descriptions,
    '#default_value' => $edit['authentication'],
  );

  $form['actions'] = array('#prefix' => '<p><div class="confirmation container-inline">', '#suffix' => '</div></p>');

  $form['mid'] = array(
    '#type' => 'hidden',
    '#value' => $mid,
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save mailbox'),
  );

  $form['actions']['test'] = array(
    '#type' => 'submit',
    '#value' => t('Test connection'),
  );

  if ($mid) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete mailbox'),
      '#submit' => array('mailhandler_delete_mailbox_submit'),
      '#validate' => array(),
    );
  }

  $form['actions']['cancel'] = array(
    '#value' => l(t('Cancel'), 'admin/content/mailhandler'),
  );

  return $form;
}


/**
 * Verify that the Mailbox is valid, and save it to the database.
 */
function mailhandler_add_edit_mailbox_validate($form, &$form_state) {

  // Check for valid email address
  if ($error = user_validate_mail($form_state['values']['mail'])) {
    form_set_error('mail', $error);
  }

  if ($form_state['values']['mailto'] && ($error = user_validate_mail($form_state['values']['mailto']))) {
    form_set_error('mailto', $error);
  }

  // Test POP/IMAP settings, and store result
  $mailbox_appears_ok = TRUE;

  if ($form_state['values']['domain'] && $form_state['values']['port'] && !is_numeric($form_state['values']['port'])) { // assume external mailbox
    form_set_error('port', t('Mailbox port must be an integer.'));
    $mailbox_appears_ok = FALSE;
  }

  if (!$form_state['values']['domain'] && !$form_state['values']['port'] && $form_state['values']['folder']) { // assume local folder
    // check read and write permission
    if (!is_readable($form_state['values']['folder']) || !is_writable($form_state['values']['folder'])) {
      form_set_error('port', t('The local folder has to be readable and writable by owner of the webserver process, e.g. nobody.'));
      $mailbox_appears_ok = FALSE;
    }
  }

  // Test POP/IMAP settings are valid in all cases, and test connection if requested
  if ($mailbox_appears_ok && $form_state['clicked_button']['#value'] == t('Test connection')) {
    // Call the test function
    mailhandler_test_mailbox($form_state);

    // We don't want to save, so we set an 'invisible error' that means the form is not submitted
    form_set_error('mailhandler');
  }
  
  // If POP3 mailbox is chosen, messages must be deleted after processing.
  if ($form_state['values']['imap'] == 0 && $form_state['values']['delete_after_read'] == 0) {
    form_set_error('delete_after_read', t('You must check off "Delete messages after they are processed" when using a POP3 mailbox.'));
  }

}


/**
 * Save a mailbox to the database - determine whether adding or editing by checking for a mailbox id (mid)
 */
function mailhandler_add_edit_mailbox_submit($form, &$form_state) {
// TODO: try to rewrite these monsters wth drupal_write_record();
  if ($form_state['values']['mid']) {
    db_query("UPDATE {mailhandler} SET mail = '%s', mailto = '%s', domain = '%s', port = %d, folder = '%s', name = '%s', pass = '%s', extraimap = '%s', mime = '%s', imap = '%s', security = %d, replies = %d, fromheader = '%s', commands = '%s', sigseparator = '%s', enabled = %d, delete_after_read = %d, format = %d, authentication = '%s' WHERE mid = %d", $form_state['values']['mail'], $form_state['values']['mailto'], $form_state['values']['domain'], $form_state['values']['port'], $form_state['values']['folder'], $form_state['values']['name'], $form_state['values']['pass'], $form_state['values']['extraimap'], $form_state['values']['mime'], $form_state['values']['imap'], $form_state['values']['security'], $form_state['values']['replies'], $form_state['values']['fromheader'], $form_state['values']['commands'], $form_state['values']['sigseparator'], $form_state['values']['enabled'], $form_state['values']['delete_after_read'], $form_state['values']['format'], $form_state['values']['authentication'], $form_state['values']['mid']);
    drupal_set_message(t('Mailbox %mailbox updated.', array('%mailbox' => $form_state['values']['mail'])));
  }
  else {
    db_query("INSERT INTO {mailhandler} (mail, mailto, domain, port, folder, name, pass, extraimap, mime, imap, security, replies, fromheader, commands, sigseparator, enabled, delete_after_read, format, authentication) VALUES ('%s', '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s', %d, %d, '%s', '%s', '%s', %d, %d, %d, '%s')", $form_state['values']['mail'], $form_state['values']['mailto'], $form_state['values']['domain'], $form_state['values']['port'], $form_state['values']['folder'], $form_state['values']['name'], $form_state['values']['pass'], $form_state['values']['extraimap'], $form_state['values']['mime'], $form_state['values']['imap'], $form_state['values']['security'], $form_state['values']['replies'], $form_state['values']['fromheader'], $form_state['values']['commands'], $form_state['values']['sigseparator'], $form_state['values']['enabled'], $form_state['values']['delete_after_read'], $form_state['values']['format'], $form_state['values']['authentication']);
    drupal_set_message(t('Mailbox %mailbox added.', array('%mailbox' => $form_state['values']['mail'])));
  }

  $form_state['redirect'] = 'admin/content/mailhandler';
  return;
}


/**
 *  Submit handler for mailbox delete button
 *  Redirect to delete handler if delete is clicked
 */
function mailhandler_delete_mailbox_submit($form, &$form_state) {
  $form_state['redirect'] = array('admin/content/mailhandler/delete/'.$form_state['values']['mid']);
}


/**
 * Confirm/Delete Mailbox
 */
function mailhandler_admin_delete_confirm($form_state, $mid) {

  $info = db_fetch_object(db_query("SELECT mid, mail FROM {mailhandler} WHERE mid = %d", $mid));

  $form['mid'] = array(
    '#type' => 'hidden',
    '#value' => $mid,
  );

  return confirm_form(
      $form,
      t('Do you wish to delete mailbox %mailbox?', array('%mailbox' => $info->mail)),
      'admin/content/mailhandler',
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
}


function mailhandler_admin_delete_confirm_submit($form, &$form_state) {
  $info = db_fetch_object(db_query("SELECT mid, mail FROM {mailhandler} WHERE mid = %d", $form_state['values']['mid']));
  db_query("DELETE FROM {mailhandler} WHERE mid = %d", $form_state['values']['mid']);
  watchdog('mailhandler', 'Mailhandler: Mailbox %mailbox deleted', array('%mailbox' => $info->mail), WATCHDOG_INFO);
  drupal_set_message(t('Mailbox %mailbox deleted', array('%mailbox' => $info->mail)));
  $form_state['redirect'] = 'admin/content/mailhandler';
  return;
}


/**
 * Test connection to a mailbox, and output results as a message
 */
function mailhandler_test_mailbox(&$form_state) {
  module_load_include('inc', 'mailhandler', 'mailhandler.retrieve');
  $mailbox = $form_state['values'];
  $result = mailhandler_open_mailbox($mailbox);

  if ($result) {
    drupal_set_message('Mailhandler was able to connect to the mailbox.');
    imap_close($result);
  }
  else {
    if ($mailbox['domain']) {
      form_set_error('mailhandler', t('%c connection to %m failed.', array('%c' => ($mailbox['imap'] ? 'IMAP' : 'POP3'), '%m' => $mailbox['mail'])));
    }
    else {
      form_set_error('mailhandler', t('Mailhandler could not access local folder: %m', array('%m' => $mailbox['mail'])));
    }
  }

  return;
}


function mailhandler_list_mailboxes() {

  $header = array(t('Mailbox'), t('Folder'), array('data' => t('Operations'), 'colspan' => 4));
  $rows = array();
  $result = db_query('SELECT * FROM {mailhandler} ORDER BY mail');
  while ($mailbox = db_fetch_object($result)) {
    $rows[] = array(
      '<a href="mailto:'. $mailbox->mail .'">'. $mailbox->mail .'</a>',
      $mailbox->folder ? check_plain($mailbox->folder) : '',
      l(t('Retrieve'), 'admin/content/mailhandler/retrieve/'.$mailbox->mid, array('title' => t('Retrieve and process pending e-mails in this mailbox'))),
      l(t('Edit'), 'admin/content/mailhandler/edit/'.$mailbox->mid, array('title' => t('Edit this mailbox configuration'))),
      l(t('Clone'), 'admin/content/mailhandler/clone/'.$mailbox->mid, array('title' => t('Add a new mailbox, using this mailbox configuration as a template'))),
      l(t('Delete'), 'admin/content/mailhandler/delete/'.$mailbox->mid, array('title' => t('Delete this mailbox'))),
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => '<em>'. t('No mailboxes have been defined.') .'</em>', 'colspan' => 4));
  }

  return theme('table', $header, $rows);

}


/**
 * Return the settings page for mailhandler
 */
function mailhandler_admin_settings() {

  $form['mailhandler_default_type'] = array(
    '#type' => 'radios',
    '#title' => t('Default content type'),
    '#options' => node_get_types('names'),
    '#default_value' => mailhandler_default_type(),
    '#description' => t('If an incoming message does not specify a particular content type, then use this type as a default.'),
  );

  $form['mailhandler_max_retrieval'] = array(
    '#type' => 'textfield',
    '#title' => t('Cron maximum message retrieval'),
    '#default_value' => variable_get('mailhandler_max_retrieval', 0),
    '#description' => t('To prevent timeout errors from large mailboxes you can limit the maximum number of messages that will be processed during each cron run. This is a global setting and applies to all mailboxes. A value of zero means that no limit will be applied. Some trial and error may be needed to find the optimum setting.'),
  );
  
  $form['mailhandler_default_encoding'] = array(
    '#type' => 'textfield',
    '#title' => t('Default character encoding'),
    '#default_value' => 'UTF-8',
    '#description' => t('The default character encoding to use when an incoming message does not define an encoding.')
  );

  $form['#validate'][] = 'mailhandler_admin_settings_validate';

  return system_settings_form($form);
}


/**
 * Validation for mailhandler settings page
 */

function mailhandler_admin_settings_validate($form, &$form_state) {

  if (empty($form_state['values']['mailhandler_max_retrieval'])) {
    $form_state['values']['mailhandler_max_retrieval'] = 0;
  }

  if (
    !is_numeric($form_state['values']['mailhandler_max_retrieval']) ||
    $form_state['values']['mailhandler_max_retrieval'] < 0 ||
    $form_state['values']['mailhandler_max_retrieval'] <> intval($form_state['values']['mailhandler_max_retrieval'])
  ) {
    form_set_error('mailhandler_max_retrieval', t('The value should a positive integer, or zero.'));
  }
}
