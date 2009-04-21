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
  // The sms_user and sms_email_gateway modules are required.
  if (!$node->uid && module_exists('sms_user') && module_exists('sms_email_gateway')) {
    list($from_address, $from_name) = mailhandler_get_fromaddress($header, $mailbox);
    list($to_address, $to_name) = registration_get_toaddress($header, $mailbox);
    $from = explode('@', $from_address);
    $to  = explode('@', $to_address);
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

    //if (isset($node->title)) {
    //	$node->title = 'Nueva Historia / New Story';
    //}

    //if it comes from a valid sms email gateway
    /*$sms_email_gateway_providers = sms_email_gateway_sms_email_gateway_providers();
    if (!$account->uid && isset($sms_email_gateway_providers[$from[1]])) {
     $user = FALSE;
     $password = dechex(mt_rand()) . dechex(mt_rand());
     while (!$user) { 
      $user = user_save(NULL, array(
        'name' => implode('-', array('vozmob-registration', mt_rand(1,10000))),
        'pass' => $password,
        'mail' => $from_address,
        'roles' => drupal_map_assoc(array(3)),
        'status' => 1,
        'sms_user' => array(array('number' => sms_formatter($from[0]), 'status' => 1, 'gateway' => array('carrier' => $from[1]))),
    ));
    }
    }

    //Send message confirming with password and instruction
    //sms_send($number, $message, $options = array())
    //$number = "6073515559";
    $message = "Your blog was created. Your password is .... .";
    $options = array('carrier'=> $from[1], 'from' => 'vozmob');
    sms_send($number, $message, $options);


    //send question "do you want to have a blog in vozmob"
    //if yes
    //   while blog_name is not unique 
    //	 	what is the name of your blog?
    //		change the username to the one they choose
    //		generate temporary password 
    //  	send message confirming , sending password and instructions
     */
  }
}

/**
 * Report back module information when requested.
 */
function registration_mailsave_clean_info() {
  $items['sms'] = array(
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
    $from = $header->to;
  }
  return array($to[0]->mailbox .'@'. $to[0]->host, $to[0]->personal);
}
