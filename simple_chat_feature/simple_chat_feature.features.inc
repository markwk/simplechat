<?php
/**
 * @file
 * simple_chat_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function simple_chat_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function simple_chat_feature_node_info() {
  $items = array(
    'simple_chat' => array(
      'name' => t('Simple Chat'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
