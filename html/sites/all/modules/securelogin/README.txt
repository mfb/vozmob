$Id: README.txt,v 1.3.2.1 2008/07/12 15:07:42 avf Exp $

Secure Login module README

This module enables secure logins by redirecting the login form to a
secure host address.  The module can also do the same for the user
edit and user registration forms, so that passwords are never send in
cleartext.

See INSTALL.txt for instructions on how to install the module.

Before enabling the module, you need to set up your server to support
SSL.  The result should be that if you Drupal site lives at
http://host.domain/dir/, it should also be accessible at
https://otherhost.domain/otherdir/ (the secure base URL, which
defaults to https://host.domain/dir/).  You must make sure that
cookies coming from otherhost.domain will be sent to host.domain.  You
can change the cookie domain in settings.php.

You can set which of the three forms (login, user edit, user
registration) are secured by this module in the module settings,
although there will usually be no reason why you would not want all
three to be secured.

If you are upgrading from an older version, note that it is no longer
necessary to set $base_url in settings.php.

