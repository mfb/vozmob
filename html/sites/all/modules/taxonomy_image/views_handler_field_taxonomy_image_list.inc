<?php

/**
 * @file
 * Views integration for Taxonomy Image module.
 */

/**
 * Field handler to provide all taxonomy images from select vocabularies
 *
 * @ingroup views_handler_field_prerender_list
 */
class views_handler_field_taxonomy_image_list extends views_handler_field_prerender_list {
  function init(&$view, $options) {
    parent::init($view, $options);
    if ($view->base_table == 'node_revisions') {
      $this->additional_fields['vid'] = array('table' => 'node_revisions', 'field' => 'vid');
    }
    else {
      $this->additional_fields['vid'] = array('table' => 'node', 'field' => 'vid');
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['imagecache_preset'] = array('default' => '');
    $options['link_to_taxonomy'] = array('default' => TRUE);
    $options['limit'] = array('default' => FALSE);
    $options['vids'] = array('default' => array());

    return $options;
  }

  /**
   * Provide "link to term" option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    
    // If ImageCache module is found, add its presets as available options
    // for how to display the image.
    if (module_exists('imagecache')) {
      $raw_presets = imagecache_presets();
      $presets[''] = t('Default');
      foreach ($raw_presets as $preset_id => $preset_info) {
        $preset = $preset_info['presetname'];
        $presets[$preset] = $preset;
      }
      $form['imagecache_preset'] = array(
        '#type' => 'select',
        '#title' => t('ImageCache preset'),
        '#options' => $presets,
        '#default_value' => $this->options['imagecache_preset'],
      );
    }
    
    $form['link_to_taxonomy'] = array(
      '#title' => t('Link this field to its term page'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy']),
    );

    $form['limit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Limit terms by vocabulary'),
      '#default_value'=> $this->options['limit'],
    );

    $options = array();
    $vocabularies = taxonomy_get_vocabularies();
    foreach ($vocabularies as $voc) {
      $options[$voc->vid] = check_plain($voc->name);
    }

    $form['vids'] = array(
      '#prefix' => '<div><div id="edit-options-vids">',
      '#suffix' => '</div></div>',
      '#type' => 'checkboxes',
      '#title' => t('Vocabularies'),
      '#options' => $options,
      '#default_value' => $this->options['vids'],
      '#process' => array('expand_checkboxes', 'views_process_dependency'),
      '#dependency' => array('edit-options-limit' => array(TRUE)),
    );
  }

  /**
   * Add this term to the query
   */
  function query() {
    $this->add_additional_fields();
  }

  function pre_render($values) {
    $this->field_alias = $this->aliases['vid'];
    $vids = array();
    foreach ($values as $result) {
      if (!empty($result->{$this->aliases['vid']})) {
        $vids[] = $result->{$this->aliases['vid']};
      }
    }

    if ($vids) {
      $voc = '';
      if (!empty($this->options['limit']) && !empty($this->options['vids'])) {
        $voc = " AND td.vid IN (" . implode(', ', array_keys(array_filter($this->options['vids']))) . ")";
      }

       $result = db_query("SELECT tn.vid AS node_vid, td.* FROM {term_data} td INNER JOIN {term_node} tn ON td.tid = tn.tid INNER JOIN {term_image} ti ON ti.tid = tn.tid WHERE tn.vid IN (" . implode(', ', $vids) . ")$voc ORDER BY td.weight, td.name");

      while ($term = db_fetch_object($result)) {
        
        if ($this->options['imagecache_preset']) {
          $image = taxonomy_image_display($term->tid, NULL, $this->options['imagecache_preset']);
        }
        else {
          $image = taxonomy_image_display($term->tid);
        }
        
        if (empty($this->options['link_to_taxonomy'])) {
          $this->items[$term->node_vid][$term->tid] = $image;
        }
        else {
          $this->items[$term->node_vid][$term->tid] = l($image, taxonomy_term_path($term), array('html' => TRUE));
        }
      }
    }
  }
}
