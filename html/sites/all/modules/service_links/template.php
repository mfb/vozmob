<?php
// $Id: template.php,v 1.3.4.3 2009/06/07 16:11:33 thecrow Exp $

/**
 * @file
 * Example template.php for service_links.module (put the name of your theme instead of 'themename')
 * Use <?php print $service_links ?> to insert links in your node.tpl.php or you page.tpl.php file.
 */

function themename_preprocess_page(&$vars) {
  if (module_exists('service_links')) {
    $vars['service_links'] = theme('links', service_links_render($vars['node'], TRUE));
  }
}

function themename_preprocess_node(&$vars) {
  if (module_exists('service_links')) {
    $vars['service_links'] = theme('links', service_links_render($vars['node'], TRUE));
  }
}

/**
 * If something don't work well try this
 */
function themename_preprocess(&$vars, $hook) {
  switch ($hook) {
    case 'node':
      $vars['service_links'] = theme('links', service_links_render($vars['node'], TRUE));
      break;
    case 'page':
      $vars['service_links'] = theme('links', service_links_render($vars['node'], TRUE));
      break;
  }
}
