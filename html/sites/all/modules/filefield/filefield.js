// $Id: filefield.js,v 1.13 2009/01/31 17:56:22 dopry Exp $

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
}
