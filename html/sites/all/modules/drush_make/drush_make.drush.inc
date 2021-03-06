<?php
// $Id: drush_make.drush.inc,v 1.11.2.60 2010/06/30 22:24:24 dmitrig01 Exp $

define('DRUSH_MAKE_UPDATE_DEFAULT_URL', 'http://updates.drupal.org/release-history');
define('DRUSH_MAKE_VERSION_BEST', 'best');

include_once 'drush_make.utilities.inc';
include_once 'drush_make.download.inc';
include_once 'drush_make.project.inc';

/**
 * Implement EXTENSION_drush_init().
 */
function drush_make_drush_init() {
  // The default URL to use for retrieving update XML files.
  drush_set_default('drush-make-update-default-url', DRUSH_MAKE_UPDATE_DEFAULT_URL);
  // The default method drush_make will use to find the best release.
  drush_set_default('drush-make-version-best', DRUSH_MAKE_VERSION_BEST);
}

/**
 * Implementation of hook_drush_command().
 */
function drush_make_drush_command() {
  $items['make'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => 'Turns a makefile into a working drupal install.',
    'arguments' => array(
      'makefile' => 'Filename of the makefile to use for this build.',
      'build path' => 'The path at which to build the makefile.',
    ),
    'examples' => array(
      'drush make example.make example' => 'Build the example.make makefile in the example directory.',
    ),
    'options' => array(
      '--version' => 'Print the version and exit.',
      '--contrib-destination=path' => 'Specify a path under which modules and themes should be placed. Defaults to sites/all.',
      '--force-complete' => 'Force a complete build even if errors occur.',
      '--md5' => 'Output an md5 hash of the current build after completion.',
      '--no-clean' => 'Leave temporary build directories in place instead of cleaning up after completion.',
      '--no-core' => 'Do not require a Drupal core project to be specified.',
      '--no-patch-txt' => 'Do not write a PATCHES.txt file in the directory of each patched project.',
      '--prepare-install' => 'Prepare the built site for installation. Generate a properly permissioned settings.php and files directory.',
      '--tar' => 'Generate a tar archive of the build. The output filename will be [build path].tar.gz.',
      '--test' => 'Run a temporary test build and clean up.',
      '--working-copy' => 'Where possible, retrieve a working copy of projects from their respective repositories.',
    ),
  );

  $items['make-test'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'description' => 'Run a drush make test.',
    'arguments' => array(
      'test' => 'Name of the test to run or ALL to run all tests.',
    ),
    'examples' => array(
      'drush make-test svn' => 'Run tests for SVN integration.',
    ),
  );

  $items['generate-makefile'] = array(
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'description' => 'Attempts to generate a makefile from the current drupal install.',
  );

  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function drush_make_drush_help($section) {
  switch ($section) {
    case 'drush:make':
      return dt('Turns a makefile into a working drupal install. For a full description of options and makefile syntax, see the README.txt included with drush make.');
    case 'drush:make-test':
      return dt('Run a drush make test.');
    case 'drush:generate-makefile':
      return dt('Attempts to generate a makefile from the current drupal install.');
  }
}

/**
 * Drush callback; make a makefile from the current install.
 */
function drush_drush_make_generate_makefile($file) {
  include_once 'drush_make.generate.inc';
  _drush_generate_makefile($file);
}

/**
 * Drush callback; run a drush make test.
 */
function drush_drush_make_make_test($test = NULL) {
  include_once 'drush_make.test.inc';
  drush_make_test($test);
}

/**
 * Drush callback; make based on the makefile.
 */
function drush_drush_make_make($makefile = NULL, $build_path = NULL) {
  if (!drush_make_ensure_version()) {
    return;
  }
  if (drush_get_option('version', FALSE)) {
    drush_print(drush_make_version());
    return;
  }
  if (!($build_path = drush_make_build_path($build_path))) {
    return;
  }
  if (!($info = drush_make_validate_info_file(drush_make_parse_info_file($makefile)))) {
    return;
  }

  drush_make_add_projects(FALSE, drush_get_option('contrib-destination', 'sites/all'), $info, $build_path);
  drush_make_add_libraries(drush_get_option('contrib-destination', 'sites/all'), $info, $build_path);

  // Add clean up tasks.
  if (drush_get_option('prepare-install')) {
    drush_make_prepare_install($build_path);
  }
  if (drush_get_option('md5')) {
    drush_make_md5();
  }

  // Only take final build steps if not in testing mode.
  if (!drush_get_option('test')) {
    if (drush_get_option('tar')) {
      drush_make_tar($build_path);
    }
    else {
      drush_make_move_build($build_path);
    }
  }

  drush_make_clean_tmp();
}

function drush_make_add_projects($recursion, $contrib_destination, $info, $build_path) {
  $projects = array();
  if (empty($info['projects'])) {
    return;
  }
  foreach ($info['projects'] as $key => $project) {
    // Merge the known data onto the project info.
    $project += array(
      'name'                => $key,
      'core'                => $info['core'],
      'build_path'          => $build_path,
      'contrib_destination' => $contrib_destination,
      'version'             => drush_get_option('drush-make-version-best'),
      'location'            => drush_get_option('drush-make-update-default-url'),
      'subdir'              => '',
      'directory_name'      => '',
    );
    $project = drush_make_updatexml($project);
    $class_name = 'DrushMakeProject_' . $project['type'];
    $projects[($project['type'] == 'core' ? 'core' : 'contrib')][$project['name']] = new $class_name($project);
  }

  if (!empty($projects['core'])) {
    $cores = count($projects['core']);
  }
  else {
    $cores = 0;
  }

  if (drush_get_option('no-core')) {
    unset($projects['core']);
  }
  elseif ($cores == 0 && !$recursion) {
    drush_set_error(dt('No core project specified.'));
    return FALSE;
  }
  elseif ($cores == 1 && $recursion) {
    unset($projects['core']);
  }
  elseif ($cores > 1) {
    drush_set_error(dt('More than one core project specified.'));
    return FALSE;
  }
  if (isset($projects['core'])) {
    foreach ($projects['core'] as $project) {
      $project->make();
    }
  }

  if (isset($projects['contrib'])) {
    foreach ($projects['contrib'] as $project) {
      $project->make();
    }
  }
}

function drush_make_add_libraries($contrib_destination, $info, $build_path) {
  if (!empty($info['libraries'])) {
    foreach ($info['libraries'] as $key => $library) {
      if (!is_string($key) || !is_array($library)) {
        // TODO Print a prettier message
        continue;
      }
      // Merge the known data onto the library info.
      $library += array(
        'name'                => $key,
        'core'                => $info['core'],
        'build_path'          => $build_path,
        'contrib_destination' => $contrib_destination,
        'subdir'              => '',
        'directory_name'      => $key,
      );
      $class = new DrushMakeProject_Library($library);
      $class->make();
    }
  }
}

function drush_make_updatexml($project) {
  if (isset($project['download']) && isset($project['type'])) {
    return $project;
  }
  $xml = drush_make_get_remote_file($project['location'] . '/' . $project['name'] . '/' . $project['core']);
  // First, get the release history.
  $release_history = simplexml_load_string($xml);
  if (!is_object($release_history) || !$release_history->title) {
    drush_make_error('XML_ERROR', dt("Could not retrieve version information for %project.", array('%project' => $project['name'])));
    return FALSE;
  }
  drush_log(dt('Project information for %project retrieved.', array('%project' => $project['name'])), 'ok');
  $project['release_history'] = $release_history;
  if (!isset($project['type'])) {
    // Determine the project type.
    $term_map = array(
      'Modules' => 'module',
      'Themes' => 'theme',
      'Drupal project' => 'core',
      'Installation profiles' => 'profile',
      'Translations' => 'translation'
    );
    // Iterate through all terms related to this project.
    foreach ($release_history->terms->term as $term) {
      // If we find a term from the term map, add it.
      if (in_array((string) $term->value, array_keys($term_map))) {
        $project['type'] = $term_map[(string)$term->value];
        break;
      }
    }
    if (!isset($project['type'])) {
      drush_make_error('BUILD_ERROR', dt("Unable to determine project type for %project.", array('%project' => $project['name'])));
      return FALSE;
    }
  }
  if (!isset($project['download'])) {
    $project = drush_make_update_xml_download($project);
  }
  return $project;
}

function drush_make_update_xml_download($project) {
  // Make an array of releases.
  foreach ($project['release_history']->releases->release as $release) {
    $version = (string) $release->version_major;
    // there should be version_patch attribute for every stable release
    // so checking whether the attribute exists should be enough
    if (isset($release->version_patch)) {
      $version .= '.' . (string) $release->version_patch;
    }
    // if version_patch attribute does not exist, then it should be a dev release
    // and the version string should be in format MAJOR_VERSION.x-dev
    else {
      $version .= '.x';
    }
    if ($extra_version = (string) $release->version_extra) {
      $version .= '-' . $extra_version;
    }
    $releases[$version] = array(
      'file' => (string) $release->download_link,
      'md5' => (string) $release->mdhash,
      'version' => (string) $release->version,
    );
    foreach (array('major', 'patch', 'extra') as $part) {
      $releases[$version][$part] = (string) $release->{'version_' . $part};
    }
  }

  // Find the best release.
  if ($project['version'] == drush_get_option('drush-make-version-best')) {
    $recommended_major = (string)$project['release_history']->recommended_major;
    $project['version'] = _drush_make_update_xml_best_version($recommended_major, $releases);
  }
  // Find the best release for the specified version.
  else {
    $exploded_version = explode('.', $project['version']);
    if (count($exploded_version) == 1) {
      list($major) = $exploded_version;
      $project['version'] = _drush_make_update_xml_best_version($major, $releases);
    }
    // Otherwise we have a full version.
  }

  $final_version = empty($project['version']) ? '' : $project['version'];

  // Uh oh, couldn't find a valid version.
  if (empty($final_version) || empty($releases[$final_version])) {
    drush_make_error('BUILD_ERROR', dt("Invalid version %version for %project.", array('%version' => $final_version, '%project' => $project['name'])));
    return FALSE;
  }

  $release = $releases[$final_version];
  $project['download'] = array(
    'type' => 'get',
    'url'  => $release['file'],
    'md5'  => $release['md5'],
  );
  return $project;
}

function _drush_make_update_xml_best_version($major, $releases) {
  $best = NULL;
  foreach ($releases as $version => $release_info) {
    if ($release_info['major'] == $major && version_compare($best, $version, '<')) {
      $best = $version;
    }
  }
  return $best;
}

function drush_make_build_path($build_path) {
  // Determine the base of the build.
  if (drush_get_option('tar')) {
    $build_path = dirname($build_path) . '/' . basename($build_path, '.tar.gz') . '.tar.gz';
  }
  elseif (isset($build_path) && (!empty($build_path) || $build_path == '.')) {
    $build_path = rtrim($build_path, '/');
  }
  // Allow tests to run without a specified base path.
  elseif (drush_get_option('test') || drush_confirm(dt("Make new site in the current directory?"))) {
    $build_path = '.';
  }
  else {
    drush_log(dt('Build aborted.'), 'ok');
    return FALSE;
  }
  if ($build_path != '.' && file_exists($build_path)) {
    drush_set_error(dt('Base path %path already exists', array('%path' => $build_path)));
    return FALSE;
  }
  return $build_path;
}

function drush_make_move_build($build_path) {
  $tmp_path = drush_make_tmp();
  if ($build_path == '.') {
    drush_shell_exec('ls -A %s/__build__', $tmp_path);
    $info = drush_shell_exec_output();
    foreach ($info as $file) {
      drush_shell_exec("cp -Rf %s/__build__/%s %s", $tmp_path, $file, $build_path);
    }
  }
  else {
    drush_shell_exec('mkdir -p %s', dirname($build_path));
    drush_shell_exec("mv %s/__build__ %s/%s", $tmp_path, $tmp_path, basename($build_path));
    drush_shell_exec("cp -Rf %s/%s %s", $tmp_path, basename($build_path), dirname($build_path));
  }
  return TRUE;
}

function drush_make_version() {
  return '2.0-beta8';
}
