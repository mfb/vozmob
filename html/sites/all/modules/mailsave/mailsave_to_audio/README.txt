mailsave to audio
-----------------
THIS MODULE IS BETA

Mailsave to image is an extension for mailsave that works with audio.module.
When activated, and if the user has "convert to audio node" privileges then
the first mp3 in an email will cause the node to be converted to an audio
node. At this time the title is taken from the body text, rather than the
ID3 tags.

At this time if mailsave to audio is successful in saving the node then no
further node processing occurs and other attachments are discarded.

You do not have to enable upload / save attachment privileges to use
mailsave to audio.

; $Id: README.txt,v 1.2 2007/10/23 22:04:02 stuartgreenfield Exp $