;$Id: README.txt,v 1.1.2.3.2.1 2009/01/02 19:20:59 arthuregg Exp $

-----------------------------------------------------------------------
 CONFIGURATION
-----------------------------------------------------------------------

Go to admin/settings/ffmpeg_wrapper

Make sure you configure the path to FFmpeg. This is relative to the root 
of your server. If don't know where FFmpeg is installed and you have
access to the command line of your server, you can run:

$locate ffmpeg

This should hopefully give you a path to ffmpeg. 
If you need install FFmpeg, there are instructions here for installing 
it on a debian/ubuntu environment:

http://www.24b6.net/?p=188

If you need a compiled binary, check http://mediamover.24b6.net/ffmpeg

If you are having PHP issues with openbasedir, you may want to look
at this page: http://drupal.org/node/82223

-----------------------------------------------------------------------
 DEVELOPERS
-----------------------------------------------------------------------
ffmpeg_wrapper supports defining configurations that set default values
in forms that are built to support ffmpeg_wrapper. Look in the conf dir
for an example of how these configurations can be built. This is helpful
for constraining users from building configurations that ffmpeg will not
function with.

To enable the support in your forms, merely call this function: 
  ffmpeg_wrapper_enable($prefix, $output_form_id);

Here, $prefix is a prefix used in your forms (in standard drupal, this will
be at least "edit-"). $output_form_id is the name of the output controler
form element. Check out media mover API's mm_ffmpeg module for an example.

The second piece that needs to be done is to name your form elements with
the ffmpeg_wrapper convention. Any form element that controls an aspect
of ffmpeg (eg, audio bit rate) needs to be named in a specific way:
  ffmpeg_audio_ab 
Where ffmpeg is always used, audio is for audio options (video is the other
possibility) and ab is the ffmpeg command for setting the audio bit rate.
Again, mm_ffmpeg offers a good example of how to do this.

Alternatively, you can just use 
$form[] = ffmpeg_wrapper_configuration_form($prefix, $configuration) to
build a form from ffmpeg_wrapper. You will need to handle your own validation
and submission, but outside of handing the size:other options, this is
very easy. Going this route gives you the ajax form configuration straight
away.


