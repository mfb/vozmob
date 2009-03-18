<?php
// $Id: ffmpeg_converter_video.tpl.php,v 1.2 2009/01/12 11:25:09 zoo33 Exp $

/**
 * @file
 * Template file for converted video files.
 * 
 * Available variables:
 *   $file
 *   $file_path
 *   $snapshot
 *   $snapshot_path
 *   $description
 *   $nid
 *   $width
 *   $height
 *   $link_href
 */

?>

<?php if ($link_href): ?>
  <a href="<?php print $link_href ?>" id="ffmpeg-converter-<?php print $nid ?>"><img src="<?php print $snapshot_path ?>" alt="<?php print $description ?>" /></a>
<?php else: ?>
  <img src="<?php print $snapshot_path ?>" alt="<?php print $description ?>" />
<?php endif; ?>
