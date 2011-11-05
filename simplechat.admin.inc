<?php
/**
 * @file
 * Admin page callback for Simple Chat module
 */

/**
 * Admin Settings Form
 */
function simplechat_admin_settings() {
  $form['simplechat_maxusers'] = array(
	'#type' => 'select',
	'#options' => drupal_map_assoc(range(2, 1000)),
	'#title' => t('Maximum Users Per Simple Chat'),
	'#default_value' => variable_get('simplechat_maxusers', '20'),
	'#description' => t('Select Maximum Number of Users Per Simple Chat'),
	'#required' => TRUE,
  );
  return system_settings_form($form);
}