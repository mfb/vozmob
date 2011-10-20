Module: Return Path
Author: Mike Carter <www.ixis.co.uk/contact>

Description
===========
Fixes the problem of bad email 'Return-Path' header settings which prevent email bouncebacks going to the sender
of the email.


Installation
============
Activate the module and it will replace the Drupal mail handling feature. In Drupal 4.7 deactivating
the module requires some manual work as there is no hook_uninstall() available.

To remove the module you need to delete the variable 'smtp_library' from your sites database.

Executing the following PHP code on your Drupal site will also disable the module correctly:-

variable_set('smtp_library', '');


Details
=======
Most mail servers appear to over-write any 'Return-path' headers sent by the PHP mail() function.
The 'Return-Path' value is used by some mail servers to send bounce back emails. A lot of web hosting
companies send emails out with a return-path email based on the account name Apache runs as on the
server, e.g. apache@example.com or nobody@example.com

One solution is to define the -f sendmail parameters in the php.ini 'sendmail_path'.

This is no good when running a single Apache instance with PHP as there is only one php.ini and you
need to specify different return addresses for each domain.

This module resolves the problem by parsing out the return-path set by Drupal modules, and passing it
to the sendmail binary using the -f option, rather than sending it only in the $headers parameter of the
mail() command.


Credits
=======
This module is the work of Mike Carter <www.ixis.co.uk/contact>.

