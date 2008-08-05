mms_se_telia
------------
Messages sent via the Telia network are slightly strange - the attachment file
name is not in the usual parameters/dparameters areas, but is hidden in the
Content-Location header of the mime part. Mailsave and mailhandler therefore
do not find an attachment name and treat the item as "Unknown".

This clean up filter does some post-processing to retrieve the relevant name
from Content-Location by requerying the message and using a regex to get the
filename.

If you are not bothered about the filename then if you do not enable this
filter mailsave/mailhandler will assign the filename unnamed_attachment.

Note: some tests with a messages sent via Verizon (US) showed that the email
structure varied according to the mobile device used to send the message, so
this filter may not be effective for all devices - I only have one message to
test on, so I have coded against that example!

; $Id: README.txt,v 1.2 2007/10/23 22:04:02 stuartgreenfield Exp $