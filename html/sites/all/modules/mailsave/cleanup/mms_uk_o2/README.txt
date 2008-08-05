mms_uk_o2
---------
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
and the email that is currently in use. In the case of T-Mobile an email
can be identified as originating from them as the sending email will be
<yourphonenumber>@mediamessaging.o2.co.uk. By looking for this with
a regex we know we have an O2 message.

More samples are needed as the one received so far seemed to set the title
to the body of the message, and there was no body text. Therefore at present
the body and teaser are set to match the title.

; $Id: README.txt,v 1.1 2007/05/27 23:45:40 stuartgreenfield Exp $