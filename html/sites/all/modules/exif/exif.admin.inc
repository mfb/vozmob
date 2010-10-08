<?php 
// $Id: exif.admin.inc,v 1.1.2.1 2010/06/09 13:14:02 rapsli Exp $

/**
 * Just some help page. Gives you an overview over the available tags
 * @return string html
 */
function exif_admin_settings() {
  drupal_add_css(drupal_get_path('module', 'exif') .'/exif.css');
  $filepath = drupal_get_path('module', 'exif') .'/sample.jpg';
  $exif = _exif_get_class();
  $ar_exif = read_exif_data($filepath, 0, TRUE);
  // CCK field names must be lowercase
  $ar_exif = array_change_key_case($ar_exif, CASE_LOWER);
  $out = t('This is an overview over the most common data that is extracted with the exif module. How to read this table: The grey table header would be the key identifier and the following attributes would be the field to read.');
  $out .= ' ' . t('For example: If you want to import datetimeoriginal into an CCK field, you would name the CCK field field_exif_datetimeoriginal. Since this is a date field you can use a datetime field.');
  $rows1 = array();
  $help = t('This would be the keyword for your CCK field.');
  foreach ($ar_exif as $key => $value) {
    if (is_array($value)) {
      $value = _exif_reformat($value);
      $rows1[] = array('data' => array($key, $help), 'class' => 'tag_type');
      foreach ($value as $key2 => $value2) {
        $rows1[] = array('data' => array($key2, check_plain(utf8_encode($value2))));
      }
    }
  }
  $human_readable_key = $exif->getHumanReadableIPTCkey();
  $size = GetImageSize($filepath, $info_image);
  $iptc = iptcparse($info_image["APP13"]);
  $rows2 = array();
  $help = t('This would be the keyword for your CCK field.');
  if (is_array($iptc)) {
    $rows2[] = array('data' => array('IPTC', $help), 'class' => 'tag_type');
    foreach ($iptc as $key => $value) {
      $result_tag = "";
      foreach ($value as $innerkey => $innervalue) {
        if ( ($innerkey+1) != count($value) ) {
          $result_tag .= $innervalue .", ";
        }
        else {
          $result_tag .= $innervalue;
        }
      }
      $rows2[] = array('data' => array($human_readable_key[$key], check_plain(utf8_encode($result_tag))));
    }
  }


  $rows = array_merge($rows1, $rows2);
  $header = array(t('Key'), t('Value'));
  $out .= theme('table', $header, $rows, array('id' => 'exif-fields'));
  // TODO Prevent binary data values from busting the page layout
  return $out;
}

/**
 * The form definition for the admin settings
 * @return array
 *  form definition
 */
function exif_admin_settings_form() {
  $forms = array();
  /*$forms['exif_granularity'] = array(
    '#type' => 'select',
    '#title' => t('Granularity'),
    '#options' => array(0 => t('Default'), 1 => ('Day')),
    '#default_value' => variable_get('exif_granularity', 0),
    '#description' => t('If a timestamp is select (for example the date the picture was taken), you can specify here how granular the timestamp should be. If you select default it will just take whatever is available in the picture. If you select Day, the Date saved will look something like 13-12-2008. This can be useful if you want to use some kind of grouping on the data.'),
  );*/

  $all_nodetypes = node_get_types();
  $all_nt = array();
  foreach ($all_nodetypes as $item) {
    $all_nt[$item->type] = $item->name;
  }
  $forms['exif_nodetypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Nodetypes'),
    '#options' => $all_nt,
    '#default_value' => variable_get('exif_nodetypes', array()),
    '#description' => t('Select nodetypes which should be checked for exif data. Incase the nodetypes contains more than one filefield, make sure that the imagefield is the first one!!!!'),
  );

  $forms['exif_update'] = array(
    '#type' => 'checkbox',
    '#title' => t('Refresh on node update'),
    '#default_value' => variable_get('exif_update', TRUE),
    '#description' => t('If you enable this option, Exif data is being updated when the node is being updated.'),
  );
  
  $forms['exif_empty_values'] = array(
    '#type' => 'checkbox',
    '#title' => t('Write empty image data?'),
    '#default_value' => variable_get('exif_empty_values', TRUE),
    '#description' => t("If checked all values will be written. So for example if you want to read the creation date from EXIF, but it's not available, it will just write an empty string. If unchecked, empty strings will not be written. This might be the desired behavior, if you have a default value for the CCK field."),
  );
  return system_settings_form($forms);
}