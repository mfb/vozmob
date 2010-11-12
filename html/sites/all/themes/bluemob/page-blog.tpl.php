<?php // $Id: page.tpl.php,v 1.15.4.7 2008/12/23 03:40:02 designerbrent Exp $ ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
	"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>">
<head>
	<title><?php print $head_title ?></title>
	<meta http-equiv="content-language" content="<?php print $language->language ?>" />
	<?php print $meta; ?>
  <?php print $head; ?>
  <?php print $styles; ?>
  <!--[if lte IE 7]>
    <link rel="stylesheet" href="<?php print $path; ?>blueprint/blueprint/ie.css" type="text/css" media="screen, projection">
  	<link href="<?php print $path; ?>css/ie.css" rel="stylesheet"  type="text/css"  media="screen, projection" />
  <![endif]-->  
  <!--[if lte IE 6]>
  	<link href="<?php print $path; ?>css/ie6.css" rel="stylesheet"  type="text/css"  media="screen, projection" />
  <![endif]-->  
</head>

<body class="<?php print $body_classes; ?><?php if (!arg(1)) print ' page-blogs'; ?>">



<div id="header_wrap"><!--header_wrap to contain header outside of main container div-->

  <div id="header">
  
  <div id="headerinner"><?php print $header; ?></div>
    <h1 id="logo">
      <a title="<?php print $site_name; ?><?php if ($site_slogan != '') print ' &ndash; '. $site_slogan; ?>" href="<?php print url(); ?>"><?php print $site_name; ?><?php if ($site_slogan != '') print ' &ndash; '. $site_slogan; ?></a>
    </h1>
    
   
  </div>
  
   <div id="menu">
    
    <?php if (isset($primary_links)) : ?>
      <?php print theme('links', $primary_links, array('id' => 'nav', 'class' => 'header_links')) ?>
    <?php endif; ?>
    <?php if (isset($secondary_links)) : ?>
      <?php print theme('links', $secondary_links, array('id' => 'subnav', 'class' => 'header_links')) ?>
    <?php endif; ?>    
    </div>
</div><!--end #header_wrap-->

<div class="container">

  <?php if ($left): ?>
    <div class="<?php print $left_classes; ?>"><?php print $left; ?></div>
  <?php endif ?>
  
  <div class="<?php print $center_classes; ?>">
    <?php
      if ($breadcrumb != '') {
        print $breadcrumb;
      }

      if ($tabs != '') {
        print '<div class="tabs">'. $tabs .'</div>';
      }

      if ($messages != '') {
        print '<div id="messages">'. $messages .'</div>';
      }
      
      if (arg(1)) print theme('user_picture', user_load(arg(1)));
      if ($title != '') {
        print '<h2>'. $title .'</h2>';
      }      

      print $help; // Drupal already wraps this one in a class      
      print $content;
      print $feed_icons;
    ?>


        <div id="">

    <?php if ($footer_message | $footer): ?>
      <div id="" class="clear">
       
       
        
        
   </div>     
      </div>
    <?php endif; ?>
  </div>

  <?php if ($right): ?>
    <div class="<?php print $right_classes; ?>"><?php print $right; ?></div>
  <?php endif ?>
  

  <?php print $scripts ?>
  <?php print $closure; ?>

</div>

<div id="footer">
<div id="footer2">
<?php print theme('image', path_to_theme() . '/images/copy_left.png', t('Copyleft')); ?>
<ul>
<li><a href ="">Contact Info</a></li>
<li>/</li>
<li><a href ="">About Vozmob</a></li>
<li>/</li>
<li><a href ="">Subscribe</a></li>
<li>/</li>
<li><a href ="">Login</a></li>
<li>/</li>
</ul>
<?php print theme('image', path_to_theme() . '/images/idepsca.png', 'IDEPSCA'); ?>
<?php print theme('image', path_to_theme() . '/images/annenberg.png', 'Annenberg'); ?>
<p>Powered by</p>
<?php print theme('image', path_to_theme() . '/images/drupal.png', 'Drupal'); ?>


<?php if ($footer): ?>
         
        <?php endif; ?>
        
        <?php if ($footer_message): ?>

        <?php endif; ?></div></div>
</body>
</html>

