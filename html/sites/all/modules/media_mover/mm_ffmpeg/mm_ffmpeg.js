// $Id: mm_ffmpeg.js,v 1.1.2.7 2008/07/11 19:26:45 arthuregg Exp $

if (Drupal.jsEnabled) {
  $(document).ready(function () {
  
    mm_ffmpeg_size_other('ffmpeg-video-size');
    mm_ffmpeg_size_other('ffmpeg-thumb-size');
    
    function mm_ffmpeg_size_other(name) {
      
	    $('select.'+name).bind('change', function () {
	      if ($(this).val() == 'other'){
	        $(this).parents('.form-item').siblings('.'+name+'-other').show('slow');
	       }
	       else {
	         $(this).parent('.form-item').siblings('.'+name+'-other').hide('slow');
	       }
	     });
	  }
    
  });
}