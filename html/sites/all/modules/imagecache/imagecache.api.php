<?php
// $Id: imagecache.api.php,v 1.1 2009/02/07 19:33:22 drewish Exp $

/**
 * @file
 * Hooks provided by the ImageCache module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Inform ImageCache about actions that can be performed on an image.
 *
 * @return array
 *   An array of information on the actions implemented by a module. The array
 *   contains a sub-array for each action node type, with the machine-readable
 *   action name as the key. Each sub-array has up to 3 attributes. Possible
 *   attributes:
 *     "name": the human-readable name of the action. Required.
 *     "description": a brief description of the action. Required.
 *     "file": the name of the include file the action can be found
 *             in relative to the implementing module's path.
 */
function hook_imagecache_actions() {
  $actions = array(
    'imagecache_resize' => array(
      'name' => 'Resize',
      'description' => 'Resize an image to an exact set of dimensions, ignoring aspect ratio.',
    ),
  );
}
