<?php
/**
 * @file
 * simple_chat_feature.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function simple_chat_feature_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_simple_chat';
  $strongarm->value = 0;
  $export['comment_anonymous_simple_chat'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_simple_chat';
  $strongarm->value = 1;
  $export['comment_default_mode_simple_chat'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_simple_chat';
  $strongarm->value = '50';
  $export['comment_default_per_page_simple_chat'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_simple_chat';
  $strongarm->value = 1;
  $export['comment_form_location_simple_chat'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_simple_chat';
  $strongarm->value = '1';
  $export['comment_preview_simple_chat'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_simple_chat';
  $strongarm->value = '1';
  $export['comment_simple_chat'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_simple_chat';
  $strongarm->value = 1;
  $export['comment_subject_field_simple_chat'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_simple_chat';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_simple_chat'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_simple_chat';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_simple_chat'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_simple_chat';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_simple_chat'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_simple_chat';
  $strongarm->value = '1';
  $export['node_preview_simple_chat'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_simple_chat';
  $strongarm->value = 1;
  $export['node_submitted_simple_chat'] = $strongarm;

  return $export;
}
