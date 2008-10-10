<?php
// $Id: content_plugin_display_simple.inc,v 1.1.2.2 2008/10/04 13:14:21 karens Exp $

/**
 * @file
 * Handler for 'content_simple' display.
 */
class content_plugin_display_simple extends views_plugin_display {
  function execute() {
    return $this->view->render($this->display->id);
  }

  function render() {
    return !empty($this->view->result) || !empty($this->view->style_plugin->definition['even empty']) ? $this->view->style_plugin->render($this->view->result) : '';
  }
}