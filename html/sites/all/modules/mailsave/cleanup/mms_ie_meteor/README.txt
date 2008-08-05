mms_ie_meteor
-------------
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
and the email that is currently in use. In the case of Meteor an email
can be identified as originating from them as the sending email will be
<yourphonenumber>@mms.mymeteor.ie. By looking for this with
a regex we know we have a Meteor message.

Meteor sends the image as part of the message, and an attachement containing
the text.

; $Id: README.txt,v 1.1 2007/05/27 23:45:42 stuartgreenfield Exp $