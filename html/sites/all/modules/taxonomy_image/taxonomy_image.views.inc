<?php
// $Id: taxonomy_image.views.inc,v 1.1.2.4 2008/09/13 22:37:47 nancyw Exp $

/**
 * @file
 * Views integration for Taxonomy Image module.
 */

/**
 * Implementation of hook_views_data().
 */
function taxonomy_image_views_data() {
  // Table definition.
  $data['term_image']['table']['group'] = t('Taxonomy');
//  $data['term_image']['table']['group'] = t('Taxonomy Image');

  // Joins.
  $data['term_image']['table']['join'] = array(
    'term_data' => array(
      // Links directly to term_data via tid.
      'left_field' => 'tid',
      'field' => 'tid',
    ),
     // Tell Views we can join with 'node'
    'node' => array(
      'left_table' => 'term_node', // or 'term_data'
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );

  // Fields.
  $data['term_image']['tid'] = array(
    'title' => t('Term image'),
    'help' => t("The image associated with a taxonomy term."),
    'field' => array(
      'handler' => 'views_handler_field_taxonomy_image',
    ),
  );
  $data['term_image']['tid_list'] = array(
    'title' => t('All term images'),
    'help' => t("All images associated with a node's taxonomy terms."),
    'field' => array(
      'handler' => 'views_handler_field_taxonomy_image_list',
    ),
  );

  return $data;
}
