<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function vozmob_og_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_flag_default_flags().
 */
function vozmob_og_flag_default_flags() {
  $flags = array();
  // Exported flag: "Requires moderation".
  $flags['moderate_node'] = array(
    'content_type' => 'node',
    'title' => 'Requires moderation',
    'global' => '1',
    'types' => array(
      '0' => 'blog',
    ),
    'flag_short' => 'Post requires moderation',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Post has been moderated',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => 4,
      ),
      'unflag' => array(
        '0' => 4,
      ),
    ),
    'show_on_page' => 0,
    'show_on_teaser' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'i18n' => 0,
    'module' => 'vozmob_og',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  // Exported flag: "Moderated group".
  $flags['moderated_group'] = array(
    'content_type' => 'node',
    'title' => 'Moderated group',
    'global' => '1',
    'types' => array(
      '0' => 'group',
    ),
    'flag_short' => 'Make this a moderated group',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Make this an unmoderated group',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => '2',
      ),
      'unflag' => array(
        '0' => '2',
      ),
    ),
    'show_on_page' => 0,
    'show_on_teaser' => 0,
    'show_on_form' => 1,
    'access_author' => '',
    'i18n' => 0,
    'module' => 'vozmob_og',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function vozmob_og_imagecache_default_presets() {
  $items = array(
    'group_logo_large' => array(
      'presetname' => 'group_logo_large',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '200',
            'height' => '',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    'group_logo_small' => array(
      'presetname' => 'group_logo_small',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '50',
            'height' => '50',
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function vozmob_og_node_info() {
  $items = array(
    'group' => array(
      'name' => t('Group'),
      'module' => 'features',
      'description' => t('Create a new group.'),
      'has_title' => '1',
      'title_label' => t('Título'),
      'has_body' => '1',
      'body_label' => t('Cuerpo'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_rules_defaults().
 */
function vozmob_og_rules_defaults() {
  return array(
    'rules' => array(
      'rules_moderated_group_post' => array(
        '#type' => 'rule',
        '#set' => 'event_node_insert',
        '#label' => 'moderated group post',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'og',
          'vozmob_og' => 'vozmob_og',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#type' => 'condition',
            '#settings' => array(
              '#argument map' => array(
                'group' => 'node',
              ),
            ),
            '#name' => 'og_rules_condition_content_is_group_post',
            '#info' => array(
              'label' => 'Content is a group post',
              'arguments' => array(
                'group' => array(
                  'type' => 'node',
                  'label' => 'Group post',
                ),
              ),
              'module' => 'Organic groups',
            ),
            '#weight' => 0,
          ),
          '1' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Node parent(s) are flagged',
              'base' => 'ogrulesextra_rules_condition_flagged',
              'label callback' => 'ogrulesextra_rules_condition_flagged_label',
              'arguments' => array(
                'flag' => array(
                  'type' => 'flag',
                  'label' => 'Parent group flag',
                  'flag_type' => 'node',
                ),
                'object' => array(
                  'type' => 'node',
                  'label' => 'Flagged content',
                ),
                'user' => array(
                  'type' => 'user',
                  'label' => 'User on whose behalf to check',
                  'description' => 'For non-global flags, this is the user on whose behalf the flag is checked.',
                ),
              ),
              'module' => 'Organic groups',
            ),
            '#name' => 'ogrulesextra_rules_condition_flagged_node',
            '#settings' => array(
              'flag' => 'moderated_group',
              '#argument map' => array(
                'object' => 'node',
                'user' => 'user',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'flag' => 'moderate_node',
              'permission_check' => 0,
              '#argument map' => array(
                'object' => 'node',
                'flagging_user' => 'author',
              ),
            ),
            '#name' => 'flag_rules_action_flag_node',
            '#info' => array(
              'label' => 'Flag created content, under "Requires moderation"',
              'base' => 'flag_rules_action_flag',
              'label callback' => 'flag_rules_action_flag_label',
              'arguments' => array(
                'flag' => array(
                  'type' => 'flag',
                  'label' => 'Flag',
                  'flag_type' => 'node',
                ),
                'object' => array(
                  'type' => 'node',
                  'label' => 'Flagged content',
                ),
                'flagging_user' => array(
                  'type' => 'user',
                  'label' => 'User on whose behalf to flag',
                  'description' => 'For non-global flags, this is the user on whose behalf to flag the object. In addition, if checked below, the access permissions to the flag are checked against this user.',
                ),
              ),
              'module' => 'Flag',
            ),
            '#weight' => 0,
          ),
          '1' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Unpublish created content',
              'module' => 'Node',
              'arguments' => array(
                'node' => array(
                  'label' => 'Content',
                  'type' => 'node',
                ),
              ),
              'base' => 'rules_core_action_execute',
              'action_name' => 'node_unpublish_action',
              'configurable' => FALSE,
              'label callback' => 'rules_core_node_label_callback',
              'label_skeleton' => 'Unpublish @node',
            ),
            '#name' => 'rules_core_node_unpublish_action',
            '#settings' => array(
              'auto_save' => 1,
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'rules_moderated_group_post_good' => array(
        '#type' => 'rule',
        '#set' => 'event_flag_unflagged_moderate_node',
        '#label' => 'Moderated group post good',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'og',
          '1' => 'vozmob_og',
        ),
        '#status' => 'default',
        '#conditions' => array(),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'auto_save' => 1,
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_core_node_publish_action',
            '#info' => array(
              'label' => 'Publish flagged content',
              'module' => 'Node',
              'arguments' => array(
                'node' => array(
                  'label' => 'Content',
                  'type' => 'node',
                ),
              ),
              'base' => 'rules_core_action_execute',
              'action_name' => 'node_publish_action',
              'configurable' => FALSE,
              'label callback' => 'rules_core_node_label_callback',
              'label_skeleton' => 'Publish @node',
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
    ),
  );
}

/**
 * Implementation of hook_views_api().
 */
function vozmob_og_views_api() {
  return array(
    'api' => '3.0-alpha1',
  );
}
