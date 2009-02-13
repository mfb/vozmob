<?php
// $Id: devel_generate.drush.inc,v 1.1.2.4 2009/02/06 05:28:01 weitzman Exp $

/**
 * @file
 *  Generate content, taxonomy, and users via drush framework.
 */

/**
 * Implementation of hook_drush_help().
 */
function devel_generate_drush_help($section) {
  switch ($section) {
    case 'drush:generate users':
      return dt('Usage: drush [options] generate users <number_users> <kill>. <kill> is optional, Specify "kill" if you want to delete the users first.');
    case 'drush:generate taxonomy':
      return dt('Usage: drush [options] generate tax <number_vocab> <number_terms> <kill>. <kill> is optional; specify "kill" if you want to delete all taxonomy first.');
    case 'drush:generate content':
      return dt('Usage: drush [options] generate content <number_nodes> <number_comments>. <kill> is optional; specify "kill" if you want to delete all content first.');
  }
}

/**
 * Implementation of hook_drush_command().
 */
function devel_generate_drush_command() {
  $items['generate users'] = array(
    'callback' => 'drush_generate_users',
    'description' => 'Creates users.',
    'arguments' => array(
      'n' => 'Number of users to generate.',
    ),
    'options' => array(
      'kill' => 'Specify \'kill\' to delete all users before generating new ones.',
    ),
  );
  $items['generate taxonomy'] = array(
    'callback' => 'drush_generate_tax',
    'description' => 'Creates taxonomy.',
    'arguments' => array(
      'n' => 'Vocabulary ID into which new terms will be inserted.',
    ),
    'options' => array(
      'kill' => 'Specify \'kill\' to delete all terms in specified vocab before generating.'
    ),
    
  );
  $items['generate content'] = array(
    'callback' => 'drush_generate_nodescontent',
    'description' => 'Creates content.',
    'arguments' => array(
      'n' => 'Number of nodes to generate.',
      'c' => 'Number of comments to generate.',
    ),
    'options' => array(
      'kill' => 'Specify \'kill\' to delete all content before generating new content.'
    ),
  );
  return $items;
}


/**
 * Command callback. Generate a number of users.
 */
function drush_generate_users($num_users = NULL) {
  if (drush_generate_is_number($num_users) == FALSE) {
    drush_error(t('Invalid number of users.'));
  }
  drush_generate_include_devel();
  devel_create_users($num_users, drush_get_option('kill'));
  drush_print(t('Generated @number users.', array('@number' => $num_users)));
}

/**
 * Command callback. Generate a number of taxonomy.
 */
function drush_generate_tax($num_vocab = NULL, $num_terms = NULL) {
  if (drush_generate_is_number($num_vocab) == FALSE) {
    drush_error(t('Invalid vocabulary ID.'));
  }
  if (drush_generate_is_number($num_terms) == FALSE) {
    drush_error(t('Invalid number of terms.'));
  }
  drush_generate_include_devel();
  devel_generate_taxonomy_data($num_vocab, $num_terms, '12', drush_get_option('kill'));
  drush_print(t('Generated @num_vocab vocabulary, @num_terms terms', array('@num_vocab' => $num_vocab, '@num_terms' => $num_terms)));
}

/**
 * Command callback. Generate a number of content.
 */
function drush_generate_nodescontent($num_nodes = NULL, $num_comments = NULL) {
  if (drush_generate_is_number($num_nodes) == FALSE) {
    drush_error(t('Invalid number of nodes'));
  }
  if (!empty($num_comments) && drush_generate_is_number($num_comments) == FALSE) {
    drush_error(t('Invalid number of comments.'));
  }

  // Let's load user 1, seems to be need for creating comments.
  global $user;
  $user_one = user_load(array('uid' => '1'));
  $user = $user_one;
  drupal_save_session(FALSE);
  
  if ($kill == 'kill') $values['values']['kill_content'] = 1;
  $values['values']['title_length'] = '8';
  $values['values']['num_nodes'] = $num_nodes;
  $values['values']['num_comments'] = $num_comments;
  $values['values']['node_types'] = array('page' => 'page', 'article' => 'article');
  drush_generate_include_devel();
  devel_generate_content($values);
  drush_print(t('Generated @num_nodes nodes, @num_comments comments per node', array('@num_nodes' => $num_nodes, '@num_comments' => $num_comments)));
}

//////////////////////////////////////////////////////////////////////////////
// Helper functions

// verify if param is a number
function drush_generate_is_number($number) {
  if ($number == NULL) return FALSE;
  if (!is_numeric($number)) return FALSE;
  if ($number < 0) return FALSE;
  return TRUE;
}

// include devel_generate.inc
function drush_generate_include_devel() {
  $path = drupal_get_path('module', 'devel_generate');
  require_once($path .'/devel_generate.inc');
}