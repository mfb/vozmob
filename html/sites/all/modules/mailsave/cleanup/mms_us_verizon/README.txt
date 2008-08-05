mms_us_verizon
--------------
This is a prototype module to try and clean up messages that are posted via
the MMS feature of a mobile phone. The emails generated this way typically
contain additional logos from the service provider, and possible additional
text.

hook_mailsave_clean is triggered by mailsave before it calls hook_mailsave.
You can therefore intercept the node and the email attachments before
other modules begin processing.

By implementing hook_mailsave_clean in a module you can therefore discard
additional images that are attached and modify the text of the node.

hook_mailsave_clean passes the node, but also information about the mailbox
and the email that is currently in use.

--------

Verizon messages are identified by being from @vzwpix.com. The messages are
plain text, but have a long string attached to the end which is stripped.
There are no additional attachments to remove.

Filter updated 28 October 2007 - some changes suggested by Moonshine in
drupal.org/node/155440 to improve handling of variation in message format from
different handsets.

; $Id: README.txt,v 1.2 2007/10/28 13:36:08 stuartgreenfield Exp $