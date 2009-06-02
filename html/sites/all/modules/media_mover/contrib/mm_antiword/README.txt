; $Id: README.txt,v 1.1.2.2 2009/03/01 17:18:16 arthuregg Exp $

Media Mover Antiword
--------------------------------------

This module provides integration with Media Mover and Antiword.
This allows you to convert MS files into PDF, PS, and TXT formats.
While Antiword does a good job of converting documents, it is not
perfect- the more complicated the document is, the more difficult
it is to convert. 


Installation
--------------------------------------
You need to have a copy of antiword on your system to use this module.
You can install it on OSX using fink, apt-get on Debian/Ubuntu. You can
also download the binary from the antiword site here:

http://www.winfield.demon.nl/

Copy the module into your sites modules directory or into the all/modules
directory

Go to admin > build > modules and enable the module

MM Antiword will try to find your antiword binary. If it can not find it, you 
will need to set it by hand:

Go to admin > build > media mover > settings and set the path to the
binary file for antiword (where you installed it).

Got to admin > build > media mover and create a new configuration using antiword


For more information on Antiword, please see: http://www.winfield.demon.nl/


