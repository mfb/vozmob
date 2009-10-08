// $Id$

Drupal.behaviors.imagefieldRotate = function(context) {

  // Hide rotation select, set a single option for the select, add rotate buttons.
  $('select.imagefield-rotate:not(.processed)').addClass('processed').each(function(i) {
    $(this).after('<a class="imagefield-rotate right" href="#">' + Drupal.t('Right') + '</a>').nextAll('a.imagefield-rotate.right').click(function(e) {
      rotateImg(this, 90);
      setForm(this, 90);
      return false;
    });
    $(this).after('<a class="imagefield-rotate left" href="#">' + Drupal.t('Left') + '</a> ').nextAll('a.imagefield-rotate.left').click(function(e) {
      rotateImg(this, -90);
      setForm(this, -90);
      return false;
    });
    $(this).hide().empty().append('<option selected="selected" value="0" />');
  });

  // Perform the rotation.
  function rotateImg(button, angle) {
    var previewDiv = $(button).parent().parent().siblings('.widget-preview'); 
    // If a rotation has already occurred we'll have a canvas instead of an img element.
    if ($(previewDiv).find('img').size()) {
       $(previewDiv).find('img').rotateRight(angle);
    } else {
      $(previewDiv).find('canvas').rotateRight(angle);
    }
  }
  
  // Alter the form value.
  function setForm(button, angle) {
    var rotateOption = $(button).parent().find('select.imagefield-rotate option');
    var newVal = parseInt($(rotateOption).val()) + angle;
    // Need to keep the value matching an original form value to validate in FAPI:
    // 0, 90, 180, 270.
    newVal = newVal % 360;
    if (newVal < 0) {
      newVal = newVal + 360;
    }
    $(rotateOption).val(newVal);
  }
  
}
