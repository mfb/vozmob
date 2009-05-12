// $Id: thickbox_login.js,v 1.2.2.4 2009/04/18 21:32:36 frjo Exp $
// Contributed by user jmiccolis.
Drupal.behaviors.initThickboxLogin = function(context) {
  $("a[href*='/user/login']").addClass('thickbox').each(function() { this.href = this.href.replace(/user\/login(%3F|\?)?/,"user/login/thickbox?height=230&width=250&") });
  $("a[href*='?q=user/login']").addClass('thickbox').each(function() { this.href = this.href.replace(/user\/login/,"user/login/thickbox&height=230&width=250") });
}