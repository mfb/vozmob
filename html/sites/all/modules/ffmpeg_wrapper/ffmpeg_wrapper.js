// $Id: ffmpeg_wrapper.js,v 1.1.2.3 2008/08/04 21:01:27 arthuregg Exp $;

/**
 * @param string $prefix is the prefix of the elements
 * @param string $source is the source for the output type
 */
function ffmpeg_wrapper_update_options(prefix, source) {

  // get the path to the output settings set in ffmpeg_wrapper module
  var path = Drupal.settings.ffmpeg_wrapper.ffmpeg_wrapper_output_url;
  
  // get the output type
  var output = $('#'+prefix+source).val();
  
  // only look for the value if there is one selected
  if (output != 0) {
    // now get the values for this codec
    $.getJSON(path+output, function(json) {
    
      // now we need to itterate through each of the keys returned and 
      // limit the choices to the incoming values.
      var data = eval (json);
      
      // first break the configuration into the types of config data we have
      for (var type in data) {
        // catch the flag for default settings and don't do 
        // any form updating for this value
        if (data[type] != 'default') {
          // now get each of the items in the config
          // types here are 'audio' and 'video' 
          for (var key in data[type] ) {
            // build the element from the prefix value:  ffmpeg & the kind of item it is & the key 
            var element = '#'+prefix+'ffmpeg-'+type+'-'+key;
            // make sure element exists
            if ($(element)) {
              // remove existing html from this select box
              $(element).html('');
              var html = '';
             
              // break each of the options out for each select box
              for (var option in data[type][key]) {
              
                // create the description for this element 
                var description = data[type][key][option].valueOf();              
                                
                // now we check to see if the option coming in contains
                // the default value for this configuration so we know to use it as default
                // make a new variable to properly type cast the value
                var test = ''+data[type][key][option]+'';
           
                // search this string for the "default" text and save it for 
                // use after the select is rebuilt
                if (test.match(Drupal.settings.ffmpeg_wrapper.default_string) ) { 
                  var selected = option;                 
                }
                
                // now build the html for the select options
                html += ffmpeg_wrapper_select_builder(option, description);
              }
               
              // build the new select element 
              $(element).html(html);
               
              // now update the selected value
              if (selected) {
                $(element).val(selected);
                selected = null;
              }
               
            }
          }
          // now turn on the advanced options so they are picked up
          $('#'+prefix+'ffmpeg-'+type+'-advanced').attr('checked', true);
        }            
        else {
          // is the configuration data being passed back was the default data?
          // turn off the advanced configuration so that nothing nasty 
          // happens without user action
          $('#'+prefix+'ffmpeg-audio-advanced').attr('checked', false);
          $('#'+prefix+'ffmpeg-video-advanced').attr('checked', false);        
        }
      }
    });
  }
}

/**
 * this builds the html for the output of the selects
 * simple function but makes code easier
 * @param string $value is the key value
 * @param string $description is the text value
 */
 function ffmpeg_wrapper_select_builder(value, description) {
   if (selected) { var selected = 'selected'; }
   else { var selected = ''; }   
   return '<option value="'+value+'">'+description+'</option>';
 }