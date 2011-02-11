<?php
// $Id$
 /**
  * Variables available:
  * - $view: The view object
  * - $field: The field handler object that can process the input
  * - $row: The raw SQL result that can be used
  * - $output: The processed output that will normally be used.
  *
  * When fetching output from the $row, this construct should be used:
  * $data = $row->{$field->field_alias}
  *
  * @see phptemplate_preprocess_views_view_field__features__tid()
  * - $media: 
  */
?>
<?php print $media['content']; ?>
