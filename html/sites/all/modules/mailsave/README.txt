Mailsave for Drupal 6.x
-----------------------
This is the first official release of mailsave for Drupal 6. At this time
only the main mailsave module and the clean up filters are ready for use.
The mailsave_to_image module isn't available as the image module itself
hasn't been officially released.

Mailsave version 2 for Drupal 5.x
---------------------------------
If you have used a previous version of mailsave please make sure to read
this file as there are some important changes!

mailsave is an extension that works in conjunction with mailhandler.
mailsave without mailhandler does not do anything!

When installed mailsave will collect attachments from incoming mail messages
and save them with the nodes that are created, if the user has the appropriate
permissions.

To save attachments you must enable upload.module and give the user both
the upload permission "upload files" and the mailsave permission "save
attachments".

The module uses the same rules as upload to determine what files the user is
allowed to save (maximum sizes, extensions etc). Attached files are saved
in to your Drupal files directory, just like upload does.

You can enhance mailsave by using additional modules that can alter the
created nodes in some way, depending on what attachments they find.

For example, mailsave_to_image works with mailsave and image such that if the
incoming message contains a jpeg then the node will automatically be
converted to an image node, using the first jpeg it finds. Any additional files
or jpegs will be attached.

You can achieve a similar effect by using the inline module if you prefer or
if you want to use multiple images in your post. In this case you simply use
regular inline syntax in your message, i.e. [inline|nn]. Refer to inline's
help for details.

The package also includes mailsave_to_image_attach and mailsave_to_imagefield
which allows mailsave to work with other modules that handle image formats.
There is also a beta version of mailsave_to_audio which has only had limited
testing.

If you want you can enable a mailsave extension, but disable the ability to
save attachments, so that you can limit what users are able to do.

You can also extend mailsave by using modules that interact with mailsave's
"clean up" hook. If you have a mobile phone that can send picture messages
via e-mail then you might find that the finished message contains additional
logos and text. If there is a clean up module for your service provider then
you can enable it to have additional images and text discarded, leaving you
with just the message and the actual attachments. You can find the plug-ins
at Administer > Mailsave. By default they are all turned off.

If you find mailsave and its related modules aren't saving attachments or
doing conversions that you expect always double check your user access and node
settings first. The most common error is that the user doesn't have permission
to use the conversion feature you want. This is often true if you are allowing
anonymous users to post via email. To allow this you must let anonymous users
have permission to create the appropriate content types.

You might also find the mailalias module (drupal.org/project/mailalias) useful
to allow users to associate other email address (e.g. their mobile MMS address)
with their account.

Key changes from mailsave version 1
-----------------------------------
The original mailsave module did both the attachment extraction and the
image conversion itself. To make the module more flexible and allow other
developers to interact with it the code has been separated in to mailsave,
the engine that does the ground work of retrieving attachments, and then
additional functions are implemented in separate modules via hook_mailsave.

MMS clean up functions are achived by using hook_mailsave_clean.


Key changes from mailsave for Drupal 4.6
----------------------------------------
administer > settings > mailsave no longer exists. Everything is now controlled
from Administer > User management > Access control.

Users need TWO permissions to allow saving of attachments, and TWO permissions
to allow creation of image nodes. This allows you to fine tune who can do what.

Files are now saved in the regular drupal files directory.

; $Id: README.txt,v 1.9 2008/07/12 21:31:51 stuartgreenfield Exp $