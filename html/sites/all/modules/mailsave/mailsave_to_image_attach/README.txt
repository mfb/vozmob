mailsave to image_attach
------------------------
This is an additional module for handling images that are submitted by email.
It works in conjuction with mailsave and image_attach.

The original mailsave_to_image module converts the node type to image if an
email contains a jpeg. However, this means that the "original" node type is
altered which may not always be desirable.

mailsave_to_image_attach uses the image_attach feature of the image module to
create a separate node that contains the image file. This node is then linked
to the main content node.

To enable this feature the required node types must be configured to use
image_attach, and the user must have "allow images from email to be attached"
permission for the mailsave_to_image_attach module.

Note that when the permission is active it applies to all node types - any
node that can accept image attachments will have images attached if they
are supplied via an incoming email.

Additional files, or multiple jpegs, will be attached, if the user has upload
"upload files" and mailsave "save attachments" privileges.

You do not have to enable upload / save attachment privileges to use
mailsave to image attach.

; $Id: README.txt,v 1.1 2007/08/16 14:17:19 stuartgreenfield Exp $