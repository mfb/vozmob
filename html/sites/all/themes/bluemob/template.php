<?php

/**
 * Intercept page template variables
 *
 * @param $vars
 *   A sequential array of variables passed to the theme function.
 */
function bluemob_preprocess_page(&$vars) {
  global $user;
  $headers = drupal_set_header();
  if (strpos($headers, 'HTTP/1.1 403 Forbidden') && !$user->uid) {
    $vars['content'] .= "\n" . l(t('Please login to continue'), 'user/login', array('query' => drupal_get_destination()));
  }
}

/**
 * Intercept node template variables
 *
 * @param $vars
 *   A sequential array of variables passed to the theme function.
 */
function bluemob_preprocess_node(&$vars) {
  
  jquery_plugin_add('cycle', 'theme', 'header');
  jquery_plugin_add('expose');
  jquery_plugin_add('overlay');
  drupal_add_js(drupal_get_path('theme', 'bluemob') . '/scripts/cycle.js', 'theme');
  $node = $vars['node']; // for easy reference
  // for easy variable adding for different node types
  // jquery_plugin_add('jquery.sound');
  // drupal_add_js(drupal_get_path('theme', 'bluemob') . '/scripts/jquery.sound.js', 'theme');

  // Hackish way of adding video thumbnail into the node view.
  // This should get cleaned up (e.g. using CCK to render media mover video).
  $vars['overlay_launcher'] = FALSE;
  if (empty($node->field_image[0]['view']) &&  !empty($node->media_mover)) {
    foreach ($node->media_mover as $cid) {
      foreach ($cid as $mmfid) {
        if (substr($mmfid['complete_file'], -4, 4) == '.jpg') {
          $vars['overlay_launcher'] = TRUE;
          $vars['overlay_launcher_image'] = $mmfid['complete_file'];
          break 2;
        }
      }
    }
  }

  // Hackish way of adding audio overlay launcher into the node view.
  // This should get cleaned up (e.g. using CCK to render media mover audio).
  if (!$vars['overlay_launcher'] && empty($node->field_image[0]['view']) && !empty($node->media_mover)) {
    foreach ($node->media_mover as $cid) {
      foreach ($cid as $mmfid) {
        if (substr($mmfid['complete_file'], -4, 4) == '.mp3') {
          $vars['overlay_launcher'] = TRUE;
          $vars['overlay_launcher_image'] = drupal_get_path('theme', 'bluemob') . '/images/' . ($vars['teaser'] ? 'audio_icon_whitebg.gif' : 'audio_icon_large.gif');
          break 2;
        }
      }
    }
  }

  // split term by vocabulary and format appropriatly
  $vocabulary = array();
  foreach ($node->taxonomy as $tid => $term) {
    $vocabulary[$term->vid]['taxonomy_term_' . $term->tid] = array(
      'tid' => $term->tid,                  // views style
      'name' => $term->name,                // views style
      'title' => $term->name,               // taxonomy module style.
      'href' => taxonomy_term_path($term),  // taxonomy module style
       'attributes' => array(               // taxonomy module style
        'rel' => 'tag',
        'title' => strip_tags($term->description),
      ),
      'path' => taxonomy_term_path($term),  // views style
    );
  }
  // tags
  $vars['terms'] = theme('links', $vocabulary[1], array('class' => 'links inline'));
  if (module_exists('uploadterm') && module_exists('taxonomy_image')) {
    // media terms
    $term_image_links = array();
    foreach ($vocabulary[variable_get('uploadterm_vocabulary', 0)] as $term) {
      $term_image_links[] = l(taxonomy_image_display($term['tid']), $term['path'], array('html' => TRUE)); // can add size here
    }
    $vars['mediaterms'] = theme('item_list', $term_image_links, NULL, 'ul', array('class' => 'links inline media'));
  }

  switch ($node->type) {
    case 'page':
      break;
  //  case 'slideshow': //add the sound plugin
  //    jquery_plugin_add('jquery.sound');
  //    drupal_add_js(drupal_get_path('theme', 'bluemob') . '/scripts/jquery.sound.js', 'theme');
  }
}

/**
 * Displays file attachments in table
 *
 * @ingroup themeable
 */
function bluemob_upload_attachments($files) {
  $header = array(array('data' => t('Attachment'), 'colspan' => 2), t('Size'));
  $rows = array();
  foreach ($files as $file) {
    $file = (object)$file;
    if ($file->list && empty($file->remove)) {
      $href = file_create_url($file->filepath);
      $text = $file->description ? $file->description : $file->filename;
      $rows[] = array(bluemob_media_element($file, $href), l($text, $href), format_size($file->filesize));
    }
  }
  if (count($rows)) {
    return theme('table', $header, $rows, array('id' => 'attachments'));
  }
}

/**
 * Renders a video or audio element.
 */
function bluemob_media_element($file, $href) {
  // @fixme: due to firefox bugginess, sometimes WAV files are not decoded and page render stalls?
  // $elements = drupal_map_assoc(array('audio', 'video'));
  $elements = drupal_map_assoc(array('video'));
  list($element) = explode('/', $file->filemime);
  if (isset($elements[$element])) {
    return '<' . $element . ' controls="controls" src="' . check_url($href) . '" />';
  }
}

/**
 * View features: taxonomy image all.
 *
 * If there is no output add default image.
 */
function phptemplate_preprocess_views_view_field__tid_list(&$vars) {
  if ($vars['output'] == '') {
    $text = drupal_get_path('module', 'vozmob_media') . '/icons/text-tiny.png';
    $vars['output'] = theme('item_list', array(theme('image', $text)));
  }
}
