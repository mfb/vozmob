
;$Id: README.txt,v 1.8.2.14.2.1 2008/10/29 02:16:43 arthuregg Exp $ 


This is the Media Mover API module. 

If you're using the s3 module, please download the pear and s3 drivers here:

http://mediamover.24b6.net/drivers


-----------------------------------------------------------------------
REQUIRES
-----------------------------------------------------------------------
FFmpeg requires ffmpeg_wrapper module
S3 support requires the drivers file referenced above
mimedetect is required for FFmpeg and S3 support

-----------------------------------------------------------------------
 INSTALLATION
-----------------------------------------------------------------------
Install the media_mover module directory under sites/all/modules or 
sites/yoursite/modules

Go to admin > build > modules

Enable the Media Mover modules you want to use

-----------------------------------------------------------------------
 CONFIGURATION
-----------------------------------------------------------------------

Go to admin > media_mover > settings
These are the global settings for the Media Mover modules

Individual configurations can be configured when you create a new job.

-----------------------------------------------------------------------
 USAGE
-----------------------------------------------------------------------

Goto admin > media_mover to build a configuration

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
 CLUSTERING
-----------------------------------------------------------------------

It is possible to cluster multiple servers to process media mover configurations.
The easiest implementation is a parralel  setup which gives no speed gains per file,
however yields significant performance with large numbers of files. Further, it can
configured to remove processing from front-end web app servers, while 
running all the processing with other servers, utilizing the same database.

Some tips:
 * reduce the numbers of files harvested, processed, etc, by each config (look at the
   advanced options for each configuration
 * on web app servers, or non processing servers in the cluster, in settings.php in the 
   $conf array add:
    'media_mover_cron_off_CID' => true); 
   Where CID is the id of the configuration. This allows you to turn off cron 
   processing per server, per configuration. This is important because you don't 
   want your web app servers having to process extra data. Note that since this
   is being done in an individual server's settings.php, it doesn't impact 
   other servers that are accessing the same database.  



-----------------------------------------------------------------------
 DEVELOPERS
-----------------------------------------------------------------------
 please see http://drupal.org/node/276134

