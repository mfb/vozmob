<?php
//$Id:

/**
 *
 * @author Raphael Schär
 * This is a helper class to handle the whole data processing of exif
 *
 */
Class Exif {
  static private $instance = NULL;

  /**
   * We are implementing a singleton pattern
   */
  private function __construct() {
  }

  public function getInstance() {
    if(is_null(self::$instance)) {
      self::$instance = new self;
    }
    return self::$instance;
  }

  /**
   * Going through all the fields that have been created for a given node type
   * and try to figure out which match the naming convention -> so that we know
   * which exif information we have to read
   *
   * Naming convention are: field_exif_xxx (xxx would be the name of the exif
   * tag to read
   *
   * @param $arCckFields array of CCK fields
   * @return array a list of exif tags to read for this image
   */
  public function getExifFields($arCckFields=array()) {
    $arSections = array('exif', 'file', 'computed', 'ifd0', 'gps', 'winxp', 'iptc', 'xmp');
    $arExif = array();

    foreach ($arCckFields as $field) {
      $ar = explode("_", $field['field_name']);
      if ($ar[0] == 'field' && isset($ar[1]) && in_array($ar[1], $arSections)) {
        unset($ar[0]);
        $section = $ar[1];
        unset($ar[1]);
        $arExif[] = array('section'=>$section, 'tag'=>implode("_", $ar));
      }
    }
    return $arExif;
  }

  /**
   * Read the Information from a picture according to the fields specified in CCK
   * @param $file
   * @param $arTagNames
   * @return array
   */
  public function readExifTags($file, $arTagNames=array()) {
    if (!file_exists($file)) {
      return array();
    }

    $ar_supported_types = array('jpg', 'jpeg');
    if (!in_array(strtolower($this->getFileType($file)), $ar_supported_types)) {
      return array();
    }
    $exif = exif_read_data($file, 0);
    
    $arSmallExif = array();
    $arSmallExif = array_change_key_case((array)$exif, CASE_LOWER);
    $arSmallExif['computed'] = array_change_key_case((array)$arSmallExif['computed'], CASE_LOWER); //why this function isn't recursive is beyond me
    $arSmallExif['thumbnail'] = array_change_key_case((array)$arSmallExif['thumbnail'], CASE_LOWER);
    $arSmallExif['comment'] = array_change_key_case((array)$arSmallExif['comment'], CASE_LOWER);
    $info = array();
    
    foreach ($arTagNames as $tagName) {
      if ($tagName['section'] != 'iptc') {
        if (!empty($arSmallExif[$tagName['tag']])) {
          $info[$tagName['section'] .'_'. $tagName['tag']] = $arSmallExif[$tagName['tag']];
        } 
        elseif (!empty($arSmallExif[$tagName['section']][$tagName['tag']])) {
          $info[$tagName['section'] .'_'. $tagName['tag']] = $arSmallExif[$tagName['section']][$tagName['tag']];
        }
      }
    }
    foreach ($info as $tag => $value) {
      if (!is_array($value)) {
        $info[$tag] = utf8_encode($value);
      }
    }
    
    return $info;
  }

  private function getFileType($file) {
    $ar = explode('.', $file);
    $ending = $ar[count($ar)-1];
    return $ending;
  }

  /**
   * Read IPTC tags.
   * 
   * @param String $file
   * 	Path to image to read IPTC from
   * 
   * @param array $arTagNames
   * 	An array of Strings that contain the IPTC to read
   * 	If you leave this empty nothing will be returned, unless you select a special 
   * 	return style in the $arOptions
   * 
   * @param array $arOptions
   * 	The following options are possible:
   * 	style: fullSmall
   * 	
   */
  public function readIPTCTags($file, $arTagNames=array(), $arOptions=array()) {
    $humanReadableKey = $this->getHumanReadableIPTCkey();
    $size = GetImageSize ($file, $infoImage);
    $iptc = empty($infoImage["APP13"]) ? array() : iptcparse($infoImage["APP13"]);

    $arSmallIPTC = array();
    if (is_array($iptc)) {
      foreach ($iptc as $key => $value) {
        $resultTag = "";
        foreach ($value as $innerkey => $innervalue) {
          if( ($innerkey+1) != count($value) ) {
             $resultTag .= $innervalue . ", ";     
          }
          else {
            $resultTag .= $innervalue;
          }
        }
        $arSmallIPTC[$humanReadableKey[$key]] = $resultTag;
      }
    }
    
    if ($arOptions['style'] == 'fullSmall') {
      return $arSmallIPTC;
    }
    
    $info = array();
    foreach ($arTagNames as $tagName) {
      if ($tagName['section'] == "iptc") {
        $info[$tagName['section'] .'_'. $tagName['tag']] = utf8_encode($arSmallIPTC[$tagName['tag']]);
      }
    }
    return $info;
  }

  /**
   * Read XMP data from an image file.
   *
   * @param $file
   *   File path.
   *
   * @param $arTagNames
   *   Available metadata fields.
   *
   * @return
   *   XMP image metadata.
   *
   * @todo
   *   Support for different array keys.
   */
  public function readXMPTags($file, $arTagNames=array()) {
    // Get a CCK-XMP mapping.
    $map  = $this->getXMPFields();
    $xmp  = $this->openXMP($file);
    $info = array();

    if ($xmp != FALSE) {
      // Iterate over XMP fields defined by CCK.
      foreach ($arTagNames as $tagName) {
        if ($tagName['section'] == "xmp") {
          // Get XMP field.
          $config = $map[$tagName['tag']];
          $field = $this->readXMPItem($xmp, $config);
          $info[$tagName['section'] .'_'. $tagName['tag']] = $field;
        }
      }

      $this->closeXMP($xmp);
    }

    return $info;
  }

  /**
   * Open an image file for XMP data extraction.
   *
   * @param $file
   *   File path.
   *
   * @return
   *   Array with XMP file and metadata.
   */
  function openXMP($file) {
    // Setup.
    SXMPFiles::Initialize();
    $xmpfiles = new SXMPFiles();
    $xmpmeta  = new SXMPMeta();

    // Open.
    $xmpfiles->OpenFile($file);

    // Get XMP metadata into the object.
    if ($xmpfiles->GetXMP($xmpmeta)) {
      // Sort metadata.
      $xmpmeta->Sort();

      return array('files' => $xmpfiles, 'meta' => $xmpmeta);
    }

    // No XMP data available.
    return FALSE;
  }

  /**
   * Close a file opened for XMP data extraction.
   *
   * @param $xmp
   *   XMP array as returned from openXMP().
   */
  function closeXMP($xmp) { 
    $xmp['files']->CloseFile();
  }

  /**
   * Read a single item from an image file.
   *
   * @param $xmp
   *   XMP array as returned from openXMP().
   *
   * @param $config
   *   XMP field configuration.
   *
   * @param $key
   *   In case of array field type, the numeric field key.
   *
   * @return
   *   Field value.
   */
  public function readXMPItem($xmp, $config, $key = 0) {
    // Setup.
    $xmpfiles = $xmp['files'];
    $xmpmeta  = $xmp['meta'];

    // Try to read XMP data if the namespace is available.
    if(@$xmpmeta->GetNamespacePrefix($config['ns'])) {
      if ($config['type'] == 'property') {
        $value = @$xmpmeta->GetProperty($config['ns'], $config['name']);
      }
      elseif ($config['type'] == 'array') {
        $value = @$xmpmeta->GetArrayItem($key, $config['ns'], $config['name']);
      } 
      elseif ($config['type'] == 'struct') {
        $value = @$xmpmeta->GetStructField($config['ns'], $config['struct'], $config['ns'], $config['name']);
      }
    }

    return $value;
  }

  /**
   * Just some little helper function to get the iptc fields
   * @return array
   *
   */
  public function getHumanReadableIPTCkey() {
    return array(
      "2#202" => "object_data_preview_data",
      "2#201" => "object_data_preview_file_format_version",
      "2#200" => "object_data_preview_file_format",
      "2#154" => "audio_outcue",
      "2#153" => "audio_duration",
      "2#152" => "audio_sampling_resolution",
      "2#151" => "audio_sampling_rate",
      "2#150" => "audio_type",
      "2#135" => "language_identifier",
      "2#131" => "image_orientation",
      "2#130" => "image_type",
      "2#125" => "rasterized_caption",    
      "2#122" => "writer",
      "2#120" => "caption",
      "2#118" => "contact",
      "2#116" => "copyright_notice",
      "2#115" => "source",
      "2#110" => "credit",
      "2#105" => "headline",
      "2#103" => "original_transmission_reference",
      "2#101" => "country_name",
      "2#100" => "country_code",
      "2#095" => "state",
      "2#092" => "sublocation",
      "2#090" => "city",
      "2#085" => "by_line_title",
      "2#080" => "by_line",
      "2#075" => "object_cycle",
      "2#070" => "program_version",
      "2#065" => "originating_program",
      "2#063" => "digital_creation_time",
      "2#062" => "digital_creation_date",   
      "2#060" => "creation_time",
      "2#055" => "creation_date",
      "2#050" => "reference_number",
      "2#047" => "reference_date",
      "2#045" => "reference_service",
      "2#042" => "action_advised",
      "2#040" => "special_instruction",
      "2#038" => "expiration_time",
      "2#037" => "expiration_date",
      "2#035" => "release_time",
      "2#030" => "release_date",
      "2#027" => "content_location_name",
      "2#026" => "content_location_code",
      "2#025" => "keywords",
      "2#022" => "fixture_identifier",
      "2#020" => "supplemental_category", 
      "2#015" => "category",
      "2#010" => "subject_reference", 
      "2#010" => "urgency",
      "2#008" => "editorial_update",
      "2#007" => "edit_status",
      "2#005" => "object_name",
      "2#004" => "object_attribute_reference",
      "2#003" => "object_type_reference",
      "2#000" => "record_version"
      );
  }

  /**
   * XMP fields mapper. As we're dealing with a mapper between RDF
   * elements and CCK fields, we have to define custom keys that
   * both on the field name and the namespace used.
   *
   * And, as the XMP specs also defines some datatypes like properties,
   * arrays and structures, we have to deal with those as well.
   *
   * @return array
   *   Mapping between CCK and XMP fields.
   */
  public function getXMPFields() {
    return array(
      'headline'            => array(
        'name'              => 'Headline',
        'ns'                => 'http://ns.adobe.com/photoshop/1.0/',
        'type'              => 'property',
      ),
      'authorsposition'     => array(
        'name'              => 'AuthorsPosition',
        'ns'                => 'http://ns.adobe.com/photoshop/1.0/',
        'type'              => 'property',
        ),
      'source'              => array(
        'name'              => 'Source',
        'ns'                => 'http://ns.adobe.com/photoshop/1.0/',
        'type'              => 'property',
        ),
      'instructions'        => array(
        'name'              => 'Instructions',
        'ns'                => 'http://ns.adobe.com/photoshop/1.0/',
        'type'              => 'property',
        ),
      'subject'             => array(
        'name'              => 'subject',
        'ns'                => 'http://purl.org/dc/elements/1.1/',
        'type'              => 'array',
        ),
      'description'         => array(
        'name'              => 'description',
        'ns'                => 'http://purl.org/dc/elements/1.1/',
        'type'              => 'array',
        ),
      'creator'             => array(
        'name'              => 'creator',
        'ns'                => 'http://purl.org/dc/elements/1.1/',
        'type'              => 'array',
        ),
      'rights'              => array(
        'name'              => 'rights',
        'ns'                => 'http://purl.org/dc/elements/1.1/',
        'type'              => 'array',
        ),
      'title'              => array(
        'name'              => 'title',
        'ns'                => 'http://purl.org/dc/elements/1.1/',
        'type'              => 'array',
        ),
      'ciadrextadr'         => array(
        'name'              => 'CiAdrExtadr',
        'ns'                => 'http://iptc.org/std/Iptc4xmpCore/1.0/xmlns/',
        'type'              => 'struct',
        'struct'            => 'CreatorContactInfo',
        ),        
      'ciemailwork'         => array(
        'name'              => 'CiEmailWork',
        'ns'                => 'http://iptc.org/std/Iptc4xmpCore/1.0/xmlns/',
        'type'              => 'struct',
        'struct'            => 'CreatorContactInfo',
        ),        
      'ciurlwork'           => array(
        'name'              => 'CiUrlWork',
        'ns'                => 'http://iptc.org/std/Iptc4xmpCore/1.0/xmlns/',
        'type'              => 'struct',
        'struct'            => 'CreatorContactInfo',
        ),        
      'scene'               => array(
        'name'              => 'Scene',
        'ns'                => 'http://iptc.org/std/Iptc4xmpCore/1.0/xmlns/',
        'type'              => 'array',
        ),        
      'subjectcode'         => array(
        'name'              => 'SubjectCode',
        'ns'                => 'http://iptc.org/std/Iptc4xmpCore/1.0/xmlns/',
        'type'              => 'array',
        ),        
      'hierarchicalsubject' => array(
        'name'              => 'hierarchicalSubject',
        'ns'                => 'http://ns.adobe.com/lightroom/1.0/',
        'type'              => 'array',
        ),        
      'location'            => array(
        'name'              => 'Location',
        'ns'                => 'http://iptc.org/std/Iptc4xmpCore/1.0/xmlns/',
        'type'              => 'property',
        ),        
      'credit'              => array(
        'name'              => 'Credit',
        'ns'                => 'http://ns.adobe.com/photoshop/1.0/',
        'type'              => 'property',
      ),
      'countrycode'         => array(
        'name'              => 'CountryCode',
        'ns'                => 'http://iptc.org/std/Iptc4xmpCore/1.0/xmlns/',
        'type'              => 'property',
      ),
    );
  }
}  
