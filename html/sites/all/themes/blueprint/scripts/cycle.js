// $Id$

$(document).ready(function() {
  // Set up the slideshow.
  var cycleIndex = 0;
  $('div.field-field-image div.field-items, div.view-overlay span.field-content').each(function() {
    $(this).before('<div class="cycle-nav" id="cycle-nav-' + cycleIndex + '"></div>').cycle({ 
      fx:    'fade',
      pager: '#cycle-nav-' + cycleIndex,
      pause: 1
    });
    cycleIndex = 1 + cycleIndex;
  });
  // Set the height of the field since its images are now absolutely positioned.
  $('div.field-field-image div.field-items, div.view-overlay span.field-content').each(function() {
    // This won't work so well for a set of images with wildly different heights.
    $(this).height($(this).find('img').outerHeight());
  });
  // Set up the overlay and expose behaviors.
  $('div.field-field-image div.field-items').each(function() {
    $(this).overlay({
      onBeforeLoad: function() {
        this.getBackgroundImage().expose({color: '#000'});
      },
      onClose: function() {
        $.expose.close();
      }
    });
  });
});
