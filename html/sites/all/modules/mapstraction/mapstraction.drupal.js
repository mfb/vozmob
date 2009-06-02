// $Id: mapstraction.drupal.js,v 1.1.2.7 2009/05/06 15:04:14 dixon Exp $
$(document).ready(function() {
  Drupal.mapstraction = [];
  $(Drupal.settings.mapstraction).each(function(index) {
    var id = 'mapstraction-' + this.viewName + '-' + this.currentDisplay;
    Drupal.mapstraction[id] = new Mapstraction(id, this.apiName);

    // Set up markers and info bubbles.                                                                                                                                                                                                                      
    $(this.markers).each(function(index) {
      var markerPoint = new LatLonPoint(Number(this.lat), Number(this.lon));
      marker = new Marker(markerPoint);
      marker.setInfoBubble(this.title);
      marker.setIcon(this.icon);
      Drupal.mapstraction[id].addMarker(marker);
    });

    // Set up controls.                                                                                                                                                                                                                     
    Drupal.mapstraction[id].addControls(this.controls);

    // Set auto center and zoom.                                                                                                                                                                                                            
    if (this.initialPoint.auto) {
      Drupal.mapstraction[id].autoCenterAndZoom();
    }
    else {
      var startPoint = new LatLonPoint(Number(this.initialPoint.latitude), Number(this.initialPoint.longitude));
      Drupal.mapstraction[id].setCenterAndZoom(startPoint, Number(this.initialPoint.zoom));
    }
  });
});
