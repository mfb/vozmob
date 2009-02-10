// $Id: README.txt,v 1.3 2008/06/21 22:58:43 webchick Exp $

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation
 * API


INTRODUCTION
------------

Authors:
* Jeff Robbins (jjeff)
* Angela Byron (webchick)
* Addison Berry (add1sun)

This Module Made by Robots: http://www.lullabot.com

jQuery UI (http://ui.jquery.com/) is a set of cool widgets and effects that
developers can use to add some pizazz to their modules.

This module is more-or-less a utility module that should simply be required by
other modules that depend on jQuery UI being available. It doesn't do anything
on its own.


INSTALLATION
------------

1. Copy the jquery_ui module directory to your sites/SITENAME/modules
   directory.

2. Download the "Development bundle" of jQuery UI from
   http://ui.jquery.com/download.

3. Extract it as a sub-directory called 'jquery.ui' in the jquery_ui folder:

     /sites/all/modules/jquery_ui/jquery.ui/

4. Enable the module at Administer >> Site building >> Modules.

5. If desired, configure the module at Administer >> Site configuration >>
   jQuery UI. Here you may select which level of compression the jQuery library
   should use. It defaults to 'minified' compression, which strips comments and
   white space.


API
---

Developers who wish to use jQuery UI effects in their modules need only make
the following changes:

1. In your module's .info file, add the following line:

     dependencies[] = jquery_ui

   This will force users to have the jQuery UI module installed before they can
   enable your module.

2. In your module, call the following function:

     jquery_ui_add($files);

   For example:

     jquery_ui_add(array('ui.draggable', 'ui.droppable', 'ui.sortable'));
     
     jquery_ui_add('ui.sortable');  // For a single file

   See the contents of the jquery.ui-X.X sub-directory for a list of available
   files that may be included, and see http://ui.jquery.com/docs for details on
   how to use them. The required ui.core file is automatically included, as is
   effects.core if you include any effects files.

   If you wish to override the compression type selected in the settings
   screen, you may do so by passing in an optional $type parameter. Possible
   values are 'none', 'minified', and 'packed'.

     jquery_ui_add(array('ui.draggable', 'ui.droppable', 'ui.sortable'), 'none');

