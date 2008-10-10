// $Id: filefield.js,v 1.9 2008/08/08 17:37:48 drewish Exp $

/**
 * Auto-attach standard client side file input validation.
 */
Drupal.behaviors.filefieldValidateAutoAttach = function(context) {
  $("input[@type='file']", context).change( function() {
    $('.filefield-js-error').remove();

    /**
     * Add client side validation for the input[@file] accept attribute.
     */
    if(this.accept.length > 1){
      accept = this.accept.replace(/,/g, '|');
      v = new RegExp('\\.(' + (accept ? accept : '') + ')$', 'gi');
      if (!v.test(this.value)) {
        var error = Drupal.t("The selected file %filename cannot not be uploaded. Only files with the following extensions are allowed: %extensions.",
          { '%filename' : this.value, '%extensions': accept.replace(/\|/g, ', ') }
        );
        // what do I prepend this to?
        $(this).before('<div class="messages error filefield-js-error">' + error + '</div>');
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
