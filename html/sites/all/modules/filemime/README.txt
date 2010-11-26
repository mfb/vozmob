// $Id: README.txt,v 1.1.2.5 2008/11/07 21:07:38 mfb Exp $

Drupal determines the MIME type of each uploaded file by applying a MIME 
extension mapping to the file name.  The default mapping is hard-coded 
in the file_get_mimetype() function: 
http://api.drupal.org/api/function/file_get_mimetype

This module allows site administrators to replace the built-in mapping.  
For example, you may wish to serve FLAC files as audio/x-flac rather 
than application/x-flac.

Custom mappings can be extracted from the server's mime.types file 
(often available on a path such as /etc/mime.types) and/or a 
site-specific mapping string, both of which must use the standard syntax 
for mime.types files.  For example:

text/html	html htm
text/plain	txt

After installing and enabling this module, the MIME extension mapping 
can be customized by visiting Administer > Site configuration > File 
MIME (admin/settings/filemime).

Once a custom mapping is configured, the built-in mapping will no longer 
be available, so all desired mappings must be explicitly set, either in 
the mime.types file or in the additional mappings text box.  You cannot 
simply append extra mappings to the built-in mapping.

Uninstalling this module will delete the mime_extension_mapping 
variable, thus restoring Drupal's built-in mapping.
