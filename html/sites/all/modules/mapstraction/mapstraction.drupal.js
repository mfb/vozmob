$(document).ready(function() {
  Drupal.mapstraction = [];
  $(Drupal.settings.mapstraction).each(function(index) {
    var id = 'mapstraction-' + this.viewName + '-' + this.currentDisplay;
    Drupal.mapstraction[id] = new Mapstraction(id, this.apiName);
    
    // Set start point
    var startPoint = new LatLonPoint(38.92, -77.04);
    Drupal.mapstraction[id].setCenterAndZoom(startPoint, 13);
    
    // Set up controls
    Drupal.mapstraction[id].addControls(this.controls);
    
    // Set up markers & info bubbles
    $(this.markers).each(function(index) {
      marker = new Marker(new LatLonPoint(this.lat, this.lon));
      marker.setInfoBubble(this.title);
      Drupal.mapstraction[id].addMarker(marker);
    });
  });
});
