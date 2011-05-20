<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function vozmob_og_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer flags
  $permissions['administer flags'] = array(
    'name' => 'administer flags',
    'roles' => array(
      '0' => 'sysadmin',
    ),
  );

  // Exported permission: administer mail2og
  $permissions['administer mail2og'] = array(
    'name' => 'administer mail2og',
    'roles' => array(),
  );

  // Exported permission: administer organic groups
  $permissions['administer organic groups'] = array(
    'name' => 'administer organic groups',
    'roles' => array(
      '0' => 'sysadmin',
    ),
  );

  // Exported permission: administer rules
  $permissions['administer rules'] = array(
    'name' => 'administer rules',
    'roles' => array(
      '0' => 'sysadmin',
    ),
  );

  // Exported permission: send to group
  $permissions['send to group'] = array(
    'name' => 'send to group',
    'roles' => array(),
  );

  // Exported permission: subscribe to content in groups
  $permissions['subscribe to content in groups'] = array(
    'name' => 'subscribe to content in groups',
    'roles' => array(
      '0' => 'sysadmin',
    ),
  );

  return $permissions;
}
