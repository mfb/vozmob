Media Mover FTP Module
----------------------------------------------
Original Code by Wim Leers http://drupal.org/user/99777
Contributions by Arthur http://drupal.org/user/27259

REQUIREMENTS
----------------------------------------------
 * PHP with FTP support
 * Media Mover installation
 * mimedetect module

USAGE
----------------------------------------------
 * Enable the module at admin > build > modules
 * create a new media mover job
 * select this as your harvest module
 * make sure you fill out all the forms correctly
 
 
SECURITY CONCERNS
----------------------------------------------
** PLEASE NOTE ** When you enable this module and run it
you are moving files from one system into your drupal system.
There are MANY ways in which this could be exploited to to place
malicious files on your Drupal system. You should configure the
harvesting options to be as strict as possible. The system will
only harvest items that match the file extensions that you provide.
Once files are harvested, the system will check to make sure 
that these files are mimetype audio, image, video and all others
will be deleted. 