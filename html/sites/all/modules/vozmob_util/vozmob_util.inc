<?php
// $Id$

/**
 * Provide a checkbox for each set of truncate options.
 *
 * @todo: Split these up onto separate forms for cleanup vs. scrubbing site data.
 */
function vozmob_util_truncate_form(&$form_state) {
  global $base_url;
  if ($form_state['submitted']) {
    $form['task'] = array('#type' => 'value', '#value' => $form_state['values']['task']);
    return confirm_form($form, t('Are you sure you want to permanently delete data for @site_name?', array('@site_name' => variable_get('site_name', ''))), 'admin/build', t('Data for <big>%base_url</big> will be permanently deleted. This action cannot be undone.', array('%base_url' => $base_url)), t('Delete data'), t('Cancel'));
  }
  $tasks = array(
    'sandbox' => t('Clean up production database for sandbox environment.'),
    'staging' => t('Clean up production database for staging environment.'),
    'release' => t('Scrub all site-specific data for public release of database. <strong>Warning all site data will be deleted!</strong>'),
  );
  $form['task'] = array('#type' => 'radios', '#title' => t('Task'), '#options' => $tasks, '#required' => TRUE);
  $form['submit'] = array('#type' => 'submit', '#value' => t('Delete site data'));
  return $form;
}

/**
 * Perform various database cleanup tasks.
 */
function vozmob_util_truncate_form_submit($form, &$form_state) {
  if (empty($form_state['values']['confirm'])) {
    $form_state['rebuild'] = TRUE;
  }
  else {
    $form_state['redirect'] = 'admin';
    switch ($form_state['values']['task']) {
      case 'sandbox':
      case 'staging':
        vozmob_util_cleanup($form_state['values']['task']);
        break;
      case 'release':
        db_query('TRUNCATE {accesslog}');
        db_query('TRUNCATE {aggregator_item}');
        db_query('TRUNCATE {aggregator_category_item}');
        db_query('TRUNCATE {contact}');
        db_query('TRUNCATE {flood}');
        db_query('TRUNCATE {history}');
        db_query('TRUNCATE {notifications}');
        db_query('TRUNCATE {notifications_event}');
        db_query('TRUNCATE {notifications_fields}');
        db_query('TRUNCATE {notifications_queue}');
        db_query('TRUNCATE {notifications_sent}');
        db_query('TRUNCATE {mailhandler}');
        db_query('TRUNCATE {openid_association}');
        db_query('TRUNCATE {search_dataset}');
        db_query('TRUNCATE {search_index}');
        db_query('TRUNCATE {search_node_links}');
        db_query('TRUNCATE {search_total}');
        db_query('TRUNCATE {sessions}');
        db_query('TRUNCATE {url_alias}');
        module_load_include('admin.inc', 'comment');
        $result = db_query('SELECT cid FROM {comments}');
        while ($comment = db_fetch_object($result)) {
          _comment_delete_thread($comment);
        }
        $result = db_query('SELECT nid FROM {node}');
        while ($node = db_fetch_object($result)) {
          node_delete($node->nid);
        }
        $result = db_query('SELECT uid FROM {users} WHERE uid > 1');
        while ($user = db_fetch_object($result)) {
          user_delete(array(), $user->uid);
        }
        db_query('DELETE FROM {sms_user} WHERE uid = 1');
        $user = user_load(array('uid' => 1));
        $user->name = uniqid('VozMob', TRUE);
        $user->mail = '';
        $user->init = '';
        $user->pass = md5('Change m3');
        $foo = drupal_write_record('users', $user, 'uid');
        variable_del('drupal_private_key');
        variable_del('ffmpeg_error_email');
        variable_del('site_mail');
        variable_del('site_name');
        variable_del('site_slogan');
        variable_del('mma_cron_notify_email');
        variable_del('sms_email_gateway_mail_domain');
        variable_del('sms_registration_address');
        variable_del('vozmob_support_messaging_mms_from_node');
        db_query("DELETE FROM {system} WHERE type = '%s' AND name = '%s'", 'module', 'vozmob_util');
        db_query("DELETE FROM {files} WHERE filepath NOT LIKE '%s'", '%/banlist/%');
        db_query('TRUNCATE {media_mover_files}');
        db_query('TRUNCATE {cache}');
        db_query('TRUNCATE {cache_block}');
        db_query('TRUNCATE {cache_content}');
        db_query('TRUNCATE {cache_filter}');
        db_query('TRUNCATE {cache_form}');
        db_query('TRUNCATE {cache_location}');
        db_query('TRUNCATE {cache_media_mover}');
        db_query('TRUNCATE {cache_menu}');
        db_query('TRUNCATE {cache_page}');
        db_query('TRUNCATE {cache_update}');
        db_query('TRUNCATE {cache_views}');
        db_query('TRUNCATE {cache_views_data}');
        db_query('TRUNCATE {watchdog}');
        drupal_set_message('Database has been prepped for release.');
        break;
    }
  }
}

function vozmob_util_cleanup($site) {
  db_query('TRUNCATE {mailhandler}');
  variable_del('drupal_private_key');
  variable_del('ffmpeg_error_email');
  variable_del('site_mail');
  variable_set('site_name', 'VozMob ' . $site);
  variable_set('site_slogan', 'Mobile Voices ' . $site);
  variable_del('mma_cron_notify_email');
  variable_del('sms_email_gateway_mail_domain');
  variable_del('sms_registration_address');
  variable_del('vozmob_support_messaging_mms_from_node');
  drupal_set_message(t('Database has been prepped for %site.', array('%site' => $site)));
}
