// $Id: README.txt,v 1.12.2.3 2008/09/12 14:24:35 yched Exp $

Content Construction Kit
------------------------
To install, place the entire CCK folder into your modules directory.
Go to Administer -> Site building -> Modules and enable the Content module and one or
more field type modules:

- text.module
- number.module
- userreference.module
- nodereference.module

Now go to Administer -> Content management -> Content types. Create a new
content type and edit it to add some fields. Then test by creating
a new node of your new type using the Create content menu link.

The included optionswidget.module provides radio and check box selectors
for the various field types.

The included fieldgroup.module allows you to group fields together
in fieldsets to help organize them.

A comprehensive guide to using CCK is available as a CCK Handbook
at http://drupal.org/node/101723.

Custom theming
--------------

See the /theme/README.txt file.

Known incompatibilitie
----------------------

The Devel Themer module that ships with Devel is known to mess with CCK admin pages.
As a general rule, Devel Themer should only be switched on intermittently when doing
theme work on a specific page, and switched off immediately after that, for it adds
massive processing overhead.

Maintainers
-----------
The Content Construction Kit was originally developped by:
John Van Dyk
Jonathan Chaffer

Current maintainers:
Karen Stevenson
Yves Chedemois

