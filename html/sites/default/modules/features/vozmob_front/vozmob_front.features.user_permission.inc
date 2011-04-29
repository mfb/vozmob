<?php

/**
 * Implementation of hook_user_default_permissions().
 */
function vozmob_front_user_default_permissions() {
  $permissions = array();

  // Exported permission: access taxonomy images
  $permissions['access taxonomy images'] = array(
    'name' => 'access taxonomy images',
    'roles' => array(
      '0' => 'anonymous user',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: administer taxonomy images
  $permissions['administer taxonomy images'] = array(
    'name' => 'administer taxonomy images',
    'roles' => array(
      '0' => 'sysadmin',
    ),
  );

  // Exported permission: can disable taxonomy images
  $permissions['can disable taxonomy images'] = array(
    'name' => 'can disable taxonomy images',
    'roles' => array(),
  );

  return $permissions;
}
