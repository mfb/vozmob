<?php
// $Id: userreference.views.inc,v 1.1.2.1 2008/09/05 15:53:12 yched Exp $

/**
 * @file
 * Interface between userreference.module and views.module.
 */

/**
 * Implementation of hook_views_data_alter().
 */
function userreference_views_data_alter(&$data) {
  // Add a string filter handler to the users/name field for use with autocomplete.
  $data['users']['name']['filter'] = array('handler' => 'views_handler_filter_string');
}

