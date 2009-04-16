// $Id$

$(document).ready(function() {
  // Set up the overlay and expose behaviors.
  $('div.field-field-image div.field-items a').each(function() {
    // Get the nid for this image field and set the rel attribute to the overlay for this node.
    var nid = $(this).parents('div.node').attr('id').slice(5);
    $(this).attr('rel', '#overlay-' + nid);
    $(this).attr('href', '#overlay-' + nid);
  });
  $('div.field-field-image div.field-items a[rel]').each(function() {
    $(this).overlay({
      onBeforeLoad: function() {
        this.getBackgroundImage().expose({color: '#000'});
      },
      onLoad: function() {
        // Set up the slideshow.
        this.getContent().find('div.views-field-field-image-fid span.field-content').each(function() {
          $(this).append('<div class="overlay-links">' + $(this).parents('div.node').find('div.node-links').html() + '</div>');
          $(this).cycle({ 
            fx:    'fade',
            next:  '.overlay-next',
            prev:  '.overlay-previous',
            pause: 1
          });
          var slideshow = this;
          $('span.overlay-pause').click(function() {
            $(slideshow).cycle('pause');
          });
          $('span.overlay-play').click(function() {
            $(slideshow).cycle('resume');
          });
        });
      },
      onClose: function() {
        $.expose.close();
      }
    });
  });
});
