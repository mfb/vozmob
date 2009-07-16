<?php
// $Id: twitter.views_default.inc,v 1.1.2.2 2009/07/02 14:08:39 eaton Exp $

function twitter_views_default_views() {
  $view = new view;
  $view->name = 'tweets';
  $view->description = 'Displays Twitter.com status messages for users who have associated Twitter accounts.';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'twitter';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Tweets', 'default');
  $handler->override_option('fields', array(
    'text' => array(
      'label' => '',
      'exclude' => 0,
      'id' => 'text',
      'table' => 'twitter',
      'field' => 'text',
      'relationship' => 'none',
    ),
    'created_time' => array(
      'label' => '',
      'date_format' => 'time ago',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'created_time',
      'table' => 'twitter',
      'field' => 'created_time',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'created_time' => array(
      'order' => 'DESC',
      'id' => 'created_time',
      'table' => 'twitter',
      'field' => 'created_time',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All author',
      'title' => '%1\'s tweets',
      'default_argument_type' => 'user',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'relationship' => 'none',
      'default_argument_user' => 1,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'page' => 0,
        'review' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'protected' => array(
      'operator' => '=',
      'value' => 0,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'protected',
      'table' => 'twitter_account',
      'field' => 'protected',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
    'role' => array(),
    'perm' => '',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('use_pager', 'mini');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'profile_image_url' => 'profile_image_url',
      'text' => 'text',
      'created_time' => 'text',
    ),
    'info' => array(
      'profile_image_url' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'text' => array(
        'sortable' => 0,
        'separator' => ' &#8212; ',
      ),
      'created_time' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler = $view->new_display('page', 'User page', 'page');
  $handler->override_option('fields', array(
    'profile_image_url' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'profile_image_url',
      'table' => 'twitter_account',
      'field' => 'profile_image_url',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'text' => array(
      'label' => '',
      'exclude' => 0,
      'id' => 'text',
      'table' => 'twitter',
      'field' => 'text',
      'relationship' => 'none',
    ),
    'created_time' => array(
      'label' => '',
      'date_format' => 'time ago',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'created_time',
      'table' => 'twitter',
      'field' => 'created_time',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('path', 'user/%/tweets');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Twitter',
    'description' => '',
    'weight' => '1',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
  ));
  $handler = $view->new_display('block', 'User block', 'block');
  $handler->override_option('items_per_page', 5);
  $handler->override_option('use_more', 1);
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('row_options', array(
    'inline' => array(
      'text' => 'text',
      'created_time' => 'created_time',
    ),
    'separator' => ' &#8212; ',
  ));
  $handler->override_option('block_description', 'User Tweets');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('block', 'Global block', 'block2');
  $handler->override_option('arguments', array());
  $handler->override_option('items_per_page', 5);
  $handler->override_option('use_more', 1);
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('row_options', array(
    'inline' => array(
      'text' => 'text',
      'created_time' => 'created_time',
    ),
    'separator' => ' &#8212; ',
  ));
  $handler->override_option('block_description', 'All Tweets');
  $handler->override_option('block_caching', -1);

  return array('tweets' => $view);
}
