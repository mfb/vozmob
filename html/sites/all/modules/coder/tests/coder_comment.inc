<?php
/* $Id .... $ */
// $Id ....$
// $Id: coder_comment.inc,v 1.1.4.6 2008/05/13 15:58:36 snpower Exp $
// $Id: coder_comment.inc,v 1.1.4.6 2008/05/13 15:58:36 snpower Exp $ ok
/**
 * @file
 * This file implements tests for the Drupal commenting standards.
 *
 * It is never actually called by the coder code, but is read when running
 * admin/coder/coder
 */

/**
 * @file this is not ok.
 */

/**
* This is a comment error.
 *This is a comment error.
 */

/*
 * @see some_function()
 * @see some_function(), some-template.tpl.php
 * @see some_function(), some-template.tpl.php, ok.tpl.php
 *
 * sdfsf @see not ok
 * @see some_function, some-template.tpl.php, not_ok.tpl.php
 * @see some_function() some-template.tpl.php, not_ok.tpl.php
 * @see some_function(), some-template.tpl.php not_ok.tpl.php
 * @see some_function(), some-template.tpl.php, not_ok.tpl.php foo.php
 * @see some_function(),
 * @see some_function(), some-template.tpl.php, not_ok.tpl.php.
 */

/*
 * Implementation of hook_foo().
 * Implementation of hook_foo()
 * implementation of hook_foo().
 * Implementation of hook_foo
 * Implementation of hook_foo.
 * Implemetation of hook_foo().
 */
// Implementation of hook_foo().
/* Implementation of hook_foo(). */
