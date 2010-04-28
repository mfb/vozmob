// $Id: mm_node.js,v 1.1.2.2 2010/03/09 02:08:42 arthuregg Exp $

/**
 * @file
 * Provides JS behaviors
 */


Drupal.behaviors.MMNode = function (context) {
  // Hide the title form on page load
  if ($("input[name='storage--mm_node--4--node_title_options']").val() != 'title') {
    $('#mm_node_title_default').hide();
  }
  else {
    $('#mm_node_title_default').addClass('open');
  }

  // Bind to the selection
  $("input[name='storage--mm_node--4--node_title_options']").bind('change', function () {
    if ($(this).val() == 'title') {
      $('#mm_node_title_default').show('slow').addClass('open');
	}
    else {
      // If the title form is open, close it
      if ($('#mm_node_title_default').hasClass('open')) {
        $('#mm_node_title_default').hide('slow').removeClass('open');
      }
    }
  });

}