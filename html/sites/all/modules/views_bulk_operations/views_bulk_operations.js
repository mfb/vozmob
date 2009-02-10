// $Id: views_bulk_operations.js,v 1.1.4.10 2009/01/29 00:29:03 kratib Exp $

Drupal.vbo = Drupal.vbo || {};

Drupal.vbo.selectAll = function() {
  var table = this;
  var form = jQuery(table).parents('form');

  var thSelectAll = jQuery('th.select-all', table).click(function() {
    setSelectAll(false);
  });
  jQuery('input#vbo-select-all-pages', table).click(function() {
    setSelectAll(true);
  });
  jQuery('input#vbo-select-this-page', table).click(function() {
    setSelectAll(false);
  });
  jQuery('td input:checkbox', table).click(function() {
    setSelectAll(jQuery('input#edit-objects-select-all', form).attr('value') == 1);
  });

  function setSelectAll(all) {
    cbSelectAll = jQuery('input.form-checkbox', thSelectAll)[0];
    if (cbSelectAll.checked) {
      tdSelectAll = jQuery('td.view-field-select-all', table).css('display', jQuery.browser.msie ? 'inline-block' : 'table-cell');
      if (all) {
        jQuery('span#vbo-this-page', tdSelectAll).css('display', 'none');
        jQuery('span#vbo-all-pages', tdSelectAll).css('display', 'inline');
        jQuery('input#edit-objects-select-all', form).attr('value', 1);
      }
      else {
        jQuery('span#vbo-this-page', tdSelectAll).css('display', 'inline');
        jQuery('span#vbo-all-pages', tdSelectAll).css('display', 'none');
        jQuery('input#edit-objects-select-all', form).attr('value', 0);
      }
    }
    else {
      jQuery('td.view-field-select-all', table).css('display', 'none');
      jQuery('input#edit-objects-select-all', form).attr('value', all ? 1 : 0);
    }
  }
}

Drupal.vbo.startUp = function(context) {
  jQuery('form table th.select-all', context).parents('table').each(Drupal.vbo.selectAll);
  jQuery('tr.rowclick', context).click(function(event) {
    if (event.target.type !== 'checkbox') {
      checkbox = jQuery(':checkbox', this)[0];
      checked = checkbox.checked;
      // trigger() toggles the checkmark *after* the event is set, 
      // whereas manually clicking the checkbox toggles it *beforehand*.
      // that's why we manually set the checkmark first, then trigger the
      // event (so that listeners get notified), then re-set the checkmark
      // which the trigger will have toggled. yuck!
      checkbox.checked = !checked;
      jQuery(checkbox).trigger('click');
      checkbox.checked = !checked;
    }
  });
}

Drupal.behaviors.vbo = function(context) {
  jQuery('form[id^=views-bulk-operations-form]').each(function() {
    jQuery(this).attr('action', Drupal.settings.basePath + Drupal.settings.vbo.url);
  });
  Drupal.vbo.startUp(context);
}

