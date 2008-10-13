/**
 * Imgzoom 1.2 (Requires the dimensions-plugin)
 *
 * Opens links that point to images in the "Imgzoom" (zooms out the image)
 *
 * Usage: jQuery.imgzoom();
 *
 * @class imgzoom
 * @param {Object} conf, custom config-object {speed: 500, dontFadeIn: 0, hideClicked: 0} // pelase note that i've removed the ability to fade in/out the zoomed image because it caused a bug in IE. If you don't care about IE uncomment the opacity-stuff on line ~50
 *
 * Copyright (c) 2008 Andreas Lagerkvist (andreaslagerkvist.com)
 * Released under a GNU General Public License v3 (http://creativecommons.org/licenses/by/3.0/)
 */
jQuery.imgzoom=function(B){var A=jQuery.extend({speed:200,dontFadeIn:1,hideClicked:1,loading:"Loading..."},B);A.doubleSpeed=A.speed/4;jQuery(document.body).click(function(I){var F=jQuery(I.target);var E=F.is("a")?F:F.parents("a");E=(E&&E.is("a")&&E.attr("href").search(/(.*)\.(jpg|jpeg|gif|png|bmp|tif|tiff)/gi)!=-1)?E:false;var G=(E&&E.find("img").length)?E.find("img"):false;if(E){E.oldText=E.text();E.setLoadingImg=function(){if(G){G.css({opacity:"0.5"})}else{E.text(A.loading)}};E.setNotLoadingImg=function(){if(G){G.css({opacity:"1"})}else{E.text(E.oldText)}};var C=E.attr("href");if(jQuery('div.imgzoom img[src="'+C+'"]').length){return false}var H=function(){E.setNotLoadingImg();var N=E.find("img").length;var T=N?E.find("img"):E;var P=N?A.hideClicked:0;var O=T.offset();var L={width:T.outerWidth(),height:T.outerHeight(),left:O.left,top:O.top};var M=jQuery('<div><img src="'+C+'" alt="" /></div>').css({position:"absolute"}).appendTo(document.body);var K={width:M.outerWidth(),height:M.outerHeight()};var R={width:jQuery(window).width(),height:jQuery(window).height()};if(K.width>R.width){var Q=R.width-100;K.height=(Q/K.width)*K.height;K.width=Q}if(K.height>R.height){var S=R.height-100;K.width=(S/K.height)*K.width;K.height=S}K.left=(R.width-K.width)/2+jQuery(window).scrollLeft();K.top=(R.height-K.height)/2+jQuery(window).scrollTop();var J=jQuery('<a href="#">Close</a>').appendTo(M).hide();if(P){E.css({visibility:"hidden"})}M.addClass("imgzoom").css(L).animate(K,A.speed,function(){J.fadeIn(A.doubleSpeed)});var U=function(){J.fadeOut(A.doubleSpeed,function(){M.animate(L,A.speed,function(){E.css({visibility:"visible"});M.remove()})});return false};M.click(U);J.click(U)};var D=new Image();D.src=C;if(D.complete){H()}else{E.setLoadingImg();D.onload=H}return false}})};
