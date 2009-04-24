<?php
// $Id$

// SUPPORTED GATEWAYS:
// 'msg.acsalaska.com' => t('Alaska Communications Systems'),
// 'message.alltel.com' => t('Alltel Wireless'),
// 'txt.att.net' => t('AT&T/Cingular'),
// 'myboostmobile.com' => t('Boost Mobile'),
// 'mobile.celloneusa.com' => t('CellularOne'),
// 'cwemail.com' => t('Centennial Wireless'),
// 'sms.mycricket.com' => t('Cricket'),
// 'messaging.sprintpcs.com' => t('Helio'),
// 'mymetropcs.com' => t('Metro PCS'),
// 'page.nextel.com' => t('Nextel'),
// 'qwestmp.com' => t('Qwest'),
// 'messaging.sprintpcs.com' => t('Sprint'),
// 'tmomail.net' => t('T-Mobile'),
// 'vmobl.com' => t('Virgin Mobile'),
// 'vtext.com' => t('Verizon'),
// SUPPORTED ALIASES:
// mms.att.net => AT&T
// messaging.nextel.com => Nextel
// vzwpix.com => Verizon

/**
 * Implementation of hook_mailsave_clean().
 * Attempt to match sender to a phone number stored by sms_user module.
 * If no match found and sender is a recognized SMS email gateway, 
 * create a user account.
 */
function registration_mailsave_clean(&$node, $result, $i, $header, $mailbox) {

  watchdog('debug', 'start registration mailsave clean');
  // If there is already a node created
  // The sms_user and sms_email_gateway modules are required.
  if ($node->uid && module_exists('sms_user') && module_exists('sms_email_gateway')) {
  watchdog('debug', '');
    list($from_address, $from_name) = mailhandler_get_fromaddress($header, $mailbox);
    list($to_address, $to_name) = registration_get_toaddress($header, $mailbox);
    $from = explode('@', $from_address);
    $to  = explode('@', $to_address);
    $email_registration_accounts = array(
      'vozmob.sandbox.reg' => 'vozmob.sandbox.reg'
    );
    $email_gateway_aliases = array(
      'messaging.nextel.com' => 'page.nextel.com',
      'mms.att.net' => 'txt.att.net',
      'vzwpix.com' => 'vtext.com',
    );
    if (isset($email_gateway_aliases[$from[1]])) {
      $from[1] = $email_gateway_aliases[$from[1]];
    }

    $number = $from[0];

    registration_mailsave_receive($account, $number);
    
    watchdog('debug', print_r($account, TRUE));
    watchdog('debug', print_r($number, TRUE));

    watchdog('debug','next the watchdogs for the if');
    watchdog('debug', print_r($account->uid, TRUE));
    watchdog('debug', print_r($from[1], TRUE));
    watchdog('debug', print_r($to_address, TRUE));
    //if it comes from a valid sms email gateway and it was sent to one of the email registration accounts
    $sms_email_gateway_providers = sms_email_gateway_sms_email_gateway_providers();
    if ($account->uid && isset($sms_email_gateway_providers[$from[1]]) && isset($email_registration_accounts[$to[0]])) {
    	$user = user_save($account, array('name' => 'nuevoNombre'), 'account');
	watchdog('debug', print_r($user));
	if ($user) {
		watchdog('debug', 'user was saved succesfully');
     		//$password = dechex(mt_rand()) . dechex(mt_rand());
		//Send message confirming with password and instruction
	        //sms_send($number, $message, $options = array())
	        //$number = "6073515559";
	        $message = "Your blog was created. Your password is ".$user->pass;
		$options = array('carrier'=> $from[1], 'from' => 'vozmob');
	    	sms_send($number, $message, $options);
	}
    }


    }
  watchdog('debug', 'end registration hookup');
}

/**
 * Report back module information when requested.
 */
function registration_mailsave_clean_info() {
  $items['registration'] = array(
    'provider'    => t('Registration through SMS'),
    'country'     => 'US',
    'author'      => 'Gabriela Rodriguez',
    'description' => t('Authenticate users with the SMS framework and create new accounts when SMS messages for registration are received.'),
  );
  return $items;
}

/**
 * Given a number, search for an associated user account.
 */
function registration_mailsave_receive(&$account, $number) {
  // Following comments and code are from the now-deprecated sms_user_sms_receive().
  // how is this be handled with international numbers? do we need to remove +44 for UK?
  // should be able to test when we get Chinese SIM card
  if (substr($number, 0, 1) == '1') {// remove leading '1', sms_user doesn't store it...
    $number = substr($number, 1);
  }
  $uid = sms_user_get_uid($number);
  $account = user_load(array('uid' => $uid, 'status' => 1));
}
 
 function registration_get_toaddress($header, $mailbox) {
  if (($toheader = strtolower($mailbox['mailto'])) && isset($header->$toheader)) {
    $to = $header->$toheader;
  }
  else {
    $to = $header->to;
  }
  watchdog('debug','dentro del get to address');
  watchdog('debug',print_r($to,TRUE));
  return array($to[0]->mailbox .'@'. $to[0]->host, $to[0]->personal);
}