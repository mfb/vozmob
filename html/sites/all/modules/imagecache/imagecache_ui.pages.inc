<?php
// $Id: imagecache_ui.pages.inc,v 1.2 2009/04/14 00:01:21 drewish Exp $

/**
 * Preset Admin callbacks and required functions.
 */
function imagecache_ui_preset_overview() {
  $header = array(t('Preset Name'), t('Storage'), t('Actions'));
  $rows = array();
  // Always clear the preset cache on this display.
  foreach (imagecache_presets(TRUE) as $preset) {
    $row = array();
    $row[] = l($preset['presetname'], 'admin/build/imagecache/'. $preset['presetid']);
    $links = array();
    switch ($preset['storage']) {
      case IMAGECACHE_STORAGE_DEFAULT:
        $row[] = t('Default');
        $links[] = l(t('View'), 'admin/build/imagecache/'. $preset['presetid']);
        $links[] = l(t('Flush'), 'admin/build/imagecache/'. $preset['presetid'] .'/flush' );
        break;
      case IMAGECACHE_STORAGE_OVERRIDE:
        $row[] = t('Override');
        $links[] = l(t('Edit'), 'admin/build/imagecache/'. $preset['presetid']);
        $links[] = l(t('Revert'), 'admin/build/imagecache/'. $preset['presetid'] .'/delete');
        $links[] = l(t('Flush'), 'admin/build/imagecache/'. $preset['presetid'] .'/flush' );
        $links[] = l(t('Export'), 'admin/build/imagecache/'. $preset['presetid'] .'/export' );
        break;
      case IMAGECACHE_STORAGE_NORMAL:
        $row[] = t('Normal');
        $links[] = l(t('Edit'), 'admin/build/imagecache/'. $preset['presetid']);
        $links[] = l(t('Delete'), 'admin/build/imagecache/'. $preset['presetid'] .'/delete');
        $links[] = l(t('Flush'), 'admin/build/imagecache/'. $preset['presetid'] .'/flush' );
        $links[] = l(t('Export'), 'admin/build/imagecache/'. $preset['presetid'] .'/export' );
        break;
    }
    $row[] = implode('&nbsp;&nbsp;&nbsp;&nbsp;', $links);
    $rows[] = $row;
  }
  $output = theme('table', $header, $rows);
  return $output;
}


function imagecache_ui_preset_add_form($form_state) {
  $form = array();
  $form['presetname'] = array(
    '#type' => 'textfield',
    '#size' => '64',
    '#title' => t('Preset Namespace'),
    '#default_value' => '',
    '#description' => t('The namespace is used in URLs for images to tell imagecache how to process an image. Please only use alphanumeric characters, underscores (_), and hyphens (-) for preset names.'),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create New Preset'),
  );
  return $form;
}

function imagecache_ui_preset_add_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  // Check for duplicates
  foreach (imagecache_presets() as $preset) {
    if ($values['presetname'] == $preset['presetname']) {
      form_set_error('presetname', t('The namespace you have chosen is already in use.'));
      break;
    }
  }

  // Check for illegal characters in preset names
  if (preg_match('/[^0-9a-zA-Z_\-]/', $values['presetname'])) {
    form_set_error('presetname', t('Please only use alphanumeric characters, underscores (_), and hyphens (-) for preset names.'));
  }
}

function  imagecache_ui_preset_add_form_submit($form, &$form_state) {
  $preset = array('presetname' => $form_state['values']['presetname']);
  $preset = imagecache_preset_save($preset);
  drupal_set_message(t('Preset %name (ID: @id) was created.', array('%name' => $preset['presetname'], '@id' => $preset['presetid'])));
  $form_state['redirect'] = 'admin/build/imagecache/'. $preset['presetid'];
}


function imagecache_ui_preset_delete_form($form_state, $preset = array()) {
  if (empty($preset)) {
    drupal_set_message(t('The specified preset was not found'), 'error');
    drupal_goto('admin/build/imagecache');
  }

  $form = array();
  $form['presetid'] = array('#type' => 'value', '#value' => $preset['presetid']);
  return confirm_form(
    $form,
    t('Are you sure you want to delete the preset %preset?',
      array('%preset' => $preset['presetname'])
    ),
    'admin/build/imagecache',
    t('This action cannot be undone.'),
    t('Delete'),  t('Cancel')
  );
}

function imagecache_ui_preset_delete_form_submit($form, &$form_state) {
  $preset = imagecache_preset($form_state['values']['presetid']);
  imagecache_preset_delete($preset);
  drupal_set_message(t('Preset %name (ID: @id) was deleted.', array('%name' => $preset['presetname'], '@id' => $preset['presetid'])));
  $form_state['redirect'] = 'admin/build/imagecache';
}


function imagecache_ui_preset_flush_form(&$form_state, $preset = array()) {
  if (empty($preset)) {
    drupal_set_message(t('The specified preset was not found'), 'error');
    $form_state['redirect'] = 'admin/build/imagecache';
  }

  $form = array();
  $form['presetid'] = array('#type' => 'value', '#value' => $preset['presetid']);
  return confirm_form(
    $form,
    t('Are you sure you want to flush the preset %preset?',
      array('%preset' => $preset['presetname'])
    ),
    'admin/build/imagecache',
    t('This action cannot be undone.'),
    t('Flush'),  t('Cancel')
  );
}

function imagecache_ui_preset_flush_form_submit($form, &$form_state) {
  $preset = imagecache_preset($form_state['values']['presetid']);
  imagecache_preset_flush($preset);
  drupal_set_message(t('Preset %name (ID: @id) was flushed.', array('%name' => $preset['presetname'], '@id' => $preset['presetid'])));
  $form_state['redirect'] = 'admin/build/imagecache';
}


/**
 * Imagecache preset export form.
 */
function imagecache_ui_preset_export_form(&$form_state, $preset = array()) {
  if (empty($preset)) {
    drupal_set_message(t('The specified preset was not found'), 'error');
    $form_state['redirect'] = 'admin/build/imagecache';
  }

  if (isset($preset['presetid'])) {
    unset($preset['presetid']);
  }
  if (isset($preset['storage'])) {
    unset($preset['storage']);
  }
  foreach ($preset['actions'] as $id => $action) {
    unset($preset['actions'][$id]['actionid']);
    unset($preset['actions'][$id]['presetid']);
  }
  $exported = '$presets = array();';
  $exported .= "\n";
  $exported .= '$presets[\''. $preset['presetname'] .'\'] = ';
  $exported .= var_export($preset, TRUE) .';';
  $rows = substr_count($exported, "\n") + 1;

  $form = array();
  $form['export'] = array(
    '#type' => 'textarea',
    '#default_value' => $exported,
    '#rows' => $rows,
    '#resizable' => FALSE,
  );
  return $form;
}


function imagecache_ui_preset_form($form_state, $preset = array()) {
  if (empty($preset)) {
    drupal_set_message(t('The specified preset was not found.'), 'error');
    drupal_goto('admin/build/imagecache');
  }

  $form = array();
  // @TODO: for some reason, simply setting '#disabled' on the normal presetname
  // textfield fails to pass the values successfully to the submit handler.
  if ($preset['storage'] === IMAGECACHE_STORAGE_DEFAULT) {
    $form['presetname'] = array(
      '#type' => 'value',
      '#value' => $preset['presetname'],
    );
    $form['presetname_display'] = array(
      '#type' => 'textfield',
      '#size' => '64',
      '#title' => t('Preset Namespace'),
      '#default_value' => $preset['presetname'],
      '#disabled' => TRUE,
    );
  }
  else {
    $form['presetname'] = array(
      '#type' => 'textfield',
      '#size' => '64',
      '#title' => t('Preset Namespace'),
      '#default_value' => $preset['presetname'],
      '#description' => t('The namespace is used in URL\'s for images to tell imagecache how to process an image. Please only use alphanumeric characters, underscores (_), and hyphens (-) for preset names.'),
      '#validate' => array('imagecache_element_presetname_validate' => array()),
    );
  }

  $form['presetid'] = array(
    '#type' => 'value',
    '#value' => $preset['presetid'],
  );

  $form['actions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Actions'),
    '#tree' => TRUE,
    '#theme' => 'imagecache_ui_preset_actions',
  );

  foreach ($preset['actions'] as $i => $action) {
    // skip unknown actions...
    if (!$definition = imagecache_action_definition($action['action'])) {
      continue;
    }
    $action_name =  t($definition['name']);
    $action_form['name'] = array(
      '#value' => $action_name,
    );

    $action_form['action'] = array(
      '#type' => 'value',
      '#value' => $action['action'],
    );
    $action_form['actionid'] = array(
      '#type' => 'value',
      '#value' => $action['actionid'],
    );
    $action_form['presetid'] = array(
      '#type' => 'value',
      '#value' => $action['presetid'],
    );

    $action_form['settings'] = array(
      '#theme' => $action['action'],
      '#value' => $action['data'],
    );
    $action_form['data'] = array(
      '#type' => 'value',
      '#value' => $action['data'],
    );
    $action_form['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $action['weight'],
      '#access' => $preset['storage'] !== IMAGECACHE_STORAGE_DEFAULT,
    );
    $action_form['configure'] = array(
      '#value' => l(t('Configure'), 'admin/build/imagecache/'. $action['presetid'] .'/'. $action['actionid'] ),
      '#access' => $preset['storage'] !== IMAGECACHE_STORAGE_DEFAULT,
    );
    $action_form['remove'] = array(
      '#value' => l(t('Delete'), 'admin/build/imagecache/'. $action['presetid'] .'/'. $action['actionid'] .'/delete'),
      '#access' => $preset['storage'] !== IMAGECACHE_STORAGE_DEFAULT,
    );
    $form['actions'][$i] = $action_form;
  }

  $form['actions']['new'] = array(
    '#tree' => FALSE,
    '#type' => 'fieldset',
    '#title' => t('New Actions'),
    '#collapsible' => TRUE,
    '#collapsed' => count($preset['actions']) > 0,
    '#access' => $preset['storage'] !== IMAGECACHE_STORAGE_DEFAULT,
  );


  foreach (imagecache_action_definitions() as $action => $definition) {
    $form['actions']['new'][] = array(
      '#type' => 'markup',
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#value' => l(t('Add !action', array('!action' => $definition['name'])),
                    'admin/build/imagecache/'.  $preset['presetid'] .'/add/'. $action) .
                    ' - '. $definition['description'],
    );
  }

/**
 @todo: 404/403 image per preset.
 @todo: global 404/403 image.

  $form['files'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' =>  t('Error Files'),
  );

  $form['files']['403']['file'] = array(
    '#type' => 'file',
    '#title' => t('403 Image'),
    '#description' => t('Image that will be used when access is denied to the source image.'),
  );

  $path403 = imagecache/'. $preset['presetname'] .'/403.png';
  if (file_exists($path403)) {
    $url403 =  imagecache_create_url($preset['presetname'], $path403);

    $form['files']['403']['view'] = array(
      '#value' => '<img src="'. $url403 .'">',
    );
  }

  $form['files']['404'] = array(
    '#type' => 'file',
    '#title' => t('404 Image'),
    '#description' => t('Image that will be used when the source image cannot be found.'),
  );
  */

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $preset['storage'] === IMAGECACHE_STORAGE_DEFAULT ? t('Override Defaults') : t('Update Preset'),
  );

  // Display a preview image for this action. Put it below the submit button so
  // users don't have to do a bunch of scrolling.
  $preview_path = file_create_path('imagecache_sample.png');
  if (!file_exists($preview_path)) {
    // Copy of the image into the files directory so rather than generating it
    // from the original so we don't end up creating subdirectories mirroring
    // the path to the this module.
    $preview_path = drupal_get_path('module', 'imagecache_ui') .'/sample.png';
    file_copy($preview_path, 'imagecache_sample.png');
  }
  imagecache_image_flush($preview_path);
  $imagecache_path = imagecache_create_url($preset['presetname'], $preview_path, TRUE);
  $form['preview'] = array(
    '#type' => 'item',
    '#title' => t('Preview'),
    '#value' => l($imagecache_path, $imagecache_path) .'<br />'. l("<img src='$imagecache_path' />", $imagecache_path, array('html' => TRUE)),
  );

  return $form;
}

function imagecache_ui_preset_form_submit($form, &$form_state) {
  // Save the preset first to retrieve the presetid when overriding
  $preset = imagecache_preset_save($form_state['values']);

  // Populate the presetid as needed for overrides
  if (isset($form_state['values']['actions'])) {
    foreach($form_state['values']['actions'] as $action) {
      if (empty($action['presetid'])) {
        $action['presetid'] = $preset['presetid'];
      }
      imagecache_action_save($action);
    }
  }

  // Push back to the preset form
  $form_state['redirect'] = 'admin/build/imagecache/'. $preset['presetid'];

  // Clear preset cache
  imagecache_presets(TRUE);
}

function theme_imagecache_ui_preset_actions($form) {
  $header = array(t('Action'), t('Settings'));
  // $header = array(, t('Weight'), '','');
  $rows = array();
  foreach(element_children($form) as $key) {
    if (!is_numeric($key)) {
      continue;
    }
    $row = array();
    $row[] = drupal_render($form[$key]['name']);
    $row[] = drupal_render($form[$key]['settings']);

    // Check for form access before rendering these portions of the table / header
    if (!empty($form[$key]['weight']['#access'])) {
      if (empty($header['weight'])) {
        $header['weight'] = t('Weight');
      }
      $form[$key]['weight']['#attributes']['class'] = 'imagecache-action-order-weight';
      $row[] = drupal_render($form[$key]['weight']);
    }
    if (!empty($form[$key]['configure']['#access'])) {
      if (empty($header['configure'])) {
        $header['configure'] = '';
      }
      $row[] = drupal_render($form[$key]['configure']);
    }
    if (!empty($form[$key]['remove']['#access'])) {
      if (empty($header['remove'])) {
        $header['remove'] = '';
      }
      $row[] = drupal_render($form[$key]['remove']);
    }
    $rows[] = array(
      'data' => $row,
      'class' => 'draggable',
    );
  }
  $output = theme('table', $header, $rows, array('id' => 'imagecache-preset-actions'));
  drupal_add_tabledrag('imagecache-preset-actions', 'order', 'sibling', 'imagecache-action-order-weight');
  $output .= drupal_render($form);
  return $output;
}


function imagecache_ui_action_form($form_state, $preset, $action) {
  $definitions = imagecache_action_definitions();

  if (empty($action)) {
    drupal_set_message(t('Unknown Action.'), 'error');
    drupal_goto('admin/build/imagecache');
  }

  if (empty($preset)) {
    drupal_set_message(t('Unknown Preset.'), 'error');
    drupal_goto('admin/build/imagecache');
  }

  $form = array(
    '#tree' => TRUE,
  );

  $form['actionid'] = array(
    '#type' => 'value',
    '#value' => $action['actionid'],
  );


  if (!empty($definitions[$action['action']]['file'])) {
    require_once($definitions[$action['action']]['file']);
  }
  $form['data'] = call_user_func($action['action'] .'_form', $action['data']);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update Action'),
  );
  return $form;
}

function imagecache_ui_action_form_submit($form, &$form_state) {
  if ($action = imagecache_action($form_state['values']['actionid'])) {
    $action = array_merge($action, $form_state['values']);
    imagecache_action_save($action);
    drupal_set_message(t('The action was succesfully updated.'));
    $form_state['redirect'] = 'admin/build/imagecache/'. $action['presetid'];
  }
  else {
    drupal_set_message(t('Unknown Action: @action_id', array('@action_id' => $form_state['values']['actionid'])));
    $form_state['redirect'] = 'admin/build/imagecache';
  }
}


function imagecache_ui_action_delete_form($form_state, $preset = array(), $action = array()) {
  if (empty($action)) {
    drupal_set_message(t('Unknown Action.'), 'error');
    drupal_goto('admin/build/imagecache');
  }
  if (empty($preset)) {
    drupal_set_message(t('Unknown Preset.'), 'error');
    drupal_goto('admin/build/imagecache');
  }

  $form = array();
  $form['actionid'] = array('#type' => 'value', '#value' => $action['actionid']);
  return confirm_form(
    $form,
    t('Are you sure you want to delete the !action action from preset !preset?',
      array('!preset' => $preset['presetname'], '!action' => $action['name'])
    ),
    'admin/build/imagecache',
    t('This action cannot be undone.'),
    t('Delete'),  t('Cancel')
  );
}

function imagecache_ui_action_delete_form_submit($form, &$form_state) {
  $action = imagecache_action($form_state['values']['actionid']);
  imagecache_action_delete($action);
  drupal_set_message(t('The action has been deleted.'));
  $form_state['redirect'] = 'admin/build/imagecache/'. $action['presetid'];
}


function imagecache_ui_action_add_form($form_state, $preset, $actionname) {
  $definition = imagecache_action_definition($actionname);

  $form = array(
    '#tree' => TRUE,
  );
  $form['action'] = array(
    '#type' => 'value',
    '#value' => $actionname,
  );
  $form['presetid'] = array(
    '#type' => 'value',
    '#value' => $preset['presetid'],
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
  );

  $form['data'] = call_user_func($actionname .'_form', array());
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add Action'),
  );
  return $form;
}

function imagecache_ui_action_add_form_submit($form, &$form_state) {
  imagecache_action_save($form_state['values']);
  $form_state['redirect'] = 'admin/build/imagecache/'. $form_state['values']['presetid'];
}
