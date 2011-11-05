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
  $form['simplechat_chat_message_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Chat message help text'),
    '#description' => t('The text to be displayed next to the chat message form field.'),
    '#default_value' => variable_get('simplechat_chat_message_text', t('Type your message and hit enter')),
    '#size' => 20,
  );
  $form['simplechat_chat_date_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Chat date format'),
    '#attributes' => array('class' => array('custom-format')),
    '#default_value' => variable_get('simplechat_chat_date_format', '* \S\e\n\t \a\t G:i'),
    '#description' => t('Format for system time messages in chats. See the <a href="@url">PHP manual</a> for available options. This format is currently set to display as <span>%date</span>.', array('@url' => 'http://php.net/manual/function.date.php', '%date' => format_date(REQUEST_TIME, 'custom', variable_get('simplechat_chat_date_format', '* \S\e\n\t \a\t G:i')))),
  );
  return system_settings_form($form);
}