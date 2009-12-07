// $Id: flowplayer.js,v 1.1.2.6 2009/11/19 21:33:48 robloach Exp $

/**
 * @file
 * Provides the FlowPlayer Drupal behavior.
 */

/**
 * The FlowPlayer Drupal behavior that creates the set of FlowPlayer elements from Drupal.settings.flowplayer.
 */
Drupal.behaviors.flowplayer = function() {
  jQuery.each(Drupal.settings.flowplayer, function(selector, config) {

    // Convert any player object events to JavaScript calls.
    var playerEvents = [
      'onBeforeClick',
      'onLoad',
      'onBeforeLoad',
      'onUnload',
      'onBeforeUnload',
      'onMouseOver',
      'onMouseOut',
      'onKeypress',
      'onBeforeKeypress',
      'onVolume',
      'onBeforeVolume',
      'onMute',
      'onBeforeMute',
      'onUnmute',
      'onBeforeUnmute',
      'onFullscreen',
      'onBeforeFullscreen',
      'onFullscreenExit',
      'onPlaylistReplace',
      'onError'
    ];
    jQuery.each(playerEvents, function(index, event) {
      if (typeof(config[event]) == 'string') {
        config[event] = eval(config[event]);
      }
    });

    // Convert any clip object events to JavaScript events.
    var clipEvents = [
      'onBegin',
      'onBeforeBegin',
      'onMetaData',
      'onStart',
      'onPause',
      'onBeforePause',
      'onResume',
      'onBeforeResume',
      'onSeek',
      'onBeforeSeek',
      'onStop',
      'onBeforeStop',
      'onFinish',
      'onBeforeFinish',
      'onLastSecond',
      'onUpdate'
    ];
    if (config['clip']) {
      jQuery.each(clipEvents, function(index, event) {
        if (typeof(config['clip'][event]) == 'string') {
          config['clip'][event] = eval(config['clip'][event]);
        }
      });
    }

    // Register the playlist clip events.
    if (config['playlist']) {
      jQuery.each(config['playlist'], function(index, clip) {
        jQuery.each(clipEvents, function(e_index, event) {
          if (typeof(config['playlist'][index][event]) == 'string') {
            config['playlist'][index][event] = eval(config['playlist'][index][event]);
          }
        });
      });
    }

    // Register the onCuepoint callback.
    if (config['clip'] && config['clip']['onCuepoint'] && typeof(config['clip']['onCuepoint'][1]) == 'string') {
      config['clip']['onCuepoint'][1] = eval(config['clip']['onCuepoint'][1]);
    }

    // Create the flowplayer element on the non-processed elements.
    $(selector + ':not(.flowplayer-processed)').addClass('flowplayer-processed').flowplayer(Drupal.settings.basePath + Drupal.settings.flowplayerSwf, config);
  });
};
