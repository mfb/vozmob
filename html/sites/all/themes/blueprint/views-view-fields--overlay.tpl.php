<?php
// $Id: views-view-fields.tpl.php,v 1.6 2008/09/24 22:48:21 merlinofchaos Exp $
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->separator: an optional separator that may appear before a field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
?>
<?php foreach ($fields as $id => $field): ?>
  <?php if (!empty($field->separator)): ?>
    <?php print $field->separator; ?>
  <?php endif; ?>

  <?php if (strlen($field->content) > 0): ?>
    <<?php print $field->inline_html;?> class="views-field-<?php print $field->class; ?>">
      <?php if ($field->label): ?>
        <label class="views-label-<?php print $field->class; ?>">
          <?php print $field->label; ?>:
        </label>
      <?php endif; ?>
        <?php
        // $field->element_type is either SPAN or DIV depending upon whether or not
        // the field is a 'block' element type or 'inline' element type.
        ?>
        <<?php print $field->element_type; ?> class="field-content"><?php print $field->content; ?></<?php print $field->element_type; ?>>
    </<?php print $field->inline_html;?>>
  <?php endif; ?>

  <?php if ($field->class == 'field-image-fid' && ($node = node_load($view->args[0])) && !empty($node->media_mover)): ?>
    <div class="media-mover">
      <?php foreach ($node->media_mover as $cid): ?>
        <?php foreach ($cid as $mmfid): ?>
          <?php if (substr($mmfid['complete_file'], -4, 4) == '.mp3'): ?>
            <?php if (empty($playlist)): ?>
              <?php $playlist[] = base_path() . drupal_get_path('theme', 'blueprint') . '/images/audio_icon_large.gif'; ?>
            <?php endif; ?>
            <?php $playlist[] = array('url' => file_create_url($mmfid['complete_file'])); ?>
          <?php endif; ?>
        <?php endforeach; ?>
      <?php endforeach; ?>
      <?php if (!empty($playlist)): ?>
        <div class="media-mover-audio">
          <?php print theme('flowplayer', array('clip' => array('autoPlay' => TRUE, 'autoBuffering' => TRUE), 'playlist' => $playlist, 'plugins' => array('controls' => array('fullscreen' => FALSE, 'playlist' => TRUE))), 'flowplayer-audio-' . $node->nid); ?>
        </div>
      <?php endif; ?>
    </div>
  <?php endif; ?>

<?php endforeach; ?>
