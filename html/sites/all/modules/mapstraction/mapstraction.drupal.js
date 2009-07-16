// $Id: mapstraction.drupal.js,v 1.1.2.9 2009/06/30 14:44:51 dixon Exp $
Drupal.behaviors.mapstraction = function (context) {
  Drupal.mapstraction = [];
  $(Drupal.settings.mapstraction).each(function(index) {
    var id = 'mapstraction-' + this.viewName + '-' + this.currentDisplay;
    Drupal.mapstraction[id] = new Mapstraction(id, this.apiName);

    // Set up hover behaviour.
    var hover = this.behaviours.hover;

    // Set up markers and info bubbles.
    $(this.markers).each(function(index) {
      var markerPoint = new LatLonPoint(Number(this.lat), Number(this.lon));
      marker = new Marker(markerPoint);
      marker.setInfoBubble(this.title);
      marker.setIcon(this.icon);
      marker.setHover(hover);
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
};
