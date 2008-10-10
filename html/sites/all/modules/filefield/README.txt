filefield provides a file field type to CCK.

In order to make files show up on a page, you need to do three things:

-> Enable the "view filefield uploads" permission for all users that
   should be able to view these files.

-> Add a CCK field of the type "File" to any content type.

-> Create a node of this content type, upload some file(s),
   and set the "List" option for those files that should show up on the node.
