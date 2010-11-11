<?php
// $Id: bot_start.php,v 1.2.2.4.2.6 2010/05/10 12:41:24 morbus Exp $

/*
 * @file
 * Wrapper around the Drupal bootstrap and Net_SmartIRC. The first half is
 * devoted to fooling Drupal into thinking it's being run through the web,
 * and the other half inserts the IRC library into Drupal's hook system.
 */

set_time_limit(0); // ignore max_execution_time.
$script_name = array_shift($_SERVER['argv']);

// --root allows us to keep this script in the bot.module directory
// without moving it to the root of the Drupal install, and --url
// is required to trick Drupal into thinking it's a web request.
// --url should be the same thing as your Drupal $base_url.
while ($param = array_shift($_SERVER['argv'])) {
  switch ($param) {
    case '--root':
      $drupal_root = array_shift($_SERVER['argv']);
      is_dir($drupal_root) ? chdir($drupal_root) : exit("ERROR: $drupal_root not found.\n");
      break;

    case '--url':
      $drupal_base_url = parse_url(array_shift($_SERVER['argv']));
      if (!$drupal_base_url || !$drupal_base_url['host']) { exit("ERROR: No URL was passed via --url.\n"); }
      $drupal_base_url['path'] = isset($drupal_base_url['path']) ? $drupal_base_url['path'] : '/';
      $_SERVER['HTTP_HOST'] = $drupal_base_url['host']; // this is all very boring, no?
      $_SERVER['PHP_SELF'] = $drupal_base_url['path'] . '/' . $script_name;
      $_SERVER['REQUEST_URI'] = $_SERVER['SCRIPT_NAME'] = $_SERVER['PHP_SELF'];
      $_SERVER['REMOTE_ADDR'] = NULL; // any values here do rather...
      $_SERVER['REQUEST_METHOD'] = NULL; // ...odd things. uh huh.
      break;
  }
}

// load in our required libraries.
require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
require_once('Net/SmartIRC.php');

// prevent MySQL timeouts on slow channels.
db_query('SET SESSION wait_timeout = %d', 24*60*60);

// initialize the bot with some sane defaults.
global $irc; // allow it to be slurped by Drupal modules if need be.
$bot = new drupal_wrapper(); // wrapper that integrates with Drupal hooks.
$irc = new Net_SmartIRC(); // MmmmmmM. The IRC object itself. Magick happens here.
$irc->setDebug( variable_get('bot_debugging', 0) ? SMARTIRC_DEBUG_ALL : SMARTIRC_DEBUG_NONE );
// the (boolean) here is required, as Net_SmartIRC doesn't respect a FAPI checkbox value of 1, only TRUE.
$irc->setAutoReconnect((boolean) variable_get('bot_auto_reconnect', 1)); // reconnect to the server if disconnected.
$irc->setAutoRetry((boolean) variable_get('bot_auto_retry', 1)); // retry if a server connection fails.
$irc->setUseSockets((boolean) variable_get('bot_real_sockets', 1)); // socket_connect or fsockopen?
$irc->setChannelSyncing(TRUE); // keep a list of joined users per channel.

// send every message type the library supports to our wrapper class.
// we can automate the creation of these actionhandlers, but not the
// class methods below (only PHP 5 supports default methods easily).
$irc_message_types = array(
  'UNKNOWN',    'CHANNEL', 'QUERY',    'CTCP',        'NOTICE',       'WHO',
  'JOIN',       'INVITE',  'ACTION',   'TOPICCHANGE', 'NICKCHANGE',   'KICK',
  'QUIT',       'LOGIN',   'INFO',     'LIST',        'NAME',         'MOTD',
  'MODECHANGE', 'PART',    'ERROR',    'BANLIST',     'TOPIC',        'NONRELEVANT',
  'WHOIS',      'WHOWAS',  'USERMODE', 'CHANNELMODE', 'CTCP_REQUEST', 'CTCP_REPLY',
);

foreach ($irc_message_types as $irc_message_type) {
  $irc->registerActionhandler(constant('SMARTIRC_TYPE_' . $irc_message_type), '.*', $bot, 'invoke_irc_msg_' . strtolower($irc_message_type));
}

// set up a timers similar to Drupal's hook_cron(), multiple types. I would have
// liked to just pass a parameter to a single function, but SmartIRC can't do that.
$irc->registerTimehandler(300000, $bot, 'invoke_irc_bot_cron');           // 5 minutes.
$irc->registerTimehandler(60000, $bot,  'invoke_irc_bot_cron_faster');    // 1 minute.
$irc->registerTimehandler(15000, $bot,  'invoke_irc_bot_cron_fastest');   // 15 seconds.

// connect and begin listening.
$irc->connect(variable_get('bot_server', 'irc.freenode.net'), variable_get('bot_server_port', 6667));
$irc->login(variable_get('bot_nickname', 'bot_module'), variable_get('bot_nickname', 'bot_module') . ' :http://drupal.org/project/bot', 8, variable_get('bot_nickname', 'bot_module'), (variable_get('bot_password', '') != '') ? variable_get('bot_password', '') : NULL);

// channel joining has moved to bot_irc_bot_cron_fastest().
// read that function for the rationale, and what we gain from it.

$irc->listen(); // go into the forever loop - no code after this is run.
$irc->disconnect(); // if we stop listening, disconnect properly.

// pass off IRC messages to our modules via Drupal's hook system.
class drupal_wrapper {
  function invoke_irc_bot_cron(&$irc)                 { module_invoke_all('irc_bot_cron'); }
  function invoke_irc_bot_cron_faster(&$irc)          { module_invoke_all('irc_bot_cron_faster'); }
  function invoke_irc_bot_cron_fastest(&$irc)         { module_invoke_all('irc_bot_cron_fastest'); }
  function invoke_irc_msg_unknown(&$irc, &$data)      { module_invoke_all('irc_msg_unknown', $data); }
  function invoke_irc_msg_channel(&$irc, &$data)      { module_invoke_all('irc_msg_channel', $data); }
  function invoke_irc_msg_query(&$irc, &$data)        { module_invoke_all('irc_msg_query', $data); }
  function invoke_irc_msg_ctcp(&$irc, &$data)         { module_invoke_all('irc_msg_ctcp', $data); }
  function invoke_irc_msg_notice(&$irc, &$data)       { module_invoke_all('irc_msg_notice', $data); }
  function invoke_irc_msg_who(&$irc, &$data)          { module_invoke_all('irc_msg_who', $data); }
  function invoke_irc_msg_join(&$irc, &$data)         { module_invoke_all('irc_msg_join', $data); }
  function invoke_irc_msg_invite(&$irc, &$data)       { module_invoke_all('irc_msg_invite', $data); }
  function invoke_irc_msg_action(&$irc, &$data)       { module_invoke_all('irc_msg_action', $data); }
  function invoke_irc_msg_topicchange(&$irc, &$data)  { module_invoke_all('irc_msg_topicchange', $data); }
  function invoke_irc_msg_nickchange(&$irc, &$data)   { module_invoke_all('irc_msg_nickchange', $data); }
  function invoke_irc_msg_kick(&$irc, &$data)         { module_invoke_all('irc_msg_kick', $data); }
  function invoke_irc_msg_quit(&$irc, &$data)         { module_invoke_all('irc_msg_quit', $data); }
  function invoke_irc_msg_login(&$irc, &$data)        { module_invoke_all('irc_msg_login', $data); }
  function invoke_irc_msg_info(&$irc, &$data)         { module_invoke_all('irc_msg_info', $data); }
  function invoke_irc_msg_list(&$irc, &$data)         { module_invoke_all('irc_msg_list', $data); }
  function invoke_irc_msg_name(&$irc, &$data)         { module_invoke_all('irc_msg_name', $data); }
  function invoke_irc_msg_motd(&$irc, &$data)         { module_invoke_all('irc_msg_motd', $data); }
  function invoke_irc_msg_modechange(&$irc, &$data)   { module_invoke_all('irc_msg_modechange', $data); }
  function invoke_irc_msg_part(&$irc, &$data)         { module_invoke_all('irc_msg_part', $data); }
  function invoke_irc_msg_error(&$irc, &$data)        { module_invoke_all('irc_msg_error', $data); }
  function invoke_irc_msg_banlist(&$irc, &$data)      { module_invoke_all('irc_msg_banlist', $data); }
  function invoke_irc_msg_topic(&$irc, &$data)        { module_invoke_all('irc_msg_topic', $data); }
  function invoke_irc_msg_nonrelevant(&$irc, &$data)  { module_invoke_all('irc_msg_nonrelevant', $data); }
  function invoke_irc_msg_whois(&$irc, &$data)        { module_invoke_all('irc_msg_whois', $data); }
  function invoke_irc_msg_whowas(&$irc, &$data)       { module_invoke_all('irc_msg_whowas', $data); }
  function invoke_irc_msg_usermode(&$irc, &$data)     { module_invoke_all('irc_msg_usermode', $data); }
  function invoke_irc_msg_channelmode(&$irc, &$data)  { module_invoke_all('irc_msg_channelmode', $data); }
  function invoke_irc_msg_ctcp_request(&$irc, &$data) { module_invoke_all('irc_msg_ctcp_request', $data); }
  function invoke_irc_msg_ctcp_reply(&$irc, &$data)   { module_invoke_all('irc_msg_ctcp_reply', $data); }
}

