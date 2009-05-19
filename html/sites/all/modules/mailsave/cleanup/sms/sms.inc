<?php
// $Id$

/**
 * Implementation of hook_mailsave_clean().
 * Attempt to match sender to a phone number stored by sms_user module.
 * If no match found and sender is a recognized SMS email gateway, 
 * create a user account.
 */
function sms_mailsave_clean(&$node, $result, $i, $header, $mailbox) {


  list($to_address, $to_name) = registration_get_toaddress($header, $mailbox);
  $to  = explode('@', $to_address);
  $email_registration_accounts = array(
	'vozmob.sandbox.reg' => 'vozmob.sandbox.reg'
);
  // The node does not exist
  // The sms_user and sms_email_gateway modules are required.
  // The mail that this sms was sent to is not one of the registration accounts

  if (!$node->uid && module_exists('sms_user') && module_exists('sms_email_gateway') && !isset($email_registration_accounts[$to[0]])) {
    list($from_address, $from_name) = mailhandler_get_fromaddress($header, $mailbox);
    $from = explode('@', $from_address);

    // Recognize MMS e-mail gateways as valid SMS e-mail gateways.
    $from[1] = mms_email_gateway_rewrite_to_sms($from[1]);

    $number = $from[0];

    sms_mailsave_receive($node, $number);
    
    if (empty($node->title)) {
    	$node->title = 'Nueva Historia / New Story';
    }
    watchdog('debug', 'el titulo del nodo es ');
    watchdog('debug', print_r($node->title,TRUE));

    $lan = $node->language;
    watchdog('debug', 'language ');
    watchdog('debug', print_r($node->language, TRUE));

    //changed the name of the user that created the node to vozmob and a random number -- gaba
    //'name' => implode('-', array(dechex(mt_rand()), dechex(mt_rand()))),
    $sms_email_gateway_providers = sms_email_gateway_sms_email_gateway_providers();
    if (!$node->uid && isset($sms_email_gateway_providers[$from[1]])) {
     $user = FALSE;
     while (!$user) { 
      $user = user_save(NULL, array(
        'name' => implode('-', array('vozmob', mt_rand(1,10000))),
        'pass' => dechex(mt_rand()) . dechex(mt_rand()),
        'mail' => $from_address,
        'roles' => drupal_map_assoc(array(3)),
        'status' => 1,
	'language' => $lan,
        'sms_user' => array('number' => sms_formatter($from[0]), 'status' => 1, 'gateway' => array('carrier' => $from[1])),
    ));
    }
      $node->uid = $user->uid;

      if (module_exists('sms_registration')) {

	    if ($lan == "en") {
	        $message = "Thanks! Do you want to have a blog in vozmob? If yes please send one word identifying the blog.";
		$subject = "Media posted";
		}
	    else {
		$message = "Gracias! Quiere tener un blog en vozmob? Si es asi envie una palabra identificando su blog.";
		$subject = "Publicado";
		}
 watchdog('debug', print_r($message, TRUE));
	    $options = array('carrier'=> $from[1], 'from' => 'vozmob.sandbox.reg@gmail.com', 'subject'=>'Media posted');
    	    sms_send($number, $message, $options);
    	    //we will receive the answer in cleanup.registration.inc
     }

    }
    if ($node->uid) {
      mailhandler_switch_user($node->uid);
    }

  }
}

/**
 * Report back module information when requested.
 */
function sms_mailsave_clean_info() {
  $items['sms'] = array(
    'provider'    => t('SMS framework'),
    'country'     => 'US',
    'author'      => 'Mark Burdett',
    'description' => t('Authenticate users with the SMS framework and create new accounts when SMS messages are received.'),
  );
  return $items;
}

/**
 * Given a number, search for an associated user account and set some node parameters.
 */
function sms_mailsave_receive(&$node, $number) {
  // Following comments and code are from the now-deprecated sms_user_sms_receive().
  // how is this be handled with international numbers? do we need to remove +44 for UK?
  // should be able to test when we get Chinese SIM card
  if (substr($number, 0, 1) == '1') {// remove leading '1', sms_user doesn't store it...
    $number = substr($number, 1);
  }

  $uid = sms_user_get_uid($number);

  $account = user_load(array('uid' => $uid, 'status' => 1));
  $node->uid = $account->uid;
  $node->name = $account->name;

}

function sms_mailsave_get_toaddress($header, $mailbox) {

	if (($toheader = strtolower($mailbox['mailto'])) && isset($header->$toheader)) {
		$to = $header->$toheader;
	}
	else {
		$from = $header->to;
		}
	return array($to[0]->mailbox .'@'. $to[0]->host, $to[0]->personal);
}
