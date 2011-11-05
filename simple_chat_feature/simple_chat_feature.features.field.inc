<?php
/**
 * @file
 * simple_chat_feature.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function simple_chat_feature_field_default_fields() {
  $fields = array();

  // Exported field: 'node-simple_chat-field_simple_chat'
  $fields['node-simple_chat-field_simple_chat'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_simple_chat',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'simplechat',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'simplechat_app',
    ),
    'field_instance' => array(
      'bundle' => 'simple_chat',
      'default_value' => array(
        0 => array(
          'show_simplechat' => 1,
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'simplechat',
          'settings' => array(),
          'type' => 'simplechat_default',
          'weight' => 0,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_simple_chat',
      'label' => 'Simple Chat',
      'required' => 0,
      'settings' => array(
        'show_simplechat' => TRUE,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'simplechat',
        'settings' => array(),
        'type' => 'simplechat_widget',
        'weight' => '-4',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Simple Chat');

  return $fields;
}
