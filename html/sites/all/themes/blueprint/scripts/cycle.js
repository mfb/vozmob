// $Id$

$(document).ready(function() {
  // Set up the slideshow.
  var cycleIndex = 0;
  $('div.field-type-image div.field-items').each(function() {
    $(this).before('<div class="cycle-nav" id="cycle-nav-' + cycleIndex + '"></div>').cycle({ 
      fx:    'fade',
      pager: '#cycle-nav-' + cycleIndex,
      pause: 1
    });
    cycleIndex = 1 + cycleIndex;
  });
  // Set the height of the field since its images are now absolutely positioned.
  $('div.field-type-image div.field-items').each(function() {
    // This won't work so well for a set of images with wildly different heights.
    $(this).height($(this).find('img').outerHeight());
  });
});
