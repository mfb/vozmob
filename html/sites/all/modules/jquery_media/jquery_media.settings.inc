<?php
// $Id: jquery_media.settings.inc,v 1.5.2.6 2008/08/28 01:47:17 aaron Exp $

/**
 * @file
 * The settings and help pages for jQuery Media.
 */

/**
 *  Callback for /admin/settings/jquery_media
 */
function _jquery_media_settings() {
  if (!module_exists('jq')) {
    drupal_set_message(t('You may wish to install and enable the !jq for a more centralized administration and development of jQuery plugin User Interface modules such as jQuery Media.', array('!jq' => l(t('jQ Module'), 'http://drupal.org/project/jq', array('target' => '_blank')))));
  }
  else {
    drupal_set_message(t('You may wish to configure these settings at the !jq.', array('!jq' => l(t('jQ administrative settings page'), 'admin/settings/jq'))));
  }
  return system_settings_form(_jquery_media_settings_form());
}

/**
 *  Callback for jquery_media_help
 */
function _jquery_media_help($section) {
  switch ($section) {
    case 'admin/help#jquery_media':
      $configure_link = module_exists('jq') ? l(t('configuring'), 'admin/settings/jq') : l(t('configuring'), 'admin/settings/jquery_media');
      $invoke_code = module_exists('jq') ? "jq_add('jquery_media')" : "jquery_media_add()";
      $override_code = module_exists('jq') ? "jq_add('jquery_media', array('bgcolor' => '#33FF00'));" : "jquery_media_add(array('bgcolor' => '#33FF00'));";
      $output .= '<p>'. t('The %jquery plugin will turn links to media into their embedded media equivalents. Installing and !configuring the module is only the first step: you must also manually theme the output of links in order to enjoy the benefits of this plugin.', array('%jquery' => t('jQuery Media'), '!configuring' => $configure_link)) .'</p>';
      $output .= '<p>'. t('Theming is outside of the scope of this help page. You might look at Drupal\'s !drupal for more information. However, once you are ready to theme your elements, you only need to add a class and invoke this plugin. For instance, if you have a File Field with a video, simply use something like the following: %print', array('!drupal' => l(t("Theme Developer's Guide"), 'http://drupal.org/theme-guide/5', array('target' => '_blank')), '%print' => t("<?php !invoke; print l(\$node->title, \$node->field_file_field[0]['filepath'], array('class' => 'media')); ?>", array('!invoke' => $invoke_code)))) .'</p>';
      $output .= '<p>'. t('You may override the global defaults of this plugin when !configuring. Additionally, you may override a global default on a case-by-case basis by including the override in an options array when invoking the plugin. For instance, you might override the background color of players on a specific page with %override. This will override the defaults of the plugin for every media link on that page, which may in turn override those individually (more on that later).', array('!configuring' => $configure_link, '%override' => $override_code)) .'</p>';
      $list[] = t("'flvplayer' => The path to the Flash Video Player to be used, such as the JW FLV Player or Wimpy Rave.");
      $list[] = t("'mp3player' => The path to the MP3 Audio Player to be used, such as the JW FLV Player or Wimpy MP3 Player.");
      $list[] = t("'media width' => The width the media will be displayed as.");
      $list[] = t("'media height' => The height the media will be displayed as.");
      $list[] = t("'autoplay' => If TRUE, then play the media as soon as loading it.");
      $list[] = t("'bgcolor' => This will be the background color for the media player, in hexidecimal code (#RRGGBB).");
      $list[] = t("'flashvars' => This is a raw string that will be sent to the flashvars. Advanced usage. An example might be %example. Be careful when setting this, as the string is unfiltered before sending to javascript.", array('%example' => "myVar1: 'myValue1', myVar2: 'myValue2'"));
      $list[] = t("'params' => This is a raw string that will be sent to the params for the embedded media. An example might be %example. Be careful when setting this, as the string is unfiltered before sending to javascript.", array('%example' => "myParam1: 'myValue1', myParam2: 'myValue2'"));
      $list[] = t("'invoke media' => If TRUE, then cause all links with the 'media class' to automatically be turned into embedded media when invoking the plugin.");
      $list[] = t("'media class' => The class that will be turned into embedded media on invocation. It uses jQuery CSS notation, so should be something like %filefield or %media for links.", array('%filefield' => '.filefield-file a', '%media' => 'a.media'));
      $override_list = theme('item_list', $list);
      $output .= '<p>'. t('The full array of global overrides are as follows: !list', array('!list' => $override_list)) .'</p>';
      $output .= '<p>'. t('Finally, you may override a global setting on an individual basis. The customizations here are extensive, and rather than repeating that here, you are directed to the !jquery_media for more information. As an example, you might set the size of a specific file with the following, while leaving any other jquery media links on the page at their default. (Note that we change the class away from the default \'media\' in this example. It also assumes we\'ve already invoked it as above.): %code', array('%code' => "<?php print l(\$node->title, \$node->field_audio_file, array('class' => 'media-audio')); drupal_add_js(\"$('a.media-audio').media( { width: 200, height: 15, autoplay: true } )\", 'inline'); ?>", '!jquery_media' => l(t('jQuery Media homepage'), 'http://www.malsup.com/jquery/media/', array('target' => '_blank')))) .'</p>';
      $output .= '<p>'. t('Note that by default, the module will add its default overrides inline. However, you may set it to output them to a dynamic javascript file, and may additionally wish to set the defaults to a static js file, once development is complete. You can alter those settings when !configuring the module.', array('!configuring' => $configure_link)) .'</p>';
      return $output;
  }
}

/**
 *  These are the settings to control global defaults for the jquery media plugin.
 *
 *  If we have jQ installed, they will be available at /admin/settings/jq, in a collapsed fieldset.
 *  They're also available at /admin/settings/jquery_media.
 */
function _jquery_media_settings_form() {
  $form = array();
  $form['nodes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node types'),
    '#description' => t('This plugin will be automatically activated when displaying nodes of the following types. Note that this will only enable the plugin for teasers and full node page views. Other uses, such as in Views, may require manual invocations.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['nodes']['jquery_media_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Types'),
    '#default_value' => variable_get('jquery_media_node_types', array()),
    '#options' => array_map('check_plain', node_get_types('names')),
    '#description' => t('A list of node types for which you want to automatically invoke this plugin on display.'),
  );
  $form['pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
    '#description' => t('The plugin will be automatically invoked on the following pages.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['pages']['jquery_media_pages'] = array(
    '#type' => 'textarea',
    '#title' => t('Pages'),
    '#default_value' => variable_get('jquery_media_pages', ''),
    '#description' => t('Automatically invoke jQuery Media on these pages. You may use * for a wildcard, such as %blog or %wild. Use %front for the front page. Enter one page per line.', array('%blog' => 'blog', '%wild' => 'blog/*', '%front' => '<front>')),
  );
  $form['classes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Classes'),
    '#description' => t('Set your media links to one of these configurable classes.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['classes']['media'] = array(
    '#type' => 'fieldset',
    '#title' => t('Media'),
    '#collapsible' => TRUE,
    '#description' => t('Links with this class will be replaced with the appropriate embedded code.'),
  );
  $form['classes']['media']['jquery_media_class_media_autoinvoke'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto-invoke Media class'),
    '#default_value' => variable_get('jquery_media_class_media_autoinvoke', JQUERY_MEDIA_CLASS_MEDIA_AUTOINVOKE_DEFAULT),
    '#description' => t('If checked, then links with the media class (below) will be automatically replaced with embed code when %code is invoked. You may always manually run this with %override, even if this checkbox is not set.', array('%code' => "jq_add('jquery_media')", '%override' => "jq_add('jquery_media', array('invoke media' => TRUE))")),
  );
  $form['classes']['media']['jquery_media_class_media'] = array(
    '#type' => 'textfield',
    '#title' => t('Media class'),
    '#default_value' => variable_get('jquery_media_class_media', JQUERY_MEDIA_CLASS_MEDIA_DEFAULT),
    '#description' => t('Media elements with a class attribute of this will be replaced with the relevant object embed code. In general, you probably want something like %class, which would replace all links with that class. Separate multiple classes with commas, such as %example.', array('%class' => JQUERY_MEDIA_CLASS_MEDIA_DEFAULT, '%example' => 'a.media, .filefield-file a, .view-item-audio-clip a')),
  );
  $form['players'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default players'),
    '#description' => t('Setting these values will determine the default players that will be loaded by the registered media links.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['players']['jquery_media_flvplayer'] = array(
    '#type' => 'textfield',
    '#title' => t('Flash Player (%flvPlayer)', array('%flvPlayer' => 'flvPlayer')),
    '#default_value' => variable_get('jquery_media_flvplayer', JQUERY_MEDIA_FLVPLAYER_DEFAULT),
    '#description' => t('Enter the path relative to your webroot to your flash video player, such as for the JW Media Player or Wimpy Rave. Do not include the beginning slash.'),
  );
  $form['players']['jquery_media_mp3player'] = array(
    '#type' => 'textfield',
    '#title' => t('MP3 Player (%mp3Player)', array('%mp3Player' => 'mp3Player')),
    '#default_value' => variable_get('jquery_media_mp3player', JQUERY_MEDIA_MP3PLAYER_DEFAULT),
    '#description' => t('Enter the path relative to your webroot to your MP3 audio player, such as for the JW Media Player or Wimpy MP3. Do not include the beginning slash.'),
  );
  $form['default'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['default']['jquery_media_media_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Media Width'),
    '#default_value' => variable_get('jquery_media_media_width', JQUERY_MEDIA_MEDIA_WIDTH_DEFAULT),
    '#description' => t('The width, in pixels, of the media that will be displayed.'),
  );
  $form['default']['jquery_media_media_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Media Height'),
    '#default_value' => variable_get('jquery_media_media_height', JQUERY_MEDIA_MEDIA_HEIGHT_DEFAULT),
    '#description' => t('The height, in pixels, of the media that will be displayed.'),
  );
  $form['default']['jquery_media_autoplay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autoplay (%autoplay)', array('%autoplay' => 'autoplay')),
    '#default_value' => variable_get('jquery_media_autoplay', JQUERY_MEDIA_AUTOPLAY_DEFAULT),
    '#description' => t('If checked, then media loaded with jQuery Media will automatically start.'),
  );
  $form['default']['jquery_media_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#default_value' => variable_get('jquery_media_bgcolor', JQUERY_MEDIA_BGCOLOR_DEFAULT),
    '#description' => t('This color, in hexidecimal form (#RRGGBB), will be used as the default background color for invoked players.'),
  );
  if (module_exists('colorpicker')) {
    $form['default']['jquery_media_bgcolor']['#type'] = 'colorpicker_textfield';
    $form['default']['jquery_media_bgcolor']['#colorpicker'] = 'jquery_media_colorpicker';
    $form['default']['jquery_media_colorpicker'] = array(
      '#type' => 'colorpicker',
      '#title' => t('Background color picker'),
      '#description' => t('Click in this box to start picking your color.'),
    );
  }
  else {
    $form['default']['jquery_media_bgcolor']['#description'] .= t(' The !colorpicker, if enabled, gives a visual aid when selecting this color.', array('!colorpicker' => l(t('Colorpicker module'), 'http://drupal.org/project/colorpicker')));
  }
  $form['default']['jquery_media_swfobject_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to swfobject.js'),
    '#default_value' => variable_get('jquery_media_swfobject_path', ''),
    '#description' => t('jQuery Media will attempt to use the swfobject.js script if it is loaded. However, due to licensing requirements, that script may not be packaged in the Drupal code repository. Other modules may have need of it as well. Therefore, if you already have the script, or choose to download it from !url, then you may enter the path to the file here, including the swfobject.js filename. Note that jQuery Media should work without this script in most cases, but using it will provide for the most optimal cross-browser experience. Do not include the beginning slash.', array('!url' => l('http://code.google.com/p/swfobject/', 'http://code.google.com/p/swfobject/'))),
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['advanced']['jquery_media_flashvars'] = array(
    '#type' => 'textfield',
    '#title' => t('Flashvars'),
    '#default_value' => variable_get('jquery_media_flashvars', ''),
    '#description' => t("This is a string that will be sent to the flashvars for the embedded media. An example might be %example. Be careful when setting this, as the string is unfiltered before sending to javascript.", array('%example' => "myVar1: 'myValue1', myVar2: 'myValue2'")),
  );
  $form['advanced']['jquery_media_params'] = array(
    '#type' => 'textfield',
    '#title' => t('Params'),
    '#default_value' => variable_get('jquery_media_params', ''),
    '#description' => t("This is a string that will be sent to the params for the embedded media. An example might be %example. Be careful when setting this, as the string is unfiltered before sending to javascript.", array('%example' => "myParam1: 'myValue1', myParam2: 'myValue2'")),
  );
  $form['advanced']['jquery_media_use_default_js_file'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use default js file'),
    '#default_value' => variable_get('jquery_media_use_default_js_file', JQUERY_MEDIA_USE_DEFAULT_JS_FILE),
    '#description' => t('If checked, then jQuery Media will load a js file with the defaults, rather than setting them inline. You should probably leave this unchecked while developing, so you don\'t need to refresh the js file, as it is created dynamically (although can be overridden below).'),
  );
  $js_filepath = variable_get('jquery_media_default_js_filepath', JQUERY_MEDIA_DEFAULT_JS_FILEPATH);
  $form['advanced']['jquery_media_default_js_filepath'] = array(
    '#type' => 'textfield',
    '#title' => t('Default js filepath'),
    '#default_value' => $js_filepath,
    '#description' => t('The path, without a leading slash, to the js file that loads the jQuery Media default settings. If it is set to the default of %filepath, then that file will be created dynamically with the settings on this page. Once you have finished developing, you may wish to copy the output of that file to a static file and set its path here, to ensure proper caching and to reduce server load.', array('%filepath' => $js_filepath)),
  );
  return $form;
}

