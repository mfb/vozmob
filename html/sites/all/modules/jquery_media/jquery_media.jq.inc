<?php
// $Id: jquery_media.jq.inc,v 1.4.2.1 2008/06/30 14:26:55 aaron Exp $

/**
 * @file
 * The jQ include file for jQuery Media.
 *
 * This file is loaded when called by jQ.
 */

 /**
 *  Implement hook_jq (hook supplied by the jQ module)
 *
 *  The preferred method to invoke this plugin is to use the jQ module, available at http://drupal.org/project/jq
 *  and then call jq_add('jquery_media'); on the pages requiring this.
 *
 *  If you don't wish the jQ module installed, then use jquery_media_add() instead.
 *
 *  This gives the info for the plugin, plus adds any required settings on invocation.
 *  Also adds the required settings form items.
 */
function jquery_media_jq($op, $plugin = NULL, $options = array()) {
  switch ($op) {
    case 'info':
      return array(
          'jquery_media' => array(
              'name' => t('jQuery Media'),
              'description' => t('By M. Alsup. The jQuery Media Plugin supports unobtrusive conversion of standard markup into rich media content. It can be used to embed virtually any media type, including Flash, Quicktime, Windows Media Player, Real Player, MP3, Silverlight, PDF and more, into a web page. The plugin converts an element (usually a link) into a div which holds the object, embed or iframe tags neccessary to render the media content.'),
              'version' => '0.81 (06/05/2008)',
              'url' => 'http://www.malsup.com/jquery/media/',
              'invocation' => t('You may invoke this plugin with %code. You may further set additional default parameters by passing an array, such as with %array (which will override the defaults on this screen). See !help for more information.', array('%code' => "jq_add('jquery_media')", '%array' => "jq_add('jquery_media', array('bgcolor' => '#660000', 'autoplay' => TRUE))", '!help' => l(t('jQuery Media help'), 'admin/help/jquery_media'))),
              'files' => array(
                  'js' => array(
                      drupal_get_path('module', 'jquery_media') .'/js/jquery.media.js',
                  ),
              ),
          ),
      );
      break;
    case 'add':
      _jquery_media_add($options);
      break;
    case 'settings':
      include_once(drupal_get_path('module', 'jquery_media') .'/jquery_media.settings.inc');
      return _jquery_media_settings_form();
      break;
  }
}

/**
 *  This actually adds the jQuery Media plugin to a page.
 *
 *  @param $options
 *  An array of any default overrides, in a keyed array.
 *    'flvplayer' => The path to the flvplayer.
 *    'mp3player' => The path to the mp3player.
 *    'autoplay' => TRUE or FALSE to autoplay the media.
 *    'bgcolor' => The bgcolor, in #rrggbb format.
 *    'flashvars' => A string to pass to the flashvars of the media player.
 *    'params' => A string to pass to the params of the media player.
 *    'invoke media' => If TRUE, then we automatically the media with the class(es) specified.
 *    'media class' => A string of one or more CVS classes, in .notation, separated by commas, to invoke our plugin.
 *    'media width' => The width to invoke our media.
 *    'media height' => The height to invoke our media.
 *  @param $return_js
 *    If TRUE, then we return the js. Otherwise, by default, we add it inline with drupal_add_js.
 */
function _jquery_media_add($options = array(), $return_js = FALSE) {
  static $defaults = array();
  static $jquery_blockui_installed = FALSE;

  $options = (array)$options;
  if (empty($defaults) && !$return_js && variable_get('jquery_media_use_default_js_file', JQUERY_MEDIA_USE_DEFAULT_JS_FILE)) {
    drupal_add_js(variable_get('jquery_media_default_js_filepath', JQUERY_MEDIA_DEFAULT_JS_FILEPATH));
    $default_js_loaded = TRUE;
  }

  if (empty($defaults) || !empty($options)) {
    if ($swf = variable_get('jquery_media_swfobject_path', '')) {
      drupal_add_js($swf);
    }
    // change jQuery Media's defaults. these may be overrriden on an individual basis.
    if (!($default_js_loaded || isset($defaults['flvplayer'])) || $options['flvplayer']) {
      $defaults['flvplayer'] = $options['flvplayer'] ? url($options['flvplayer']) : url(variable_get('jquery_media_flvplayer', JQUERY_MEDIA_FLVPLAYER_DEFAULT));
      $js .= "    $.fn.media.defaults.flvPlayer = '{$defaults['flvplayer']}';\n";
    }
    if (!($default_js_loaded || isset($defaults['mp3player'])) || $options['mp3player']) {
      $defaults['mp3player'] = $options['mp3player'] ? url($options['mp3player']) : url(variable_get('jquery_media_mp3player', JQUERY_MEDIA_MP3PLAYER_DEFAULT));
      $js .= "    $.fn.media.defaults.mp3Player = '{$defaults['mp3player']}';\n";
    }
    if (!($default_js_loaded || isset($defaults['autoplay'])) || isset($options['autoplay'])) {
      $defaults['autoplay'] = $options['autoplay'] ? $options['autoplay'] : variable_get('jquery_media_autoplay', JQUERY_MEDIA_AUTOPLAY_DEFAULT);
      if ($defaults['autoplay'] != JQUERY_MEDIA_AUTOPLAY_DEFAULT) {
        $js .= "    $.fn.media.defaults.autoplay = {$defaults['autoplay']};\n";
      }
    }
    if (!($default_js_loaded || isset($defaults['bgcolor'])) || $options['bgcolor']) {
      $defaults['bgcolor'] = $options['bgcolor'] ? $options['bgcolor'] : variable_get('jquery_media_bgcolor', JQUERY_MEDIA_BGCOLOR_DEFAULT);
      if ($defaults['bgcolor'] != JQUERY_MEDIA_BGCOLOR_DEFAULT) {
        $js .= "    $.fn.media.defaults.bgColor = '{$defaults['bgcolor']}';\n";
      }
    }
    if (!($default_js_loaded || isset($defaults['flashvars'])) || $options['flashvars']) {
      $defaults['flashvars'] = $options['flashvars'] ? $options['flashvars'] : variable_get('jquery_media_flashvars', '');
      if ($defaults['flashvars']) {
        $js .= "    $.fn.media.defaults.flashvars = { {$defaults['flashvars']} };\n";
      }
    }
    if (!($default_js_loaded || isset($defaults['params'])) || $options['params']) {
      $defaults['params'] = $options['params'] ? $options['params'] : variable_get('jquery_media_params', '');
      if ($defaults['params']) {
        $js .= "    $.fn.media.defaults.params = { {$defaults['params']} };\n";
      }
    }
    if (!($default_js_loaded || isset($defaults['invoke media'])) || $options['invoke media'] || $options['media class'] || $options['media width'] || $options['media height']) {
      $defaults['invoke media'] = $options['invoke media'] ? $options['invoke media'] : variable_get('jquery_media_class_media_autoinvoke', JQUERY_MEDIA_CLASS_MEDIA_AUTOINVOKE_DEFAULT);
      if ($defaults['invoke media']) {
        $defaults['media class'] = $options['media class'] ? $options['media class'] : variable_get('jquery_media_class_media', JQUERY_MEDIA_CLASS_MEDIA_DEFAULT);
        $defaults['media width'] = $options['media width'] ? $options['media width'] : variable_get('jquery_media_media_width', JQUERY_MEDIA_MEDIA_WIDTH_DEFAULT);
        $defaults['media height'] = $options['media height'] ? $options['media height'] : variable_get('jquery_media_media_height', JQUERY_MEDIA_MEDIA_HEIGHT_DEFAULT);
        $size = array();
        if (!($defaults['media width'] === '')) {
          $size[] = 'width: '. $defaults['media width'];
        }
        if (!($defaults['media height'] === '')) {
          $size[] = 'height: '. $defaults['media height'];
        }
        $list = implode(', ', $size);
        $list = $list ? " { $list } " : '';
        if ($defaults['media class']) {
          $js .= "    $('{$defaults['media class']}').media($list);\n";
        }
      }
    }
    if ($js) {
      $js = $js = "\nif (Drupal.jsEnabled) {\n  $(document).ready(function() {\n$js  });  \n}\n";
    }
    if ($return_js == TRUE) {
      return $js;
    }
    if ($js) {
      drupal_add_js($js, 'inline');
    }
  }
}

/**
 *  Print the default js file.
 */
function _jquery_media_default_js_file() {
  $js = _jquery_media_add(array(), TRUE);
  header('Content-Length: '. strlen($js));
  header('Content-Type: text/javascript');
  header('Date: '. date('r'));
  print $js;
}
