Readme
------

The Mailhandler module allows registered users to create or edit nodes and comments via email. Authentication
is usually based on the From: email address. Users may post taxonomy terms, teasers, and other node parameters using the Commands capability. 

See the 'more help' page within this module for more info - 'admin/help/mailhandler'.

Requirements
------------

The IMAP dynamic extension must be enabled. On Windows PHP installations, this is as easy as uncommenting 
the line containing "extension=php_imap.dll" in php.ini. For other OS, you may need to recompile PHP
if you don't have this extension already.

Installation
------------

1. Activate this module as usual

2. Add a mailbox in the Admin page. You'll need a corresponding Folder, POP, or IMAP mailbox
dedicated to inbound Drupal mail. Additional mailboxes may be useful for specialized situations.


Admin Usage
------------

1. Send a few emails to a Mailhandler email address from an email address which is registered on your site.
Then visit cron.php to kickoff a pull from your mail server and processing of pending email. Alternatively,
follow the 'retrieve' link for your newly created mailbox. You should see new nodes on
your site (go to admin/content/node) or receive error message replies via email.

2. You may add extra security to a Mailhandler mailbox if desired on the Admin page.

3. Install mailalias.module if you want to assist users who might contribute from multiple email addresses.

4. Consider installing the Marksmarty module and using its filter in the input format that you assign in your mailbox form. This filter does a nice job of prettying up plain text email for presentation on the Web.


Developer Usage
-----------------------------

Mailhandler has several hooks which other modules may implement:

* hook_mailhandler allows for the message to be altered during node creation (see mailhandler_node_process_message
  in mailhandler.module)
* hook_mailhandler_post_save allows operations on a newly saved node created by mailhandler (see mailhandler_node_process_message
  in mailhandler.module)
* hook_mailhandler_authenticate_info allows modules to define one or more mailhandler authentication plugins (see 
  mailhandler.module for implementations)

Credits
----------

- this borrows from Julian Bond's excellent Blogmail module for Drupal 3. Gerhard contributed IMAP Folder support.