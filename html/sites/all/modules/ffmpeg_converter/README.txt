$Id: README.txt,v 1.5 2009/01/26 17:12:41 zoo33 Exp $

-----------------------
FFMPEG CONVERTER README
-----------------------

This module is designed to provide a user interface for the FFmpeg Wrapper 
module (http://drupal.org/project/ffmpeg_wrapper). It automatically converts 
media files to different destination formats and allows you to change a number 
of properties such as pixel size, bitrate etc. Additionally, if the source file
is a video file, three snapshot images will be saved from different instants in
the film.


Installation
------------

1. Make sure you have a working installation of FFmpeg on your web server. The 
   version that comes with your OS distribution (if any) may not be sufficient.
   Share hosts usually don't have FFmpeg installed.
   
   - FFmpeg home: http://ffmpeg.mplayerhq.hu/
   - Ubuntu instructions: http://ubuntuforums.org/showthread.php?t=911849
   - Ubuntu instructions (compile): https://wiki.ubuntu.com/ffmpeg

2. Download and install FFmpeg Wrapper which is required by FFmpeg Converter:

   - http://drupal.org/project/ffmpeg_wrapper
   
3. Download and install these recommended modules (not much will happen without
   them):
   
   - FileField: http://drupal.org/project/filefield
   - Job queue: http://drupal.org/project/job_queue
   
4. Optionally download and install MimeDetect, which will set proper MIME types 
   on your converted files. Also consider installing Imagecache, which will 
   allow you to display your video snapshot images in custom sizes.

   - http://drupal.org/project/mimedetect
   - http://drupal.org/project/imagecache
   
5. Install and activate this module.


Configuration
-------------

1. Go to Administer > Site Configuration > FFmpeg Wrapper and supply the proper
   file paths.
   
2. Go to Administer > Site Building > FFmpeg Converter and set up your 
   conversion preset(s) (currently only one is available). For more information
   about these options go to: http://ffmpeg.mplayerhq.hu/

3. Go to Administer > Content > Content types and set up a file field for one 
   of your node types. On the configuration screen for the file field you can 
   choose a FFmpeg Converter preset that you want to use for this particular 
   field. The field must allow for multiple values (at least five) and should 
   be set up to accept all the file types you intend to use for input files.
   
   All media files uploaded to this field will be automatically converted if 
   you have Job queue installed and cron set up (see http://drupal.org/cron). 
   On Administer > Reports > Queued jobs you can find a list of all conversions
   that have not yet run.
   
4. Click on the Display tab on the administration pages for the node type(s) 
   you're using with FFmpeg Converter (the one you set up in #3). Choose the 
   display styles you want to use for teasers and full nodes. If you choose 
   either "Image" or an ImageCache preset, converted video files will be 
   displayed as a snapshot image, linked to either the converted video file or 
   the node, depending on what you choose.

5. Test you configuration. You will probably want to allow PHP to run for a 
   longer time than the usual default. This can be configured in your php.ini 
   file. You may also want to test different cron intervals.


Use
---

When creating or editing a node, add an input file to your configured file 
field. After saving, you should see a message saying that the file has been 
queued for conversion. After the next cron run, or after a couple, the file 
field should contain four additional files. One is the output file in the 
format you chose in the FFmpeg Converter Presets settings. The other three are 
snapshot images from your video file (if it is a video), taken at one fourth, 
half, and three fourths of the total length of the video. This increases the 
chances of there being a good snapshot of the film. You will see one of the 
images on the node page, linked to your converted file (depending on the 
display setting in #4 above and the file type). If you want to redo the 
conversion at a later time, edit the node and check the "Delete converted 
files" box.

FFmpeg converter doesn't currently provide any sort of video playback 
functionality. You may override the ffmpeg_converter_video.tpl.php template 
file in your theme in order to add support for video players etc.
