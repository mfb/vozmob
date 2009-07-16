// $Id: jquery_ui.js,v 1.2 2009/06/29 00:09:27 sun Exp $

(function ($) {

/**
 * @file
 * Provides the jQuery UI Drupal behaviors.
 */

/**
 * The jQuery UI Drupal behavior.
 *
 * This will go through all widgets and apply them to the given selectors with
 * the appropriate arguments.
 */
Drupal.behaviors.jqueryui = {
  attach: function(context, settings) {
    if (settings.jqueryui) {
      // Iterate through each widget and apply the tool to the elements.
      jQuery.each(settings.jqueryui, function(ui, elements) {
        // Iterate through each jQuery UI tool and apply the effects.
        jQuery.each(elements, function(selector, options) {
          // Apply the jQuery UI's effect.
          $(selector + ':not(.jqueryui-' + ui + '-processed)', context).addClass('jqueryui-' + ui + '-processed')[ui](options);
        });
      });
    }
  }
}

})(jQuery);
