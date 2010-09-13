= Masquerade =

The Masquerade module allows users to temporarily switch to another user
account. It keeps a record of the original user account, so users can easily
switch back to the previous account.

== Installation and Configuration ==

To install the Masquerade module, extract the module to your modules folder,
such as sites/all/modules. After enabling the module, it can be configured
under Administer > Site configuration > Masquerade. To enable users to
masquerade, assign the appropriate "masquerade module" permissions to the roles
available on your site. For example:

 * To allow members of the 'customer support' role to masquerade as any
   non-admin user, add the 'masquerade as user' permission to the role. In the
   Masquerade configuration, set 'site administrators' as an administrator role
   to prevent customer support users from masquerading as those users.

 * To allow members of the 'tech support' role to masquerade as 'site
   administrators', add the 'masquerade as admin' permission to the role. Then,
   in the Masquerade configuration, set 'site administrators' as an
   administrator role.

== Module Weights ==

Some modules may conflict with Masquerade depending on their weights. Modules
known to conflict include:

 * [Organic Groups](http://drupal.org/project/og)
 * [Global Redirect](http://drupal.org/project/globalredirect)

By default, the weight of Masquerade is set to -10. If there are conflicts with
other modules, you can change the weights of modules on your site by:

1. Installing the [Weight](http://drupal.org/project/weight) or
[Utility](http://drupal.org/project/util) modules to configure the weights of
modules on your site.

2. Running the following SQL query in your database to change the weight of the
Organic Groups (for example) module:

    UPDATE system SET weight = 1 WHERE name = 'og'; 

== Help and Support ==

This module was developed by a number of contributors. For more information
about this module, see:

Project Page: http://drupal.org/project/masquerade
Issue Queue: http://drupal.org/project/issues/masquerade
