Drupal service links module 2.x:
---------------------------------
Original Author: Fredrik Jonsson fredrik at combonet dot se
Current Maintainer: Sivanandhan, P. apsivam .at. apsivam .dot. in
Co Mantainer and starter of 2.x branch: Fabio Mucciante aka TheCrow
Requires - Drupal 6
License - GPL (see LICENSE)

Overview:
---------
The service links module enables admins to add the following
links to nodes:
* del.icio.us - Bookmark this post on del.icio.us
* Digg - Submit this post on digg.com
* Facebook - Submit this URL on Facebook
* Furl - Submit this post on Furl
* Google - Bookmark this post on Google
* IceRocket - Search IceRocket for links to this post
* LinkedIn - Share the post in LinkedIn
* ma.gnolia.com - Bookmark this post on ma.gnolia.com
* MySpace - Bookmark this post in MySpace
* Newsvine - Submit this post on Newsvine
* PubSub - Search PubSub for links to this post
* Reddit - Submit this post on reddit.com
* StumbleUpon - Bookmark this post on StumbleUpon
* Technorati - Search Technorati for links to this post
* Twitter - Submit this post on Twitter
* Yahoo Buzz - Buzz it
* Yahoo - Bookmark this post on Yahoo

Through plugin service links support too:
* Favorite bookmark - it work for IE and Firefox
* Forward - require forward module

The site owner can deside:
- To show the links as text, image or both.
- What node types to display links for.
- If the links should be displays in teaser view or full page view
  or both.
- If the links should be added after the body text or in the links
  section or in a block or in a block with Fisheye effect
- If aggregator2 nodes should use link to original article aggregated
  by aggregator2 module.
- Deside what roles get to see/use the service links.

Installation and configuration:
------------------------------
Copy the whole 'service_links' folder under your 'modules' directory and then 
enable the modules 'Service Links' and 'General Services' at 'administer >> modules'.

Go to 'administer >> access control' for allow users to watch the links.

For configurate the options go to at 'administer >> settings >> service_links'.
Under the tab 'Services' sort and enable the services needed.


Add links to new services:
--------------------------
2.x branch introduce a fast and less intrusive method for expand the number of services
supported: 

1) Create your own module under 'services/' folder with standard 
  '.info' and '.module' files (watch general_services as basic example).

  .module file must implement the hook_service_links() that return an array like: 

  function myaddon_service_links() {
    $links = array();

    $links['myservice'] = array(
      'name' => 'My Service',
      'link' => 'http://myservice.com/?q=<encoded-url>&title=<encoded-title>',
      'description' => t('Bookmark it on My Service'),
    );

    ...

    return $links;
  }

  Notes:
  i) be sure that 'myservice' (know as 'service-id') is unique;
  ii) tags allowed: <encoded-url>, <encoded-title>, <encoded-teaser>, <source>, <teaser>, <node-id>, <short-url>

2) Put the related standard icon (myservice.png) under 'images/' folder .

  Notes:
  i) standard filename must be the same of service-id + .png 
  ii) for overwrite the standard filename just include the key 'icon':
    $links['myservice'] = array(
      ...
      'icon' => drupal_get_path('module', 'myservice') .'/anothername.gif',
    );

3) Enable the module under admin >> modules page and under settings >> service links >> services
  complete the job!

Include service links in your theme:
-----------------------------------
In the included template.php file there  an example how to insert
the service links in to a PHPTemplate theme. Remember to place the
template.php file in the folder of your theme or integrate it with 
the content of 'template.php' provided from your theme.


Last updated:
------------
$Id: README.txt,v 1.11.4.3 2009/08/14 00:36:27 robloach Exp $
