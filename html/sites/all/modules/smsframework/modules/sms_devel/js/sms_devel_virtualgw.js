// $Id: sms_devel_virtualgw.js,v 1.1.4.2 2010/11/18 11:05:46 univate Exp $
/**
 * @file
 *
 * Handles AJAX submission and response in SMS Framework: Devel virtual gateway
 */


sms_devel.virtualgw.getactivity = function() {
  var url = location.protocol +"//"+ location.host + Drupal.settings.basePath +"admin/smsframework/devel/virtualgw/getactivity";
  $.ajax({
    url: url,
    dataType: 'json',
    success: function () {
      // Check was successful.
      $("#clean-url input.form-radio").attr("disabled", false);
      $("#clean-url input.form-radio").attr("checked", 1);
      $("#clean-url .description span").append('<div class="ok">'+ Drupal.settings.cleanURL.success +"</div>");
      $("#testing").hide();
    },
    error: function() {
      // Check failed.
      $("#clean-url .description span").append('<div class="warning">'+ Drupal.settings.cleanURL.failure +"</div>");
      $("#testing").hide();
    }
  });
  $("#clean-url").addClass('clean-url-processed');
};

this.timer = setTimeout(sms_devel.virtualgw.getactivity, 1000);

//if (this.timer) {
//  clearTimeout(this.timer);
//}
