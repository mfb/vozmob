// $Id: README.txt,v 1.1.2.2 2009/03/06 21:02:19 mfb Exp $

FILE BAN LIST
-------------

File ban list module allows site administrators to maintain a list of 
banned files; that is, files which are not permitted to be uploaded, 
attached to nodes, etc. The file ban list consists of SHA256 sums which 
are used to (hopefully) uniquely identify files.

When a file is banned, it is copied to a new file object for archival 
purposes (e.g. determining which files have been banned).

An API is provided which developers can utilize to prevent the banned 
files from being saved and attached to nodes. This module does not yet 
prevent the upload of banned files on its own.

Views Bulk Operations module is required to provide the file management 
user interface.
