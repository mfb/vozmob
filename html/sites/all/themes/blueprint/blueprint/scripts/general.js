// generic JS fixes

// various JavaScript object.
var Blueprint = {};

// jump to the value in a select drop down
Blueprint.go = function(e) {
  var destination = e.options[e.selectedIndex].value;
  if (destination && destination != 0) location.href = destination;
};

