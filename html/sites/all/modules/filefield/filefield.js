// $Id: filefield.js,v 1.16 2009/03/09 05:07:35 quicksketch Exp $

/**
 * Auto-attach standard client side file input validation.
 */
Drupal.behaviors.filefieldValidateAutoAttach = function(context) {
  $("input[type='file'][accept]", context).change( function() {
    // Remove any previous errors.
    $('.file-upload-js-error').remove();

    /**
     * Add client side validation for the input[type=file] accept attribute.
     */
    var accept = this.accept.replace(/,\s*/g, '|');
    if (accept.length > 1) {
      var v = new RegExp('\\.(' + accept + ')$', 'gi');
      if (!v.test(this.value)) {
        var error = Drupal.t("The selected file %filename cannot not be uploaded. Only files with the following extensions are allowed: %extensions.",
          { '%filename' : this.value, '%extensions' : accept.replace(/\|/g, ', ') }
        );
        // What do I prepend this to?
        $(this).before('<div class="messages error file-upload-js-error">' + error + '</div>');
        this.value = '';
        return false;
      }
    }

    /**
     * Add filesize validation where possible.
     */
    /* @todo */
  });
};


/**
 * Prevent FileField uploads when using buttons not intended to upload.
 */
Drupal.behaviors.filefieldButtons = function(context) {
  $('input.form-submit').bind('mousedown', Drupal.filefield.disableFields);
};

/**
 * Admin enhancement: only show the "Files listed by default" when needed.
 */
Drupal.behaviors.filefieldAdmin = function(context) {
  var $listField = $('div.filefield-list-field', context);
  if ($listField.size()) {
    $listField.find('input').change(function() {
      if (this.checked) {
        if (this.value == 0) {
          $('#edit-list-default-wrapper').css('display', 'none');
        }
        else {
          $('#edit-list-default-wrapper').css('display', 'block');
        }
      }
    }).change();
  }
};

/**
 * Utility functions for use by FileField.
 * @param {Object} event
 */
Drupal.filefield = {
  disableFields: function(event){
    var clickedButton = this;

    // Only disable upload fields for AHAH buttons.
    if (!$(clickedButton).hasClass('ahah-processed')) {
      return;
    }

    // Using the grandparent, we ensure that we get up to at least the level
    // of the CCK multiple field wrapper.
    var $enabledFields = $(this).parent().parent().find('input.form-file');
    var $disabledFields = $('div.filefield-element input.form-file').not($enabledFields);

    // Disable upload fields other than the one we're currently working with.
    $disabledFields.attr('disabled', 'disabled');

    // All the other mousedown handlers (like AHAH) are excuted before any
    // timeout functions will be called, so this effectively re-enables
    // the filefields after the AHAH process is complete even though it only
    // has a 1 millisecond timeout.
    setTimeout(function(){
      $disabledFields.attr('disabled', '');
    }, 1000);
  }
};
