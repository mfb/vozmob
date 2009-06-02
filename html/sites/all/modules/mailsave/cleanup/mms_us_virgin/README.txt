mms_us_virgin
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
and the email that is currently in use. In the case of Virgin Mobile an email
can be identified as originating from them as the sending email will be
<yourphonenumber>@vmpix.com. By looking for this with
a regex we know we have a Virgin Mobile message.

Virgin Mobil adds a line "This message is from a Virgin Mobile user. Enjoy."
at the beginning of the message's body.

; $Id: README.txt,v 1.1 2009/05/31 13:13:41 gaba Exp $
