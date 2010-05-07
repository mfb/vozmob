if (Drupal.jsEnabled) {
  $(document).ready(function(){
  // only show for FF and IE
    if (window.sidebar || window.external.AddFavourite) {
      $("a.service_links_favorite").show();
      $("a.service_links_favorite").click(function(event){
        event.preventDefault();
        var url = unescape(this.href.replace(/\+/g,' '));
        var url = url.replace(/^[^\?]*\?/g, "");
        var title = url.replace(/^[^#]*#/g,"");
        url = url.replace(/#.*$/g, "");

        if (window.sidebar) {
          window.sidebar.addPanel(title, url,"");
        } else if ( window.external ) {
          window.external.AddFavorite( url, title);
        }
      });
    } else {
      $("a.service_links_favorite").hide();
    }
  });
}
