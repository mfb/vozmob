$Id: README.txt,v 1.17 2008/02/06 22:35:07 bdragon Exp $

This is the Media Mover module.

If you're using the s3 module, please download the pear and s3 drivers here:

http://www.24b6.net/files/drivers.tgz

If you're having issues with FFmpeg, you can try out a binary here:

http://tunaspecial.com/files/ffmpeg.tgz


-----------------------------------------------------------------------
REQUIRES
-----------------------------------------------------------------------
FFmpeg requires a FFmpeg binary
S3 support requires the drivers file referenced above

Automatic file creation may not work on Windows due to path names. Not sure, hasn't been tested.

-----------------------------------------------------------------------
 INSTALLATION
-----------------------------------------------------------------------
Install the media_mover module directory under sites/all/modules or
sites/yoursite/modules

Go to admin/build/modules

Enable the Media Mover modules you want to use

-----------------------------------------------------------------------
 CONFIGURATION
-----------------------------------------------------------------------

Go to admin/media_mover/settings
These are the global settings for the Media Mover modules

Make sure you configure the path to FFmpeg. This is relative to the root
of your server. If don't know where FFmpeg is installed and you have
access to the command line of your server, you can run:

#locate ffmpeg

This should hopefully give you a path to ffmpeg.
If you need install FFmpeg, there are instructions here for installing
it on a debian/ubuntu environment:

http://www.24b6.net/?p=188

-----------------------------------------------------------------------
 USAGE
-----------------------------------------------------------------------

Goto admin/media_mover to build a configuration

Each configuration can have its own options, so you can make a configuration
that converts a video, and then another configuration which makes a thumbnail
for that video.

Media Mover configurations will be run every time cron is run, though you
can run a single configuration by hand if you use the run option at
admin/media_mover

 Example:
 ------------------------------------
 You have a site where users upload video with their nodes. You want
 these files converted to FLV format automatically. Media Mover can
 find and convert the files, but you will need some additional theming
 and potentially some additional modules.

 1) add a new configuration
 2) select "Media Mover node module: Select drupal uploaded files"
 3) select the content types you wish to harvest from and the kinds
    files you wish to harvest
 4) select "FFmpeg module: convert video"
 5) default options for conversion should work in most cases
 6) select bypass for storage and complete

 Once cron runs, uploaded files will be converted. Files are accessible
 at $node->media_mover. To get the flv file to be shown in a flash
 player, you can get the file path by $media->mover[X][0]['complete_file']
 where X is the Media Mover configuration id.

 You may find XSPF Playlist module and the SWFobject module helpful to
 display videos.


-----------------------------------------------------------------------
 DEVELOPERS
-----------------------------------------------------------------------

Documentation is here:
http://mediamover.24b6.net/


Media mover $item format
------------------------

$item = array(
  // File arrays.
  'harvest' => array(
    // Candidate fields for node.
    'node' => array(

    ),

    // Is this file a locally accessible file?
    'local' => TRUE,
  ),
  'process' => array(

  ),
  'storage' => array(

  ),
  'complete' => array(

  ),

  // Media mover ID.
  'mmfid' => 12345,

  // Status === 0 means there was a failure.
  // Modules should stop processing in this case. (@@@ Why do we continue in
  // this case, anyway?)
  'status' => 1,

  // Sometimes there may be an associated node.
  // @@@ Make sure code doesn't rely on this.
  // @@@ Node per-action?
  'nid' => 12345,

  // User ID these files are owned by.
  'uid' => 12345,
);









Here's an example of the media mover hook

/**
 * Implementation of media_mover hook
 */
function my_media_mover($op, $action = null, $configuration = null, &$file = array() ) {

  switch ($op) {

    // give your module a distinct name
    case 'name':
      return "My module name";
    break;

    // defines the actions that this module does
    // structure is an array of media mover verbs
    // with an array of actions for each verb
    // $actions[$verb][$action_id] Your ids only
    // need to be unique to your module and should
    // not contain spaces
    case 'actions':
      return array(
        'harvest' => array(
          'action_1' => t('select drupal uploaded files'),
          'action_2' => t('select files from some source'),
          ),
        'storage' => array(
          'action_3' => t('Save data as a node'),
          'action_4' => t('Attach converted file to node'),
        )
      );
    break;

    // create edit configuration option set
    // The creation of a Media Mover configuration pulls the
    // configuration form from every module.
    case 'config':
      switch ($action_id) {

         // here are internal media mover options
        case 'action_1': //select_drupal_uploaded_files
          return _media_mover_admin_harvest_config($action_id, $configuration);
        break;

        case 'action_2': //set node status
          return _media_mover_admin_complete_config($action_id, $configuration);
        break;

        ......
      }
    break;

    // set global configuration for this module
    // this is the global config. Configuration options for individaul
    // configurations are in $op = "config"
    // This displayed at admin/settings/media_mover
    // return a form array
    case 'admin':
      return _media_mover_admin();
    break;


    // defines directories (under master media_mover directory) this module uses
    // array of directories, will be created under the default
    // media_mover directory, set in admin
    case 'directories':
      return array('converted_files', 'harvested_files');
    break;


    // allows for a module to return additional data for a given file when
    // someone requests it
    // called from media_mover's mm_files_db_fetch and _mm_file_db_fetch
    // by default, medial_mover's files table is used. $file is available
    case 'fetch':
    break;

    // allows for module to update additional critera
    // add file to db action. called from media_mover's mm_files_db_update
    // by default, medial_mover's files table is used. $file is available
    case 'update':
    break;

    // functions called on harvest op
    // returns an array of $files
    case 'harvest':
      switch($action_id) {
        case 'action_1':
          return _mm_harvest($action_id, $configuration);
        break;

        ....
    break;

    // functions called on process op
    case 'process':
    break;

    // functions called on storage op
    case 'storage':
      switch ($action_id) {
        case 'storage--4':
          return _mm_node_save($file, $configuration);
        break;
      }
    break;

    // functions called on completion
    case 'complete':
      return _mm_complete($configuration, $file);
    break;

    default:
      return;
    break;
  }
}
