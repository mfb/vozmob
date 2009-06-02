// Media Mover javascript file

// ADD CONFIG PAGE 
// hide all the child fieldsets when the page loads, then open any that should be on
$(document).ready(function () {
  // hide all the configuration fieldsets
  $('.mm_config_fieldset').children('.mm_config_option').hide();
  
  // check each of the actions and open fieldsets if necessary
  $('.media_mover_action_select').each( function () {
    // we can check against the default media mover value
    if ($(this).val() != 'media_mover_api--1') {
      // activate
	    $('#'+$(this).attr('value')).addClass('active');
	    $('#'+$(this).attr('value')).show('normal');
    }
  });
   
});

// hide and show fieldsets from enabled modules
$(document).ready(function () {
  $('.media_mover_action_select').change( function () {

    // hide any fieldsets that are open
    $(this).parent().siblings('fieldset.active').hide('normal', function(){
      $(this).removeClass('active');
    });
    
    // show the selected fieldset
    $('#'+$(this).attr('value')).addClass('active');
    $('#'+$(this).attr('value')).show('normal');

  });
});