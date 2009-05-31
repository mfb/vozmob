; $Id; 

Media Mover CCK Integration
----------------------------

Please note: using cck file replacement will likely *not* work with private files. 
Further,  inorder to get nonlocal (eg: S3) files to work, this module overides the
theme_filefield_file($file) function to get the non-local files. I don't
like how this works, but I am unclear on how it maybe possible to get around 
this issue. This gets at the fundamental issues with file handling in Drupal 
and won't be solved until D7 (hopefully).