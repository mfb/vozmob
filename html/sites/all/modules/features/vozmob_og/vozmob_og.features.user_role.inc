<?php

/**
 * Implementation of hook_user_default_roles().
 */
function vozmob_og_user_default_roles() {
  $roles = array();

  // Exported role: editor
  $roles['editor'] = array(
    'name' => 'editor',
  );

  // Exported role: sysadmin
  $roles['sysadmin'] = array(
    'name' => 'sysadmin',
  );

  return $roles;
}
