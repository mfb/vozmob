// $Id: openlayers_behavior_layerswitcher.js,v 1.1.2.7 2010/10/06 14:31:11 strk Exp $

/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

/**
 * Layer Switcher Behavior
 */
Drupal.behaviors.openlayers_behavior_layerswitcher = function(context) {
  var data = $(context).data('openlayers');
  if (data && data.map.behaviors['openlayers_behavior_layerswitcher']) {
    // Add control
    var control = new OpenLayers.Control.LayerSwitcher({
      'ascending': !!data.map.behaviors['openlayers_behavior_layerswitcher'].ascending,
      'roundedCorner' : !!data.map.behaviors['openlayers_behavior_layerswitcher'].roundedCorner,
      'roundedCornerColor' : data.map.behaviors['openlayers_behavior_layerswitcher'].roundedCornerColor
    });
    data.openlayers.addControl(control);
    control.activate();
  }
}
