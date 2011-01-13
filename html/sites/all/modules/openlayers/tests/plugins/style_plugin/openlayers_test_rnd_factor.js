// $Id: openlayers_test_rnd_factor.js,v 1.1.2.2 2010/10/13 13:35:35 tmcw Exp $

/**
 * @file
 * File to hold custom context styling
 */

/**
 * Style plugin context class
 */
Drupal.openlayers.style_plugin.openlayers_test_rnd_factor = function(params) {
  this.params = params;
};

/**
 * Style plugin context class methods
 */
Drupal.openlayers.style_plugin.openlayers_test_rnd_factor.prototype = {

  // Fill opacity context.  Sets random fill opacity.
  'getFactor' : function(feature) {
    // Random factor
    return Math.random();
  }

};
