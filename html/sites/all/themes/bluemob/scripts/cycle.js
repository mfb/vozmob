// $Id$

Drupal.behaviors.featureCycle = function(context) {
  $('div.view-id-feature div.view-content:not(.feature-cycle-processed)', context)
    .addClass('feature-cycle-processed')
    .cycle({fx: 'scrollHorz', timeout: 10000, prev: '#feature-cycle-prev', next: '#feature-cycle-next',
     prevNextClick: function(isNext, zeroBasedSlideIndex, slideElement) { $('div.view-id-feature div.view-content').cycle('pause'); } });
}

Drupal.behaviors.overlayCycle = function(context) {
  // Set up the overlay and expose behaviors.
  $('div.field-field-image div.field-items a, div.overlay-launcher a').each(function() {
    // Get the nid for this image field and set the rel attribute to the overlay for this node.
    var nid = $(this).parents('div.node').attr('id').slice(5);
    $(this).attr('rel', '#overlay-' + nid);
    $(this).attr('href', '#overlay-' + nid);
  });
  var cycleIndex = 0;
  $('div.view-overlay div.views-field-title').each(function() {
    $(this).before('<div class="overlay-links">' + $(this).parents('div.node').find('div.node-links').html() + '</div>');
  });

  $('div.overlay-launcher a[rel]').each(function() {
    $(this).overlay({expose: '#000', close: '.close', onLoad: function() {
      if (this.getContent().find('div.media-mover-video').length || this.getContent().find('div.media-mover-audio').length) {
        this.getContent().find('div.views-field-field-image-fid').hide();
      }
    }});
  });

  $('div.field-field-image div.field-items a[rel]').each(function() {
    $(this).overlay({
      expose: '#000',
      onLoad: function() {
        if (this.getContent().find('div.media-mover-video').length) {
          this.getContent().find('div.views-field-field-image-fid').hide();
        }
        else {
          // Set up the slideshow.
          this.getContent().find('div.views-field-field-image-fid div.field-content').each(function() {
            if (!this.loaded) {
              $(this).before('<div class="cycle-nav" id="cycle-nav-' + cycleIndex + '"></div>').cycle({ 
                fx:    'fade',
                pager: '#cycle-nav-' + cycleIndex,
                pause: 1
              });
              cycleIndex = 1 + cycleIndex;
              this.loaded = true;
            }
            $(this).cycle('resume');
          });
        }
      },
      onClose: function() {
        this.getContent().find('div.views-field-field-image-fid div.field-content').each(function() {
          $(this).cycle('pause');
        });
      }
    });
  });
};
