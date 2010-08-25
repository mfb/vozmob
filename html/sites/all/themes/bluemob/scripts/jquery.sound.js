/*
Copyright (c) 2008 Dj Gilcrease

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

Author: Dj Gilcrease
Version: 1.0.1

*/
(function($){
    $.sound_count = 1;

    var methods = {
        sound: function sound(user_settings) {
            var self = $(this);

            var settings = {
                quality: 'high',
                events: {
                    play: function(evt) {
                        var self = $(this);
                        var movie = self.data("sound.get_movie")(self.data("sound.settings").id);
                        movie.play();
                        self.data("sound.isPlaying", true);
                    },
                    pause: function(evt) {
                        var self = $(this);
                        var movie = self.data("sound.get_movie")(self.data("sound.settings").id);
                        movie.pause();
                        self.data("sound.isPlaying", false);
                    },
                    stop: function(evt) {
                        var self = $(this);
                        var movie = self.data("sound.get_movie")(self.data("sound.settings").id);
                        movie.stop();
                        self.data("sound.isPlaying", false);
                    },
                    volume: function(evt, vol) {
                        if(vol > 100) {
                            vol = vol - 100;
                        }

                        var self = $(this);
                        var movie = self.data("sound.get_movie")(self.data("sound.settings").id);
                        movie.volume(vol);
                    },
                    load: function(evt, url) {
                        var self = $(this);
                        var id = self.data("sound.settings").id;
                        var movie = self.data("sound.get_movie")(id);
                        movie.load(url);
                        self.data("sound.isPlaying", true);
                    },
                    error: function(evt, err) {
                        alert(err.msg);
                    }
                }
            };

            if(self.data("sound.settings")) {
                settings = self.data("sound.settings");
            }

            settings = $.extend(user_settings, settings);

            if(!self.data("sound")) {
                settings.id = 'sound_player_' + $.sound_count;

                var html = '<object classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000"';
                html += ' codebase="http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab" width="0" height="0"';
                html += ' id="' + settings.id + '"';
                html += ' align="middle">';
                html += '<param name="movie" value="' + settings.swf + '" />';
                html += '<param name="quality" value="' + settings.quality + '" />';
                html += '<param name="FlashVars" value="id=' + settings.id + '"/>';
                html += '<param name="allowScriptAccess" value="always"/>';
                html += '<embed src="' + settings.swf + '" FlashVars="id='+ settings.id +'"';
                html += ' allowScriptAccess="always" quality="' + settings.quality + '" bgcolor="#ffffff" width="0" height="0"';
                html += ' name="' + settings.id + '" align="middle" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" />';

                html += '</object>';

                var get_movie = function(id) {
                    var movie = null;
                    if ($.browser.msie) {
                        movie =  window[id];
                    } else {
                        movie = document[id];
                    }

                    return movie;
                };
                self.data("sound.get_movie", get_movie);

                for(var event in settings.events) {
                    var evt = "sound." + event;
                    self.unbind(evt);
                    self.bind(evt, settings.events[event]);
                }

                self.html(html);

                self.data("sound", true);

                $.sound_count++;
            }

            self.data("sound.settings", settings);
            $(this).data("sound.isPlaying", false);

            if(settings.file) {
                var delayed = function() {
                    self.load(settings.file);
                };
                setTimeout(delayed, 250);
            }

            return self;
        },
        play: function() {
            var self = $(this);
            if(self.data("sound")) {
                self.trigger("sound.play");
            } else {
                settings.events.error(null, {msg: "You have not yet bound the sound player to this element"});
            }
        },
        pause: function() {
            var self = $(this);
            if(self.data("sound")) {
                self.trigger("sound.pause");
            } else {
                settings.events.error(null, {msg: "You have not yet bound the sound player to this element"});
            }
        },
        stop: function() {
            var self = $(this);
            if(self.data("sound")) {
                self.trigger("sound.stop");
            } else {
                settings.events.error(null, {msg: "You have not yet bound the sound player to this element"});
            }
        },
        volume: function(vol) {
            var self = $(this);
            if(self.data("sound")) {
                self.trigger("sound.volume", vol);
            } else {
                settings.events.error(null, {msg: "You have not yet bound the sound player to this element"});
            }
        },
        load: function(url) {
            var self = $(this);
            if(self.data("sound")) {
                self.trigger("sound.load", url);
            } else {
                settings.events.error(null, {msg: "You have not yet bound the sound player to this element"});
            }
        },
        isPlaying: function() {
            return $(this).data("sound.isPlaying");
        }
    };
    $.each(methods, function(i) {
        $.fn[i] = this;
    });
})(jQuery);