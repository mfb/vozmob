// various JavaScript object.
var Blueprint = {};

// prevent users from clicking a submit button twice
Drupal.behaviors.blueprint = function() {
  
  // only apply this to node and comment and new user registration forms
  var forms = $("#node-form>div>div>#edit-submit,#comment-form>div>#edit-submit,#user-register>div>#edit-submit");

  // insert the saving div now to cache it for better performance and to show the loading image
  $('<div id="saving"><p class="saving">Saving&hellip;</p></div>').insertAfter(forms);

  forms.click(function() {
    // When the user hits enter to select the term in the autocomplete field do nothing
    if ($('#autocomplete').each(function () { this.owner.hidePopup();}).size() != 0) {
      return;
    }
    $(this).siblings("input:submit").hide();
    $(this).hide();
    $("#saving").show();

    var notice = function() {
      $('<p id="saving-notice">Not saving? Wait a few seconds, reload this page, and try again. Every now and then the internet hiccups too :-)</p>').appendTo("#saving").fadeIn();
    };

    // append notice if form saving isn't work, perhaps a timeout issue
    setTimeout(notice, 24000);
  });
};

