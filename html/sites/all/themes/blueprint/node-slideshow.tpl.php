<style type="text/css">
body { padding-bottom: 20px }
#main { background: #fff; margin: 0 20px 20px 20px }
#main h1 { padding:0; margin-top: 20px }
#main h2 { text-align: left }
#header { background-color: #eee; font-weight: normal; margin:0; padding:10px; font-size: small }

table { margin: auto; border-collapse: separate; border-spacing: 20px }
td { vertical-align: top; text-align:center; width: 235px }
hr { margin-top: 20px; }

pre { text-align: left; overflow: visible }
code { background-color: #ffc }
pre code { background-color: #eee }

.pics { height: 232px; width: 232px; padding:0; margin:0; overflow: hidden }
.pics img { height: 200px; width: 200px; padding: 15px; border: 1px solid #ccc; background-color: #eee; top:0; left:0 }
#slideshow { position: relative; width: 230px; margin: 35px; }
#controls { z-index: 1000; position: absolute; top: 0; left: 0; display: none;
    background-color: #ffc; border: 1px solid #ddd; margin: 0; padding: 6px; 
    width: 218px;
}
#controls span { margin: 0 5px }
</style>
<script type="text/javascript">
$(function() {

    if (esMMS()) {
    $('#pause').click(function() { $('#slides').cycle('pause'); $("#sound_player").pause();  return false; });
    $('#play').click(function() { $('#slides').cycle('resume'); $("#sound_player").pause(); return false; });
    
    $('#slideshow').hover(
        function() { $('#controls').fadeIn(); },
        function() { $('#controls').fadeOut(); }
    );

    //get the timeout for the slideshow from the lenght of the audio file
    var playtime = "<?php print $node->field_audio_playtime[0]['value']; ?>";
    thisOne = document.getElementById("slides");
    fotos = thisOne.getElementsByTagName("img");
    var cant_fotos = fotos.length;
    var timeout = playtime/cant_fotos;

    $('#slides').cycle({
        fx:     'fade',
        speed:   400,
        timeout: timeout
    });

    inicializar(); 
    }
});


function inicializar(){

	var url = "<?php print $node->field_audio[0]['filepath']; ?>";
	var player = "/sites/all/themes/garland/scripts/Player.swf";

	//get the flash player and load the audio file from the node
	$("#sound_player").sound({swf: player});
	$("#sound_player").load(url);
};

function esMMS(){
	return ("<?php print sizeof($node->field_audio) > 0 && sizeof($node->field_image) > 0 ?>");
}

/*
jQuery(document).ready(function() {

	if (esMMS()) {	inicializar(); }

	//while ($('#sound_player').isPlaying()) {
	//	// wait until the audio ends
	//}
        //$('#slides').cycle('stop');
});*/
</script>

<?php
// $Id: node.tpl.php,v 1.5 2007/10/11 09:51:29 goba Exp $
?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

<?php print $picture ?>

<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

  <?php if ($submitted): ?>
    <span class="submitted"><?php print $submitted; ?></span>
  <?php endif; ?>


<br>image:
<?php print sizeof($node->field_image) ?><br>audio:
<?php print sizeof($node->field_audio) ?>
<br><br>


<?php if (sizeof($node->field_audio) > 0 && sizeof($node->field_image) > 0): ?>
	<?php print $node->content['body']['#value'] ?>
	<div id="slideshow">
	    <div id="controls">
        	<span><a href="" id="pause">Pause</a></span>
	        <span><a href="" id="play">Play</a></span>
	    </div>
	    <div id="slides" class="pics">                           
	<?php 
	     foreach ($node->field_image as $item) :
        	    if (!$item['empty']) : 
	?>
        	<img src="<?php print $item['filepath'];?>" width="200" height="200" />
           
		 <?php      endif;
	     endforeach;
?>

	    </div>
	</div>
	<div id="sound_player"></div>

<?php else: ?>

<div class="content clear-block">
    <?php print $content ?>
  </div>

  <div class="clear-block">
    <div class="meta">
    <?php if ($taxonomy): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
    </div>

    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

</div>

<?php endif; ?>
