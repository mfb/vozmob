<?php
// $Id: twitter.api.php,v 1.1.2.1 2009/06/21 16:21:00 eaton Exp $

/**
 * @file
 * Provides API documentation for the Twitter module.
 */

/**
 * Retrieves what Twitter accounts the given user can post to.
 */
function hook_twitter_accounts($drupal_user, $full_access = FALSE) {
  $accounts = array();
  if (user_access('use global twitter account') &&
      ($name = variable_get('twitter_global_name', NULL)) &&
      ($pass = variable_get('twitter_global_password', NULL))) {

    $accounts[$name] = array(
      'screen_name' => $name,
      'password' => $pass,
    );
  }

  $sql = " SELECT ta.*, tu.uid, tu.password, tu.import FROM {twitter_user} tu ";
  $sql .= "LEFT JOIN {twitter_account} ta ON (tu.screen_name = ta.screen_name) ";
  $sql .= "WHERE tu.uid = %d";

  if ($full_access) {
    $sql .= " AND tu.password IS NOT NULL";
  }
  $args = array($drupal_user->uid);
  $results = db_query($sql, $args);

  while ($account = db_fetch_array($results)) {
    $accounts[$account['screen_name']] = $account;
  }
  return $accounts;
}

/**
 * Called when the status updates are cached.
 *
 * @param $status
 *   An array of all the status updates from the given user.
 */
function hook_twitter_status_update($status = array()) {

}