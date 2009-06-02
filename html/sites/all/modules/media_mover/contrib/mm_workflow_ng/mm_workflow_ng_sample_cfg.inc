<?php

function mm_workflow_ng_configuration() {
$configurations = array (
  'cfg_13' => 
  array (
    '#type' => 'configuration',
    '#altered' => false,
    '#event' => 'media_mover_harvest_file',
    '#label' => 'Media Mover Harvest Log Test',
    '#active' => 0,
    '#module' => 'workflow-ng',
    0 => 
    array (
      '#type' => 'condition',
      '#name' => 'workflow_ng_condition_mm_file_type_is',
      '#argument map' => 
      array (
        'mm_file' => 'mm_file',
      ),
      '#label' => 'Media Mover file type is',
      '#settings' => 
      array (
        'extensions' => 'avi',
      ),
    ),
    1 => 
    array (
      '#type' => 'action',
      '#name' => 'workflow_ng_action_watchdog',
      '#settings' => 
      array (
        'severity' => '0',
        'type' => 'workflow-ng',
        'message' => '[mm_file:data-filename] made it to this step',
        'message_args' => 
        array (
          0 => 'mm_file',
        ),
        'link' => '',
        'link_args' => 
        array (
        ),
      ),
    ),
    '#name' => 'cfg_13',
  ),
  'cfg_15' => 
  array (
    '#type' => 'configuration',
    '#altered' => false,
    '#event' => 'media_mover_complete_file',
    '#label' => 'Notify Admin abount newly processed files over 500k',
    '#active' => 0,
    '#module' => 'workflow-ng',
    0 => 
    array (
      '#type' => 'condition',
      '#name' => 'workflow_ng_condition_token_numeric',
      '#settings' => 
      array (
        'text1' => '[mm_file:data-filesize]',
        'text1_args' => 
        array (
          0 => 'mm_file',
        ),
        'text2' => '500000',
        'text2_args' => 
        array (
        ),
        'operation' => 'less',
      ),
      '#label' => 'File Size > 500k',
    ),
    1 => 
    array (
      '#type' => 'action',
      '#name' => 'workflow_ng_action_mail',
      '#settings' => 
      array (
        'to' => '[node:site-mail]',
        'to_args' => 
        array (
          0 => 'node',
        ),
        'from' => '[node:site-mail]',
        'from_args' => 
        array (
          0 => 'node',
        ),
        'subject' => '[user:user] Just uploaded [mm_file:data-filename] of [mm_file:data-filesize]',
        'subject_args' => 
        array (
          0 => 'mm_file',
          1 => 'user',
        ),
        'message' => 'This came in from:
[mm_config:name]
which started at: [mm_config:start_time]

If you are lazy, [mm_config:name] uses:

[mm_config:harvest-module] for harvesting
[mm_config:process-module] for processing
[mm_config:storage-module] for storage
[mm_config:complete-module] for completion

Adios!

"The token programmer"',
        'message_args' => 
        array (
          0 => 'mm_config',
        ),
      ),
      '#label' => 'Send a mail to an arbitrary mail address',
    ),
    2 => 
    array (
      '#type' => 'action',
      '#name' => 'workflow_ng_action_watchdog',
      '#settings' => 
      array (
        'severity' => '0',
        'type' => 'workflow-ng',
        'message' => 'sent a mail to [user:mail] because of [mm_file:data-filesize] file',
        'message_args' => 
        array (
          0 => 'mm_file',
          1 => 'user',
        ),
        'link' => '',
        'link_args' => 
        array (
        ),
      ),
      '#label' => 'Log to watchdog',
    ),
    3 => 
    array (
      '#type' => 'action',
      '#name' => 'workflow_ng_action_drupal_message',
      '#settings' => 
      array (
        'message' => 'This came in from:
[mm_config:name]
which started at: [mm_config:start_time]

If you are lazy, [mm_config:name] uses:

[mm_config:harvest-module] for harvesting
[mm_config:process-module] for processing
[mm_config:storage-module] for storage
[mm_config:complete-module] for completion

Adios!

"The token programmer"',
        'message_args' => 
        array (
          0 => 'mm_config',
        ),
        'error' => 0,
      ),
    ),
    '#name' => 'cfg_15',
  ),
);

return $configurations;

}
