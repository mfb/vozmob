  Drupal.behaviors.smsUser = function(context) {
    var $smstext = $('#edit-sms-text');
    var $keystrokes = $('#keystrokes span').eq(0).text('0 / ');
    $smstext.bind('keyup', function(e) {
      var chars = $smstext.val().length;
      $keystrokes.text(chars + ' / ');
    });
  };

