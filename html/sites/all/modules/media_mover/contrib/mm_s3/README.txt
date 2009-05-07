// $Id: README.txt,v 1.1.2.1.2.3 2008/12/04 23:09:42 arthuregg Exp $

INSTALLATION
--------------------------------------
 * enable module at admin > build > modules
 * configure at admin > build > media_mover > settings
 

There is a S3 test that will let you see if your configuration is setup correctly at: 
 admin > build > media_mover > tests > mm_s3

This will attempt to move an image to S3 and then download it.

This module no longer needs the PEAR libraries. For more information
on the S3 class that it is using, please see:
http://undesigned.org.za/2007/10/22/amazon-s3-php-class