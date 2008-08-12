$(document).ready(function() {
	$('input#edit-sms-muse-validate').parent().append('<div id="muse-throbber"></div>');
	$('input#edit-sms-muse-validate').click(function() {
		
		var soap = $("input#edit-sms-muse-soap").val();
		var username = $("input#edit-sms-muse-user").val();
		var pass = $("input#edit-sms-muse-password").val();
		var service = $("input#edit-sms-muse-service").val();
		
		$('#muse-throbber').removeClass().addClass('active');
		$('#muse-throbber').html('Validating...');
		
		$.get("/sms/in/auth", { soap: soap, username: username, password: pass, service: service }, function(data) {
			var result = Drupal.parseJson(data);
			
			//$('input#edit-sms-muse-validate').parent().('<span id="error-message"></span>');
			
			if(result['authenticated'] == true) {
				$('#muse-throbber').removeClass().addClass('validate');
				$('#muse-throbber').html('Your credentials validated successfully.');
			}
			else {
				$('#muse-throbber').removeClass().addClass('fail');
				$('#muse-throbber').html(result['error'] +'.');
			}
		});
		return false;
	});
});