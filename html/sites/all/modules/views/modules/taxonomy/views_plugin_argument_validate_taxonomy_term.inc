<?php
// $Id: views_plugin_argument_validate_taxonomy_term.inc,v 1.4.2.7 2011/01/11 22:54:58 merlinofchaos Exp $
/**
 * @file
 * Contains the 'taxonomy term' argument validator plugin.
 */

/**
 * Validate whether an argument is an acceptable node.
 */
class views_plugin_argument_validate_taxonomy_term extends views_plugin_argument_validate {
  function option_definition() {
    $options = parent::option_definition();
    $options['vids'] = array('default' => array());
    $options['type'] = array('default' => 'tid');
    $options['transform'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $vocabularies = taxonomy_get_vocabularies();
    $options = array();
    foreach ($vocabularies as $voc) {
      $options[$voc->vid] = check_plain($voc->name);
    }

    $form['vids'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Vocabularies'),
      '#options' => $options,
      '#default_value' => $this->options['vids'],
      '#description' => t('If you wish to validate for specific vocabularies, check them; if none are checked, all terms will pass.'),
    );

    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Argument type'),
      '#options' => array(
        'tid' => t('Term ID'),
        'tids' => t('Term IDs separated by , or +'),
        'name' => t('Term name or synonym'),
        'convert' => t('Term name/synonym converted to Term ID'),
      ),
      '#default_value' => $this->options['type'],
      '#description' => t('Select the form of this argument; if using term name, it is generally more efficient to convert it to a term ID and use Taxonomy: Term ID rather than Taxonomy: Term Name" as an argument.'),
    );

    $form['transform'] = array(
      '#type' => 'checkbox',
      '#title' => t('Transform dashes in URL to spaces in term name arguments'),
      '#default_value' => $this->options['transform'],
    );
  }

  function options_submit($form, &$form_state, &$options) {
    // filter trash out of the options so we don't store giant unnecessary arrays
    $options['vids'] = array_filter($options['vids']);
  }

  function convert_options(&$options) {
    if (!isset($options['vids']) && !empty($this->argument->options['validate_argument_vocabulary'])) {
      $options['vids'] = $this->argument->options['validate_argument_vocabulary'];
      $options['type'] = $this->argument->options['validate_argument_type'];
      $options['transform'] = $this->argument->options['validate_argument_transform'];
    }
  }

  function validate_argument($argument) {
    $vids = array_filter($this->options['vids']);
    $type = $this->options['type'];
    $transform = $this->options['transform'];

    switch ($type) {
      case 'tid':
        if (!is_numeric($argument)) {
          return FALSE;
        }

        $result = db_fetch_object(db_query("SELECT * FROM {term_data} WHERE tid = %d", $argument));
        if (!$result) {
          return FALSE;
        }
        return empty($vids) || !empty($vids[$result->vid]);

      case 'tids':
        $tids = new stdClass();
        $tids->value = $argument;
        $tids = views_break_phrase($argument, $tids);
        if ($tids->value == array(-1)) {
          return FALSE;
        }

        $test = drupal_map_assoc($tids->value);
        $titles = array();

        // check, if some tids already verified
        static $validated_cache = array();
        foreach ($test as $tid) {
          if (isset($validated_cache[$tid])) {
            if ($validated_cache[$tid] === FALSE) {
              return FALSE;
            }
            else {
              $titles[] = $validated_cache[$tid];
              unset($test[$tid]);
            }
          }
        }


        // if unverified tids left - verify them and cache results
        if (count($test)) {
          $placeholders = implode(', ', array_fill(0, count($test), '%d'));

          $result = db_query("SELECT * FROM {term_data} WHERE tid IN ($placeholders)", $test);
          while ($term = db_fetch_object($result)) {
            if ($vids && empty($vids[$term->vid])) {
              $validated_cache[$term->tid] = FALSE;
              return FALSE;
            }

            $titles[] = $validated_cache[$term->tid] = check_plain($term->name);
            unset($test[$term->tid]);
          }
        }

        // Remove duplicate titles
        $titles = array_unique($titles);

        $this->argument->validated_title = implode($tids->operator == 'or' ? ' + ' : ', ', $titles);
        // If this is not empty, we did not find a tid.
        return empty($test);

      case 'name':
      case 'convert':
        $and = '';
        if (!empty($vids)) {
          $and = " AND td.vid IN(" . implode(', ', $vids) . ')';
        }
        if ($transform) {
          $result = db_fetch_object(db_query("SELECT td.* FROM {term_data} td LEFT JOIN {term_synonym} ts ON ts.tid = td.tid WHERE (replace(td.name, ' ', '-') = '%s' OR replace(ts.name, ' ', '-') = '%s')$and", $argument, $argument));
        }
        else {
          $result = db_fetch_object(db_query("SELECT td.* FROM {term_data} td LEFT JOIN {term_synonym} ts ON ts.tid = td.tid WHERE (td.name = '%s' OR ts.name = '%s')$and", $argument, $argument));
        }
        if (!$result) {
          return FALSE;
        }

        if ($type == 'convert') {
          $this->argument->argument = $result->tid;
        }
        $this->argument->validated_title = check_plain($result->name);

        return TRUE;
    }
  }
}
