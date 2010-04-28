<?php

// $Id;

define('MAIN_SERVER_PATH', '/admin/mm_runcontrol/announce');


/**
 * @file
 * This file runs the connection to the main server
 */


// get our configuration variables
require_once('remote_server.inc');

// run our script
remote_server_announce();

function remote_server_announce() {
	global $conf;

	// create a token to send to the main server
	$token = remote_server_create_token(remote_server_announce_data());

	// open a connection to the remote server
  $data = file_get_contents($conf['server_url'] . MAIN_SERVER_PATH .'?token=' . $token, r);

  print $data;

  // main server hands back data as a serialized array
  $data = unserialize($data);



}


/**
 * Helper function to create the data we need to send to the main server
 * @return unknown_type
 */
function remote_server_announce_data() {
	global $conf;

	$data = array(
	  'server_id' => $conf['server_id'],
	  'server_modules' => $conf['modules'],
	  'server_actions' => $conf['actions']
	);

	return $data;
}

/**
 * Ensure we have a sever id to deal with
 * @return string
 */
function remote_server_sid() {
  global $conf;
  if (! $conf['server_id']) {
    // use the server address as a fall back
    // this is a problem if the ID is not unique, but
    // we can not do much if the admin does not modify the value
    $conf['server_id'] = $_SERVER['SERVER_ADDR'];
  }
  return $conf['server_id'];
}



/**
 * Wrapper functon for generating a token
 *
 * Requires Mcrypt module for PHP
 *
 * @return String: The token, or empty string on failure. e.g. if Mcrypt is not installed.
 */
function remote_server_create_token($data = null) {
  if (! function_exists('mcrypt_module_open')) {
    return false;
  }

  if (! $data) {
    return false;
  }

  $td = mcrypt_module_open(MDHE_SSO_ALGORITHM, null, MDHE_SSO_MODE, null);
  $iv_size = mcrypt_enc_get_iv_size($td);
  $iv = mcrypt_create_iv($iv_size, MCRYPT_RAND);

  // Get the secret key value. Note that this key value SHOULD BE CHANGED!
  $key = variable_get('mm_run_control_secret_key', MM_RUN_CONTROL_SECRET_KEY);

  $string = substr(sha1($key), 0, mcrypt_enc_get_key_size($td));
  mcrypt_generic_init($td, $string, $iv);

  $enc = $iv . mcrypt_generic($td, serialize($data));
  $token = base64_encode($enc);
  mcrypt_module_close($td);
  return $token;
}