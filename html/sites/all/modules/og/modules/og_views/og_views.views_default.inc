<?php
// $Id: og_views.views_default.inc,v 1.3 2008/10/29 19:59:45 dww Exp $

// Declare all the .view files in the views subdir that end in .view
function og_views_default_views() {
  $files = file_scan_directory(drupal_get_path('module', 'og_views'). '/views', '.view');
  foreach ($files as $absolute => $file) {
    require $absolute;
    $views[substr($file->name, 3)] = $view;
  }
  return $views;
}