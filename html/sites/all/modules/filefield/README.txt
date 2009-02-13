filefield provides a file field type to CCK.

In order to make files show up on a page, you need to do three things:

-> Add a CCK field of the type "File" to any content type.

-> If the Content Permissions module is enabled:
       Enable the "view <fieldname>" permission for all roles that should be able to view these files.

   Otherwise:
       Enable the "access content" permission for all roles that should be able to view filefield uploads.


-> Create a node of this content type, upload some file(s),
   and set the "List" option for those files that should show up on the node.
