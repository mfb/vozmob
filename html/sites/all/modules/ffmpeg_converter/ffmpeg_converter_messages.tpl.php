<?php
// $Id: ffmpeg_converter_messages.tpl.php,v 1.1 2009/02/08 02:03:55 zoo33 Exp $

/**
 * @file
 * Template file for the message stating that a file is being converted.
 * 
 * Available variables:
 *   $messages - an array of arrays containing the keys 'message' and 'filename'
 */

?>
<div class="ffmpeg-converter-messages">
  <?php foreach($messages as $message): ?>
    <p><?php print $message['message']; ?></p>
  <?php endforeach; ?>
</div>
