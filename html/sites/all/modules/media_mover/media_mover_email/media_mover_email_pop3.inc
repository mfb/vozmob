<?php


/**
 * Copyright (c) 1999-2006 The SquirrelMail Project Team
 *
 * Copyright (c) 1999 CDI (cdi@thewebmasters.net) All Rights Reserved
 * Modified by Philippe Mingo 2001 mingo@rotedic.com
 * An RFC 1939 compliant wrapper class for the POP3 protocol.
 *
 * Licensed under the GNU GPL. For full terms see the file COPYING.
 *
 * pop3 class
 *
 * $Id: media_mover_email_pop3.inc,v 1.5 2008/02/07 20:41:50 bdragon Exp $
 */

class POP3 {
  var $ERROR = ''; //  Error string.

  var $TIMEOUT = 600; //  Default timeout before giving up on a
  //  network operation.

  var $COUNT = -1; //  Mailbox msg count

  var $BUFFER = 512; //  Socket buffer for socket fgets() calls.
  //  Per RFC 1939 the returned line a POP3
  //  server can send is 512 bytes.

  var $FP = ''; //  The connection to the server's
  //  file descriptor

  var $MAILSERVER = ''; // Set this to hard code the server name

  var $DEBUG = FALSE; // set to true to echo pop3
  // commands and responses to error_log
  // this WILL log passwords!

  var $BANNER = ''; //  Holds the banner returned by the
  //  pop server - used for apop()

  var $ALLOWAPOP = FALSE; //  Allow or disallow apop()
  //  This must be set to true
  //  manually

  function POP3($server = '', $timeout = '') {
    settype($this->BUFFER, "integer");
    if (!empty ($server)) {
      // Do not allow programs to alter MAILSERVER
      // if it is already specified. They can get around
      // this if they -really- want to, so don't count on it.
      if (empty ($this->MAILSERVER))
        $this->MAILSERVER = $server;
    }
    if (!empty ($timeout)) {
      settype($timeout, "integer");
      $this->TIMEOUT = $timeout;
      if (!ini_get('safe_mode'))
        set_time_limit($timeout);
    }
    return true;
  }

  function update_timer() {
    if (!ini_get('safe_mode'))
      set_time_limit($this->TIMEOUT);
    return true;
  }

  function connect($server, $port = 110) {
    //  Opens a socket to the specified server. Unless overridden,
    //  port defaults to 110. Returns true on success, false on fail

    // If MAILSERVER is set, override $server with it's value

    if (!isset ($port) || !$port) {
      $port = 110;
    }
    if (!empty ($this->MAILSERVER))
      $server = $this->MAILSERVER;

    if (empty ($server)) {
      $this->ERROR = t("POP3 connect:") .' '. t("No server specified");
      unset($this->FP);
      return false;
    }

    $fp = @ fsockopen("$server", $port, $errno, $errstr);

    if (!$fp) {
      $this->ERROR = t("POP3 connect:") .' '. t("Error ") ."[$errno] [$errstr]";
      unset($this->FP);
      return false;
    }

    socket_set_blocking($fp, -1);
    $this->update_timer();
    $reply = fgets($fp, $this->BUFFER);
    $reply = $this->strip_clf($reply);
    if ($this->DEBUG)
      error_log("POP3 SEND [connect: $server] GOT [$reply]", 0);
    if (!$this->is_ok($reply)) {
      $this->ERROR = t("POP3 connect:") .' '. t("Error ") ."[$reply]";
      unset($this->FP);
      return false;
    }
    $this->FP = $fp;
    $this->BANNER = $this->parse_banner($reply);
    return true;
  }

  function user($user = "") {
    // Sends the USER command, returns true or false

    if (empty ($user)) {
      $this->ERROR = t("POP3 user:") .' '. t("no login ID submitted");
      return false;
    }
    elseif (!isset ($this->FP)) {
      $this->ERROR = t("POP3 user:") .' '. t("connection not established");
      return false;
    }
    else {
      $reply = $this->send_cmd("USER $user");
      if (!$this->is_ok($reply)) {
        $this->ERROR = t("POP3 user:") .' '. t("Error ") ."[$reply]";
        return false;
      }
      else {
        return true;
      }
    }
  }

  function pass($pass = "") {
    // Sends the PASS command, returns # of msgs in mailbox,
    // returns false (undef) on Auth failure

    if (empty ($pass)) {
      $this->ERROR = t("POP3 pass:") .' '. t("No password submitted");
      return false;
    }
    elseif (! isset ($this->FP)) {
      $this->ERROR = t("POP3 pass:") .' '. t("connection not established");
      return false;
    }
    else {
      $reply = $this->send_cmd("PASS $pass");
      if (! $this->is_ok($reply)) {
        $this->ERROR = t("POP3 pass:") .' '. t("Authentication failed") ." [$reply]";
        $this->quit();
        return false;
      }
      else {
        //  Auth successful.
        $count = $this->last("count");
        $this->COUNT = $count;
        return $count;
      }
    }
  }

  function apop($login, $pass) {
    //  Attempts an APOP login. If this fails, it'll
    //  try a standard login. YOUR SERVER MUST SUPPORT
    //  THE USE OF THE APOP COMMAND!
    //  (apop is optional per rfc1939)

    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 apop:") .' '. t("No connection to server");
      return false;
    }
    elseif (!$this->ALLOWAPOP) {
      $retval = $this->login($login, $pass);
      return $retval;
    }
    elseif (empty ($login)) {
      $this->ERROR = t("POP3 apop:") .' '. t("No login ID submitted");
      return false;
    }
    elseif (empty ($pass)) {
      $this->ERROR = t("POP3 apop:") .' '. t("No password submitted");
      return false;
    }
    else {
      $banner = $this->BANNER;
      if ((!$banner) or (empty ($banner))) {
        $this->ERROR = t("POP3 apop:") .' '. t("No server banner") .' - '. t("abort");
        $retval = $this->login($login, $pass);
        return $retval;
      }
      else {
        $auth_string = $banner;
        $auth_string .= $pass;
        $apop_string = md5($auth_string);
        $cmd = "APOP $login $apop_string";
        $reply = $this->send_cmd($cmd);
        if (!$this->is_ok($reply)) {
          $this->ERROR = t("POP3 apop:") .' '. t("apop authentication failed") .' - '. t("abort");
          $retval = $this->login($login, $pass);
          return $retval;
        }
        else {
          //  Auth successful.
          $count = $this->last("count");
          $this->COUNT = $count;
          return $count;
        }
      }
    }
  }

  function login($login = "", $pass = "") {
    // Sends both user and pass. Returns # of msgs in mailbox or
    // false on failure (or -1, if the error occurs while getting
    // the number of messages.)

    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 login:") .' '. t("No connection to server");
      return false;
    }
    else {
      $fp = $this->FP;
      if (!$this->user($login)) {
        //  Preserve the error generated by user()
        return false;
      }
      else {
        $count = $this->pass($pass);
        if ((!$count) || ($count == -1)) {
          //  Preserve the error generated by last() and pass()
          return false;
        }
        else {
          return $count;
        }
      }
    }
  }

  function top($msgnum, $numlines = "0") {
    //  Gets the header and first $numLines of the msg body
    //  returns data in an array with each returned line being
    //  an array element. If $numLines is empty, returns
    //  only the header information, and none of the body.

    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 top:") .' '. t("No connection to server");
      return false;
    }
    $this->update_timer();

    $fp = $this->FP;
    $buffer = $this->BUFFER;
    $cmd = "TOP $msgnum $numlines";
    fwrite($fp, "TOP $msgnum $numlines\r\n");
    $reply = fgets($fp, $buffer);
    $reply = $this->strip_clf($reply);
    if ($this->DEBUG) {
      @ error_log("POP3 SEND [$cmd] GOT [$reply]", 0);
    }
    if (!$this->is_ok($reply)) {
      $this->ERROR = t("POP3 top:") .' '. t("Error ") ."[$reply]";
      return false;
    }

    $count = 0;
    $msgarray = array();

    $line = fgets($fp, $buffer);
    while (!ereg("^\.\r\n", $line)) {
      $msgarray[$count] = $line;
      $count++;
      $line = fgets($fp, $buffer);
      if (empty ($line)) {
        break;
      }
    }

    return $msgarray;
  }

  function pop_list($msgnum = "") {
    //  If called with an argument, returns that msgs' size in octets
    //  No argument returns an associative array of undeleted
    //  msg numbers and their sizes in octets

    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 pop_list:") .' '. t("No connection to server");
      return false;
    }
    $fp = $this->FP;
    $total = $this->COUNT;
    if ((!$total) or ($total == -1)) {
      return false;
    }
    if ($total == 0) {
      return array(
        "0",
        "0"
      );
      // return -1;   // mailbox empty
    }

    $this->update_timer();

    if (!empty ($msgnum)) {
      $cmd = "LIST $msgnum";
      fwrite($fp, "$cmd\r\n");
      $reply = fgets($fp, $this->BUFFER);
      $reply = $this->strip_clf($reply);
      if ($this->DEBUG) {
        @ error_log("POP3 SEND [$cmd] GOT [$reply]", 0);
      }
      if (!$this->is_ok($reply)) {
        $this->ERROR = t("POP3 pop_list:") .' '. t("Error ") ."[$reply]";
        return false;
      }
      list ($junk, $num, $size) = preg_split('/\s+/', $reply);
      return $size;
    }
    $cmd = "LIST";
    $reply = $this->send_cmd($cmd);
    if (!$this->is_ok($reply)) {
      $reply = $this->strip_clf($reply);
      $this->ERROR = t("POP3 pop_list:") .' '. t("Error ") ."[$reply]";
      return false;
    }
    $msgarray = array();
    $msgarray[0] = $total;
    for ($msgc = 1; $msgc <= $total; $msgc++) {
      if ($msgc > $total) {
        break;
      }
      $line = fgets($fp, $this->BUFFER);
      $line = $this->strip_clf($line);
      if (ereg("^\.", $line)) {
        $this->ERROR = t("POP3 pop_list:") .' '. t("Premature end of list");
        return false;
      }
      list ($thismsg, $msgsize) = preg_split('/\s+/', $line);
      settype($thismsg, "integer");
      if ($thismsg != $msgc) {
        $msgarray[$msgc] = "deleted";
      }
      else {
        $msgarray[$msgc] = $msgsize;
      }
    }
    return $msgarray;
  }

  // @@@ This can run you out of ram on a large message.
  function get($msgnum) {
    //  Retrieve the specified msg number. Returns an array
    //  where each line of the msg is an array element.

    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 get:") .' '. t("No connection to server");
      return false;
    }

    $this->update_timer();

    $fp = $this->FP;
    $buffer = $this->BUFFER;
    $cmd = "RETR $msgnum";
    $reply = $this->send_cmd($cmd);

    if (!$this->is_ok($reply)) {
      $this->ERROR = t("POP3 get:") .' '. t("Error ") ."[$reply]";
      return false;
    }

    $count = 0;
    $msgarray = array();

    $line = fgets($fp, $buffer);
    while (!ereg("^\.\r\n", $line)) {
      $msgarray[$count] = $line;
      $count++;
      $line = fgets($fp, $buffer);
      if (empty ($line)) {
        break;
      }
    }
    return $msgarray;
  }

  // @@@ This can run you out of ram on a large message.
  function getstring($msgnum) {
    //  Retrieve the specified msg number. Returns a string containing the message.

    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 get:") .' '. t("No connection to server");
      return false;
    }

    $this->update_timer();

    $fp = $this->FP;
    $buffer = $this->BUFFER;
    $cmd = "RETR $msgnum";
    $reply = $this->send_cmd($cmd);

    if (!$this->is_ok($reply)) {
      $this->ERROR = t("POP3 get:") .' '. t("Error ") ."[$reply]";
      return false;
    }

    $message = '';

    $line = fgets($fp, $buffer);
    while (!ereg("^\.\r\n", $line)) {
      $message .= $line;
      $line = fgets($fp, $buffer);
      if (empty ($line)) {
        break;
      }
    }
    return $message;
  }

  // Added by Bdragon, Feb 6 2008
  // get to a file.
  function getfile($filename, $msgnum) {
    //  Retrieve the specified msg number. Returns an array
    //  where each line of the msg is an array element.

    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 getfile:") .' '. t("No connection to server");
      return false;
    }

    $this->update_timer();

    $outfp = fopen($filename, 'wb');
    if ($outfp === FALSE) {
      $this->ERROR = t('POP3 getfile:') .' '. t("Error: Couldn't open temp file!");
      return false;
    }

    $fp = $this->FP;
    $buffer = $this->BUFFER;
    $cmd = "RETR $msgnum";
    $reply = $this->send_cmd($cmd);

    if (!$this->is_ok($reply)) {
      $this->ERROR = t("POP3 getfile:") .' '. t("Error ") ."[$reply]";
      return false;
    }

    $line = fgets($fp, $buffer);
    while (!ereg("^\.\r\n", $line)) {
      // @@@ We need to implement a way to un-byte-stuff .\r\n!
      fwrite($outfp, $line);
      $line = fgets($fp, $buffer);
      if (empty ($line)) {
        break;
      }
    }
    fclose($outfp);
    return TRUE;
  }



  function last($type = "count") {
    //  Returns the highest msg number in the mailbox.
    //  returns -1 on error, 0+ on success, if type != count
    //  results in a popstat() call (2 element array returned)

    $last = -1;
    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 last:") .' '. t("No connection to server");
      return $last;
    }

    $reply = $this->send_cmd("STAT");
    if (!$this->is_ok($reply)) {
      $this->ERROR = t("POP3 last:") .' '. t("Error ") ."[$reply]";
      return $last;
    }

    $vars = preg_split('/\s+/', $reply);
    $count = $vars[1];
    $size = $vars[2];
    settype($count, "integer");
    settype($size, "integer");
    if ($type != "count") {
      return array(
        $count,
        $size
      );
    }
    return $count;
  }

  function reset() {
    //  Resets the status of the remote server. This includes
    //  resetting the status of ALL msgs to not be deleted.
    //  This method automatically closes the connection to the server.

    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 reset:") .' '. t("No connection to server");
      return false;
    }
    $reply = $this->send_cmd("RSET");
    if (!$this->is_ok($reply)) {
      //  The POP3 RSET command -never- gives a -ERR
      //  response - if it ever does, something truely
      //  wild is going on.

      $this->ERROR = t("POP3 reset:") .' '. t("Error ") ."[$reply]";
      @ error_log("POP3 reset: ERROR [$reply]", 0);
    }
    $this->quit();
    return true;
  }

  function send_cmd($cmd = "") {
    //  Sends a user defined command string to the
    //  POP server and returns the results. Useful for
    //  non-compliant or custom POP servers.
    //  Do NOT includ the \r\n as part of your command
    //  string - it will be appended automatically.

    //  The return value is a standard fgets() call, which
    //  will read up to $this->BUFFER bytes of data, until it
    //  encounters a new line, or EOF, whichever happens first.

    //  This method works best if $cmd responds with only
    //  one line of data.

    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 send_cmd:") .' '. t("No connection to server");
      return false;
    }

    if (empty ($cmd)) {
      $this->ERROR = t("POP3 send_cmd:") .' '. t("Empty command string");
      return "";
    }

    $fp = $this->FP;
    $buffer = $this->BUFFER;
    $this->update_timer();
    fwrite($fp, "$cmd\r\n");
    $reply = fgets($fp, $buffer);
    $reply = $this->strip_clf($reply);
    if ($this->DEBUG) {
      @ error_log("POP3 SEND [$cmd] GOT [$reply]", 0);
    }
    return $reply;
  }

  function quit() {
    //  Closes the connection to the POP3 server, deleting
    //  any msgs marked as deleted.

    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 quit:") .' '. t("connection does not exist");
      return false;
    }
    $fp = $this->FP;
    $cmd = "QUIT";
    fwrite($fp, "$cmd\r\n");
    $reply = fgets($fp, $this->BUFFER);
    $reply = $this->strip_clf($reply);
    if ($this->DEBUG) {
      @ error_log("POP3 SEND [$cmd] GOT [$reply]", 0);
    }
    fclose($fp);
    unset($this->FP);
    return true;
  }

  function popstat() {
    //  Returns an array of 2 elements. The number of undeleted
    //  msgs in the mailbox, and the size of the mbox in octets.

    $poparray = $this->last("array");

    if ($poparray == -1) {
      return false;
    }

    if ((!$poparray) or (empty ($poparray))) {
      return false;
    }
    return $poparray;
  }

  function uidl($msgnum = "") {
    //  Returns the UIDL of the msg specified. If called with
    //  no arguments, returns an associative array where each
    //  undeleted msg num is a key, and the msg's uidl is the element
    //  Array element 0 will contain the total number of msgs

    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 uidl:") .' '. t("No connection to server");
      return false;
    }

    $fp = $this->FP;
    $buffer = $this->BUFFER;

    if (!empty ($msgnum)) {
      $cmd = "UIDL $msgnum";
      $reply = $this->send_cmd($cmd);
      if (!$this->is_ok($reply)) {
        $this->ERROR = t("POP3 uidl:") .' '. t("Error ") ."[$reply]";
        return false;
      }
      list ($ok, $num, $myuidl) = preg_split('/\s+/', $reply);
      return $myuidl;
    }
    else {
      $this->update_timer();

      $uidlarray = array();
      $total = $this->COUNT;
      $uidlarray[0] = $total;

      if ($total < 1) {
        return $uidlarray;
      }
      $cmd = "UIDL";
      fwrite($fp, "UIDL\r\n");
      $reply = fgets($fp, $buffer);
      $reply = $this->strip_clf($reply);
      if ($this->DEBUG) {
        @ error_log("POP3 SEND [$cmd] GOT [$reply]", 0);
      }
      if (!$this->is_ok($reply)) {
        $this->ERROR = t("POP3 uidl:") .' '. t("Error ") ."[$reply]";
        return false;
      }

      $line = "";
      $count = 1;
      $line = fgets($fp, $buffer);
      while (!ereg("^\.\r\n", $line)) {
        if (ereg("^\.\r\n", $line)) {
          break;
        }
        list ($msg, $msguidl) = preg_split('/\s+/', $line);
        $msguidl = $this->strip_clf($msguidl);
        if ($count == $msg) {
          $uidlarray[$msg] = $msguidl;
        }
        else {
          $uidlarray[$count] = 'deleted';
        }
        $count++;
        $line = fgets($fp, $buffer);
      }
    }
    return $uidlarray;
  }

  function delete($msgnum = "") {
    //  Flags a specified msg as deleted. The msg will not
    //  be deleted until a quit() method is called.

    if (!isset ($this->FP)) {
      $this->ERROR = t("POP3 delete:") .' '. t("No connection to server");
      return false;
    }
    if (empty ($msgnum)) {
      $this->ERROR = t("POP3 delete:") .' '. t("No msg number submitted");
      return false;
    }
    $reply = $this->send_cmd("DELE $msgnum");
    if (!$this->is_ok($reply)) {
      $this->ERROR = t("POP3 delete:") .' '. t("Command failed ") ."[$reply]";
      return false;
    }
    return true;
  }

  //  *********************************************************
  //  The following methods are internal to the class.

  function is_ok($cmd = "") {
    //  Return true or false on +OK or -ERR

    if (empty ($cmd)) {
      return false;
    }
    else {
      return (ereg("^\+OK", $cmd));
    }
  }

  function strip_clf($text = "") {
    // Strips \r\n from server responses

    if (empty ($text))
      return $text;
    else {
      $stripped = str_replace("\r", '', $text);
      $stripped = str_replace("\n", '', $stripped);
      return $stripped;
    }
  }

  function parse_banner($server_text) {
    $outside = true;
    $banner = "";
    $length = strlen($server_text);
    for ($count = 0; $count < $length; $count++) {
      $digit = substr($server_text, $count, 1);
      if (!empty ($digit)) {
        if ((!$outside) && ($digit != '<') && ($digit != '>')) {
          $banner .= $digit;
        }
        if ($digit == '<') {
          $outside = false;
        }
        if ($digit == '>') {
          $outside = true;
        }
      }
    }
    $banner = $this->strip_clf($banner); // Just in case
    return "<$banner>";
  }

} // End class
