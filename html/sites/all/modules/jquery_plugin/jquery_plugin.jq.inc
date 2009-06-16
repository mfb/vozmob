<?php
// $Id: jquery_plugin.jq.inc,v 1.1.2.17 2009/06/15 22:53:39 mfb Exp $

/**
 * Implementation of hook_jq().
 * http://drupal.org/project/jq
 */
function jquery_plugin_jq($op, $plugin = NULL) {
  if ($op == 'info') {
    $path = drupal_get_path('module', 'jquery_plugin');
    return array(
      'ajaxQueue' => array(
        'name' => t('Ajax Queue'),
        'description' => t('Ajax Queue is a plugin that helps to manage Ajax race conditions. When multiple Ajax requests are made in rapid succession, the results can be returned out of order. This can cause weird behavior in your application.'),
        'version' => '1.0',
        'url' => 'http://plugins.jquery.com/project/ajaxqueue',
        'files' => array(
          'js' => array(
            $path .'/jquery.ajaxQueue.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
      'cookie' => array(
        'name' => t('Cookie'),
        'description' => t('A simple, lightweight utility plugin for reading, writing and deleting cookies.'),
        'version' => '1.0',
        'url' => 'http://plugins.jquery.com/project/cookie',
        'files' => array(
          'js' => array(
            $path .'/jquery.cookie.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
      'cycle' => array(
        'name' => t('Cycle'),
        'description' => t('Easy to use light-weight slideshow implementation. It supports pause-on-hover, auto-stop, auto-fit, before/after callbacks, click triggers and many transition effects including fade, shuffle, scroll, turn and zoom. It also allows you to define and run your own custom transitions. In addition, it supports, but does not require, the Metadata Plugin and the Easing Plugin.'),
        'version' => '2.65',
        'url' => 'http://plugins.jquery.com/project/cycle',
        'files' => array(
          'js' => array(
            $path .'/jquery.cycle.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
      'delegate' => array(
        'name' => t('Delegate'),
        'description' => t('The Delegate plugin is now part of the Validate plugin, so is rarely used on its own.'),
        'version' => '1.0',
        'url' => 'http://plugins.jquery.com/project/validate',
        'files' => array(
          'js' => array(
            $path .'/jquery.delegate.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
      'expose' => array(
        'name' => t('Expose'),
        'description' => t('Make your HTML elements stand out.'),
        'version' => '1.0.3',
        'url' => 'http://plugins.jquery.com/project/expose',
        'files' => array(
          'js' => array(
            $path .'/jquery.expose.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
      'flashembed' => array(
        'name' => t('Flash embed'),
        'description' => t('A javascript tool that you can use to embed Flash objects to your website.'),
        'version' => '1.0.3',
        'url' => 'http://plugins.jquery.com/project/flashembed',
        'files' => array(
          'js' => array(
            $path .'/jquery.flashembed.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
      'imgzoom' => array(
        'name' => t('Image zoom'),
        'description' => t('Opens links that point to images by zooming the image out into an imgzoom element. Requires the Dimensions plugin.'),
        'version' => '1.2',
        'url' => 'http://plugins.jquery.com/project/imgzoom',
        'files' => array(
          'js' => array(
            $path .'/jquery.imgzoom.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
      'metadata' => array(
        'name' => t('Metadata'),
        'description' => t('This plugin is capable of extracting metadata from classes, random attributes, and child elements.'),
        'version' => '2.0',
        'url' => 'http://plugins.jquery.com/project/metadata',
        'files' => array(
          'js' => array(
            $path .'/jquery.metadata.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
      'overlay' => array(
        'name' => t('Overlay'),
        'description' => t('Overlay your HTML with eye candy.'),
        'version' => '1.0.4',
        'url' => 'http://plugins.jquery.com/project/overlay',
        'files' => array(
          'js' => array(
            $path .'/jquery.overlay.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
      'scrollable' => array(
        'name' => t('Scrollable'),
        'description' => t('Make your HTML elements move.'),
        'version' => '1.0.5',
        'url' => 'http://plugins.jquery.com/project/scrollable',
        'files' => array(
          'js' => array(
            $path .'/jquery.scrollable.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
      'tabs' => array(
        'name' => t('Tabs'),
        'description' => t('jQuery Tools tabs plugin.'),
        'version' => '1.0.1',
        'url' => 'http://plugins.jquery.com/project/tabs',
        'files' => array(
          'js' => array(
            $path .'/jquery.tabs.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
      'tooltip' => array(
        'name' => t('Tooltip'),
        'description' => t('jQuery Tools tooltip plugin.'),
        'version' => '1.0.2',
        'url' => 'http://plugins.jquery.com/project/tooltips',
        'files' => array(
          'js' => array(
            $path .'/jquery.tooltip.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
      'validate' => array(
        'name' => t('Validate'),
        'description' => t('Setup powerful client-side form validation. From a protoype in a few minutes to heavily customized validation rules, methods and messages.'),
        'version' => '1.5.3',
        'url' => 'http://plugins.jquery.com/project/validate',
        'files' => array(
          'js' => array(
            $path .'/jquery.validate.min.js',
          ),
          'css' => NULL,
        ),
        'invocation' => NULL,
      ),
    );
  }
}
