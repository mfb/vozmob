/*
Copyright (c) <year> <copyright holders>

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

Compile with mtasc:
mtasc -main Player.as -swf Player.swf -header 450:325:20 -v -version 8 -group

*/

import flash.external.ExternalInterface;

class Player
{
    static var app:Player;
    var sound:Sound;
    var last_position:Number;

    function Player() {
        Player.trace("Player created");

        this.sound = new Sound();
        this.last_position = 0;

        ExternalInterface.addCallback("play", this, play);
        ExternalInterface.addCallback("pause", this, pause);
        ExternalInterface.addCallback("stop", this, stop);
        ExternalInterface.addCallback("load", this, load);
        ExternalInterface.addCallback("volume", this, volume);
    }

    function play() {
        this.sound.start(this.last_position);
    }

    function pause() {
        this.last_position = this.sound.position / 1000;
        this.sound.stop();
    }

    function stop() {
        this.last_position = 0;
        this.sound.stop();
    }

    function load(url:String) {
        this.last_position = 0;
        this.sound.loadSound(url, true);
    }

    function volume(vol:Number) {
        this.sound.setVolume(vol);
    }

    static function trace(value:String) {
    }

    static function main(mc:MovieClip) {
      app = new Player();
    }
}