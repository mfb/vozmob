; $Id: README.txt,v 1.1.2.1 2009/08/14 18:59:20 arthuregg Exp $

Mail To Organic Groups
--------------------------------------
This module works in concert with MailHandler to provide email to OG functionality.
You may also want to use Mail2Web to provide full email support


Drupal Setup
--------------------------------------
 * Install this module and enable it
 * Make sure you have MailHandler enabled and a mailbox setup to retrive mail from at least one account
 * Make sure you have OG enabled.
 * Add the Mail to OG CCK widget to your group content type
 * Create a new group.
 * Navigate to admin > settings > Mail to OG
    * Select which groups you want email enabled on, which mailbox you want to use, and a default email domain to use (ie: you expect GROUP_NAME@mydomain.com)
 * Enable permissions for users to be able to send email to groups
 * Enable the "Email information for Mail2og" block (will only display on group pages)

Email Setup (on your server)
--------------------------------------
The easiest way to configure the email system for mail2og is to have email
aliases which point to a single box. If you can use a catch all alias, this is
the easiest way to maintain the email system. 

If you can not setup a catch all, you will want to do something like this:
  group_name@mydomain.com -> retrieve@domain.com
  
If you can setup a catch all, you will want to do:
  *@domain.com -> retrive@domain.com
  

Usage
--------------------------------------
Users who have permissions can send email to GROUP_NAME@mydomain.com. If a group name
(title) has capitals or spaces in it, the email address will will be all lowercase
and replace spaces with underscores. For example: "My Great Group" would become:
  my_great_group@mydomain.com
The system will also replace a dash (-) if a user uses this by accident. 
Group admins can also set the email address of their group, however it will be automatically
generated if it is not present.

