// $Id: README.txt,v 1.1.2.4 2009/03/30 16:35:41 diggersf Exp $

Description
===========
This module delivers support for the [Mapstraction javascript library][1], which
provides an abstraction layer for the various map providers including Google,
Yahoo!, and MapQuest. It allows you to quickly display maps on your site from
multiple providers and switch between providers without worrying about
differences in their APIs.

The module provides a style for the Views module. When the style is used, it
will display nodes as points on a map. The latitude/longitude points can be
provided by any view field including CCK float fields or Location module
fields.

[1]: http://www.mapstraction.com/

Installation
============
1. Place in your modules directory
2. Download Mapstraction from the following URL and place it in the module
   directory:
     http://mapstraction.com/svn/source/compressed/mapstraction.js
3. Enable the module at admin/build/modules
4. Configure a view with the proper style at admin/build/views

Configuration
=============
1. Configure a node type with Location or CCK to provide fields for latitude
   and longitude.
2. Create a new view and select Mapstraction as the style.
3. Add title, latitude, and longitude as view fields.
4. Configure the style by selecting your desired Mapping API, controls, and
   field mapping.