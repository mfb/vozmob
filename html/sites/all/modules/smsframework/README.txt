$Id: README.txt,v 1.2.2.2 2009/02/05 23:50:07 diggersf Exp $

Description
===========
Provides a pluggable API for Drupal to interact with SMS messages. 

* sms.module - the core API module
* sms_blast.module - for sending a batch of messages
* sms_clickatell.module - support for the Clickatell gateway (http://clickatell.com/)
* sms_email_gateway.module - support for email gateways
* sms_sendtophone.module - input filter and node links for sending snippets of text
* sms_user.module - provides integration with Drupal users

Also integrates well with the Messaging module (http://drupal.org/project/messaging).

More information is available from the groups homepage at: http://groups.drupal.org/sms-framework

Installation
============
1. Drop into your preferred modules directory
2. Enable the modules you want to use from admin/build/modules
3. Set and configure your default gateway at admin/smsframework/gateways
4. Check out the settings pages at admin/smsframework

Documentation
=============
Documentation for site builders and developers is available in the handbook
pages on Drupal.org at the following URL:

http://drupal.org/node/362258

Credits
=======
Will White of Development Seed (http://drupal.org/user/32237)
Tylor Sherman of Raincity Studios (http://drupal.org/user/143420)