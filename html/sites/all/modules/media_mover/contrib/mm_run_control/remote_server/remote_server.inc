<?php

// $Id;

/**
 * @file
 * This file contains the configuration variables for running a remote
 * server that integratese with Media Mover's Run Control module.
 */

// Server URL, includes base path if needed. No trailing URL
$conf['server_url'] = "http://localhost/mm_6-2";

// Secret key for encryption. Note this much match the server's key
$conf['secret_key'] = "for goodness sake, please change this!";

// Server Id - unique server id. This is a string, but it set to null
// by default to use this server's IP address. You SHOULD fill this out
$conf['server_id'] = null;

// Media Mover modules that this script works with
$conf['modules'] =  array('mm_ffmpeg');

// Specific actions that this module works with (defaults to any action that this module has)
$conf['actions'] = array();