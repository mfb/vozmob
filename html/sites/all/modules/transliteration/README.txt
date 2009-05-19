/* $Id: README.txt,v 1.4.4.1 2008/08/13 06:56:56 smk Exp $ */

-- SUMMARY --

The purpose of this module is to provide a central transliteration service for
other Drupal modules, as well as sanitizing file names while uploading new
files.

For a full description visit the project page:
  http://drupal.org/project/transliteration
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/issues/transliteration


-- REQUIREMENTS --

None.


-- INSTALLATION --

1. Copy the transliteration module to your modules directory and enable it on
   Site building > Modules (admin/build/modules).
   During installation or update all filenames containing non-ASCII characters
   will be automatically converted.

2. That's it. The names of all new uploaded files will now automatically be
   transliterated and cleaned from non-ASCII characters.


-- 3RD PARTY INTEGRATION --

Module developers who want to make use of transliteration to clean input
strings may use code similar to this:

if (module_exists('transliteration')) {
  $transliterated = transliteration_get($string);
}

Take a look at transliteration.module for an explanation of additional function
parameters.


-- LANGUAGE SPECIFIC REPLACEMENTS --

Transliteration supports language specific alterations, the following guide
helps you adding them:

1. First find out the Unicode character code you want to change. As an example
   we'll be using the cyrillic character 'г', whose code is 0x0433 (hexadecimal).

2. The first two digits (ie. '04') tell you in which file the corresponding
   mapping belongs to. In this case it's data/x04.php.

3. Open it in an editor and add your replacements to the array, for example:

   '<LC>' => array(0x33 => 'g', ...),

   Two things are important here:
   First, <LC> must be a valid Drupal language code.
   Second, keep only the last two digits of the character code (ie. 0x33,
   imagine the other two already encoded in the file name). Remember to use
   hexadecimal numbers everywhere.

Also take a look at data/x00.php which already contains a bunch of language
specific replacements. If you think your overrides are useful for others please
create and file a patch at http://drupal.org/project/issues/transliteration.


-- CONTACT --

Authors:
* Stefan M. Kudwien (smk-ka) - dev@unleashedmind.com
* Daniel F. Kudwien (sun) - dev@unleashedmind.com

This project has been sponsored by UNLEASHED MIND
Specialized in consulting and planning of Drupal powered sites, UNLEASHED
MIND offers installation, development, theming, customization, and hosting
to get you started. Visit http://www.unleashedmind.com for more information.

The transliteration is based on MediaWiki's UtfNormal.php
(http://www.mediawiki.org) and CPAN's Text::Unidecode library
(http://search.cpan.org/~sburke/Text-Unidecode-0.04/lib/Text/Unidecode.pm).

