// $Id: README.txt,v 1.5.2.9 2008/09/05 16:13:37 yched Exp $

NOTE: these instructions assume you are familiar with the basic concepts
of Drupal 6 theming. See http://drupal.org/theme-guide, and more
specifically http://drupal.org/node/173880, for more informations.

There are 3 levels where you can customize how the data in CCK fields
is displayed in nodes:
1) Node templates
2) Field templates
3) Formatter theme functions


1) Node templates
=================

1.a Node template suggestions
-----------------------------

All themes usually come with a default node.tpl.php template.
Drupal core lets you use the following variant (suggestion):

- node-<CONTENT_TYPE_NAME>.tpl.php
  ex: node-story.tpl.php
  If present, will be used to theme a 'story' node.

IMPORTANT : whenever you add new template files in your theme, you need to
rebuild the theme registry, or the theme engine won't see them.
You can do that by :
- visiting the admin/build/modules page
- or using devel.module's 'clear cache' link.

1.b Node template variables
---------------------------

CCK makes the following variables available in your theme's node templates:

- $<FIELD_NAME>_rendered :
Contains the rendered html for the field, including the label and all the
field's values, with the settings defined on the 'Display fields' tab.

- $<GROUP_NAME>_rendered :
Contains the rendered html for the fieldgroup (if any), including the label
and all the group's fields, with the settings defined on the 'Display fields' tab.
This variable therefore includes the html contained in all the
$<FIELD_NAME>_rendered variables for the group's fields.

- $FIELD_NAME :
Contains the raw values of the fields, in the usual array-format used internally
by CCK. What you find in there depends on the field type.
Each value also contains a 'view' element, that holds the ready-to-display value
as rendered by the formatter. For instance :
array(
  0 => array(
    'nid' => 5,
    'view' => '<a href="node/5">Title of node 5</a>',
  ),
);
Raw data are *not* sanitized for output, it is therefore *not advised* to use them
directly. Use the 'view' value, or run the values through content_format().


Note that the $content variable used by default in node templates contains the
rendered html for the whole node : body, file attachments, fivestar widgets, ...
*and* CCK fields and fieldgroups.
If you want to use the more fine-grained variables described above, you cannot use
$content because you'd get duplicate information. Your template then needs to handle
the display of all the node components itself.

1.c Special case : nodes in nodereference fields
------------------------------------------------

In addition to the above, the following suggestions will be looked for
in priority for nodes that are displayed as values of a nodereference field using
the 'teaser' or 'full node' formatters, :

- node-nodereference-<REFERRING_FIELD_NAME>-<TYPE_NAME>.tpl.php
  ex: node-nodereference-field_noderef-story.tpl.php
  If present, will be used to theme a 'story' node when refererenced in the
  'field_noderef' field.

- node-nodereference-<TYPE_NAME>.tpl.php
  ex: node-nodereference-story.tpl.php
  If present, will be used to theme a 'story' node when refererenced in any
  nodereference field.

- node-nodereference-<REFERRING_FIELD_NAME>.tpl.php
  ex: node-nodereference-field_noderef.tpl.php
  If present, will be used to a node refererenced in the 'field_noderef' field.

- node-nodereference.tpl.php
  If present, will be used to theme nodes referenced in nodereference fields.

The following additional variacles are available in templates for referenced nodes :

- $referring_field
  The nodereference field that references the current node.

- $referring_node
  The node referencing the current node.

2) Field templates
==================

Field-level theming determines how the values of a given field displayed.
The resulting output ends up in the $content and $<FIELD_NAME>_rendered variables
in the node templates.

2.a Template file
-----------------

In order to customize field themeing:

- Copy the 'content-field.tpl.php' template file into your theme's root folder
(please keep the contents of the 'cck/theme' folder untouched. For the same
reason, it is advised to *copy* the file instead of just moving it).

- Edit that copy to your liking. See the comments in cck/theme/content/content-field.tpl.php
for a list of all variables available in this template.

2.b Field template suggestions
------------------------------

In addition, the theme layer will also look for field-specific variants (suggestions),
in the following order of precedence:

- content-field-<FIELD_NAME>-<CONTENT_TYPE_NAME>.tpl.php
  ex: content-field-field_myfield-story.tpl.php
  If present, will be used to theme the 'field_myfield' field when displaying a
  'story' node.

- content-field-<CONTENT_TYPE_NAME>.tpl.php
  ex: content-field-story.tpl.php)
  If present, will be used to theme all fields of 'story' nodes.

- content-field-<FIELD_NAME>.tpl.php
  ex: content-field-field_myfield.tpl.php
  If present, will be used to theme all 'field_myfield' field in all the content
  types it appears in.

- content-field.tpl.php
  If none of the above is present, the base template will be used.

IMPORTANT:
- Suggestions work only if the theme also has the base template.
If your theme has content-field-*.tpl.php files, it must also have a
content-field.tpl.php file.

- Whenever you add new template files in your theme, you need to
rebuild the theme registry, or the theme engine won't see them.
You can do that by :
- visiting the admin/build/modules page
- or using devel.module's 'clear cache' link.

See http://drupal.org/node/223440 for more informations about templates
and template suggestions.


3) Formatter theme functions
============================

Formatters are used to turn the raw data for a single field value into html.
The 'Display Fields' tab lets you chose which formatter you want to use
for each of your fields.

In CCK 2.0 for Drupal 6, all formatters now go through the theme layer.

Overriding a formatter's theme is another way you can alter how your values
are displayed (whereas changing content-field.tpl.php lets you change the HTML
that "wraps" the values).

Most formatters come as theme functions, but some might use templates instead.
Either way, you can ovverride them using the usual D6 theme override practices
(see http://drupal.org/theme-guide).