; $Id: README.txt,v 1.1.2.2 2008/10/14 15:28:14 karens Exp $

THIS MODULE IS STILL EXPERIMENTAL AND NOT READY FOR PRODUCTION!
===================================================================

To try it out, add a content_multigroup.info file to the module. 
The info file should be a plain text file that looks like:
===================================================================
name = Content Multigroup
description = Combine multiple CCK fields into repeating field collections that work in unison. This module is experimental and not ready for production.
dependencies[] = content
dependencies[] = fieldgroup
package = CCK
core = 6.x
===================================================================

USING MULTIGROUPS

The Multigroup group treats all included fields like a single field,
keeping the related delta values of all included fields synchronized.

To use a Multigroup, create a new group, make it the 'Multigroup' type,
set the number of multiple values for all the fields in the Multigroup, 
and drag into it the fields that should be included.

All fields in the Multigroup will automatically get the group
setting for multiple values. On the node form, the group is rearranged
to keep the delta values for each field in a single drag 'n drop group,
by transposing the normal array(field_name => delta => value) into
array(delta => field_name => value).

During validation and submission, the field values are restored to
their normal positions.

FIELDS AND WIDGETS THAT WORK IN MULTIGROUPS

All fields that allow the Content module to handle their multiple
values should work here. Fields that handle their own multiple values
will not be allowed into Multigroups unless they implement
hook_multigroup_allowed_widgets() to add their widgets to the allowed
widget list.

All fields that allow the Content module to handle their multiple
values should work correctly when a Multigroup is changed back to
a normal group. Fields that handle their own multiple values
which may store different results in Multigroup and standard groups should 
implement hook_multigroup_no_remove_widgets() to add their widgets 
to the list of widgets that cannot be removed from Multigroups.

If a simple array of widgets is not sufficient to test whether this
action will work, modules can implement hook_multigroup_allowed_in()
and hook_multigroup_allowed_out() to intervene.

Custom code and modules that add fields to groups outside of the UI 
should use multigroup_allowed_in() and multigroup_allowed_out()
to test whether fields are allowed in or out of a Multigroup.


TROUBLESHOOTING

The most likely cause of problems with field modules not working in 
multigroup is if they wipe out #element_validate with their own 
validation functions, or they hard-code assumptions into submit or
validation processes that the form is structured in the usual
field => delta => value order instead of allowing for the possibility 
of a different structure. See Nodereference for an example of a field
that handles validation without making assumptions about the form
structure.
