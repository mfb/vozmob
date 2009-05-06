<?php
/**
 ** Override the mail function
 ** so that we can pass the FIFTH PARAMETER.
 **/
function drupal_mail_wrapper($message) {
    $mimeheaders = array();
    foreach ($message['headers'] as $name => $value) {
         $mimeheaders[] = $name .': '. mime_header_encode($value);
    }
    return mail(
      $message['to'],
      mime_header_encode($message['subject']),
      // Note: e-mail uses CRLF for line-endings, but PHP's API requires LF.
      // They will appear correctly in the actual e-mail that is sent.
      str_replace("\r", '', $message['body']),
      // For headers, PHP's API suggests that we use CRLF normally,
     // but some MTAs incorrecly replace LF with CRLF. See #234403.
      join("\n", $mimeheaders),
      ($message['from'] ? '-f' . $message['from'] : ''));
}
