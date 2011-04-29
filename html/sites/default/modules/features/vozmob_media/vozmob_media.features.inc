<?php

/**
 * Implementation of hook_imagecache_default_presets().
 */
function vozmob_media_imagecache_default_presets() {
  $items = array(
    '160px' => array(
      'presetname' => '160px',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '160',
            'height' => '160',
            'upscale' => 0,
          ),
        ),
      ),
    ),
    '320px' => array(
      'presetname' => '320px',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale',
          'data' => array(
            'width' => '320',
            'height' => '320',
            'upscale' => 0,
          ),
        ),
      ),
    ),
  );
  return $items;
}
