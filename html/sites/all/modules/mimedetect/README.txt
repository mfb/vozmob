MimeDetect strives to provide consistent and accurate server side MIME type 
detection. It supports the PHP FileInfo Extension, the UNIX 'file' command, 
then tries to use the MIME type supplied with the file object. If everything
fails it will select a MIME type based on file extension. 

MimeDetect is Distributed with a magic database to make FileInfo based MIME
detection more consistent across servers.

Troubleshooting
===============
In some cases this database file, 'magic', does not work with your server. You
have to use the 'magic' file of your distribution, which usually comes with
File 4.x on your server.

In your settings.php, you add a new variable that tells Mimedetect the
absolute path of the magic file to use:
  $conf = array(
    'mimedetect_magic' => 'usr/share/file/magic',
  );
Replace 'usr/share/file' by the magic file path if it's different on your server.

Note: if you're still having problems just creating a new PHP script outside of
Drupal to test that you can load the database:
<?php
  $magic_file = '/usr/share/file/magic';
  $finfo = finfo_open(FILEINFO_MIME, $magic_file);
  if (!$finfo) {
    echo "Opening fileinfo database failed";
    exit();
  }
?>
Replace 'usr/share/file' by the magic file path if it's different on your server.
