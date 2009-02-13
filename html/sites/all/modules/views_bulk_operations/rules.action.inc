<?php
// $Id: rules.action.inc,v 1.1.2.1 2009/02/12 23:09:49 kratib Exp $

function views_bulk_operations_rules_action_info() {
  if (!module_exists('rules')) return array();
  $actions = array();
  foreach (rules_get_configured_items('rule_sets') as $ruleset_key => $ruleset) {
    if (count($ruleset['arguments']) == 1) { // For now, we only accept rulesets with one parameter (taken to be the 'type')
      $arg = key($ruleset['arguments']);
      $actions['views_bulk_operations_ruleset_action'] = array(
        'type' => $ruleset['arguments'][$arg]['type'],
        'parameters' => array('ruleset' => $ruleset_key),
        'description' => $ruleset['label'],
        'configurable' => false,
      );
    }
  }
  return $actions;
}

function views_bulk_operations_ruleset_action(&$object, $context) {
  rules_invoke_rule_set($context['ruleset'], $object);
}

