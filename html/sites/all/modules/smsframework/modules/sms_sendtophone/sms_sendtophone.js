$(document).ready(function() {
  $("a.sms-sendtophone").addClass('thickbox').each(function() {
    if ($(this).href().search(/\?/) < 0) {
      $(this).href($(this).href().concat('?'));
    }
    else {
      $(this).href($(this).href().concat('&'));
    }
    $(this).href($(this).href().concat('thickbox=1&height=275&width=300'));
  })
});