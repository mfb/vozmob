// $Id: views_bulk_operations.js,v 1.1.4.6 2008/12/10 05:51:24 kratib Exp $

Drupal.vbo = Drupal.vbo || {};

Drupal.vbo.selectAll = function() {
  var table = this;
  var form = jQuery(table).parents('form');

  var thSelectAll = jQuery('th.select-all', table).click(function() {
    cbSelectAll = jQuery('input.form-checkbox', thSelectAll)[0];
    setSelectAll(false);
    jQuery('input#vbo-select-all-pages', table).click(function() {
      setSelectAll(true);
    });
    jQuery('input#vbo-select-this-page', table).click(function() {
      setSelectAll(false);
    });
    jQuery('td input:checkbox', table).click(function() {
      setSelectAll($('input#edit-objects-select-all', form).attr('value') == 1);
    });
  });

  function setSelectAll(all) {
    cbSelectAll = jQuery('input.form-checkbox', thSelectAll)[0];
    if (cbSelectAll.checked) {
      tdSelectAll = jQuery('td.view-field-select-all', table).css('display', 'table-cell');
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

Drupal.vbo.ajaxViewResponse = function(target, response) {
  jQuery('form[id^=views-bulk-operations-form]').each(function() {
    jQuery(this).attr('action', Drupal.settings.basePath + Drupal.settings.vbo.url);
  });
  jQuery('form table th.select-all').parents('table').each(Drupal.vbo.selectAll);
}

if (Drupal.jsEnabled) {
  jQuery(document).ready(function() {
    jQuery('form table th.select-all').parents('table').each(Drupal.vbo.selectAll);
  })
}

