if (Drupal.jsEnabled) {
  $(document).ready(
		function()
		{
			$('#fisheye').Fisheye(
				{
					maxWidth: 32,
					items: 'a',
					itemsText: 'span',
					container: '.fisheyeContainer',
					itemWidth: 16,
					proximity: 60,
					halign : 'center'
				}
			)
    }
  );
}

