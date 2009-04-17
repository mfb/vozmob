// $Id: flowplayer.admin.js,v 1.1.2.2 2009/02/24 18:00:56 robloach Exp $

/**
 * @file
 * Provides the Drupal behavior for the Flowplayer administration page.
 */

/**
 * The Flowplayer Drupal administration behavior.
 */
Drupal.behaviors.flowplayeradmin = function() {
  // Update both the Flowplayer preview and the textbox background whenever a textbox gets changed.
  function updateTextBox(color, object) {
    $(object).css({
      'backgroundColor': color,
      'color': Drupal.settings.flowplayerAdminFarbtastic.RGBToHSL(Drupal.settings.flowplayerAdminFarbtastic.unpack(color))[2] > 0.5 ? '#000' : '#fff'
    });

    var target = $(object).attr('rel');
    var player = $f('flowplayer-preview');
    if (player) {
      player.getControls().css(target, color);
    }
  }

  // Create the Farbtastic color picker
  Drupal.settings.flowplayerAdminFarbtastic = $.farbtastic('#flowplayer-color-picker', function(color) {
    $(Drupal.settings.flowplayerAdminTextbox).val(color);
    updateTextBox(color, Drupal.settings.flowplayerAdminTextbox);
  });

  // Make the focus of the textbox change the input box we're acting on.
  $('#flowplayer-color input:text').focus(function() {
    Drupal.settings.flowplayerAdminTextbox = this;
  });

  // Colour the text boxes their value color.
  $('#flowplayer-color input:text').each(function(index, object) {
    var value = $(object).val();
    if (value) {
      updateTextBox($(object).val(), object);
    }
  });
};

/**
 * Called when the Flowplayer is initialized.
 */
function flowplayer_admin_init() {
  var player = $f('flowplayer-preview');
  // Colour the text boxes their value color.
  $('#flowplayer-color input:text').each(function(index, object) {
    var target = $(object).attr('rel');
    var color = $(object).val();
    if (target && color) {
      player.getControls().css(target, color);
    }
  });

  // Controlbar button toggles
  var buttonToggles = $('#flowplayer-styling input:checkbox');
  buttonToggles.change(function() {
    var params = {};
    buttonToggles.each(function(index, object) {
      params[this.value] = this.checked;
    });
    player.getControls().widgets(params);
  });
  buttonToggles.change(); // Update the player to reflect the settings.

  // Border radius
  $('#edit-flowplayer-border-radius').change(function() {
    player.getControls().css("borderRadius", $(this).val()); 
  });
  $('#edit-flowplayer-border-radius').change();

  // Background gradient
  $("#edit-flowplayer-background-gradient").change(function() {
    player.getControls().css("backgroundGradient", $(this).val());
  });
  $("#edit-flowplayer-background-gradient").change();

}
