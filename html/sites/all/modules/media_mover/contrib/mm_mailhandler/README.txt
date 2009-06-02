
; $Id: README.txt,v 1.1.2.1 2009/04/01 06:06:33 arthuregg Exp $;


Media Mover MailHandler Module
--------------------------------------------------------

This module enables Media Mover jobs to harvest items from email
acounts with the MailHandler module


Requires
--------------------------------------------------------
 * media_mover_api
 * mailhandler
 * php with imap compiled into it
 
Usage
--------------------------------------------------------
 * Enable module in admin > build > modules
 * Create a new Media Mover job
 * Select Media Mover Mail harvest
 * Enter in your email host configuration
   * note, you may want to look at mailhandler's support forums
     for information about the advanced settings
     