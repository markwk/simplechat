<?php

/**
 * Implements hook_theme().
 */
function simplechat_theme($existing, $type, $theme, $path) {
  return array(
	'simplechat_default' => array(
	    'variables' => array('element' => NULL),
	),
    'simplechat_disabled' => array(
	    'variables' => array('element' => NULL),
	    'template' => 'simplechat_disabled', 
	    'path' => drupal_get_path('module', 'simplechat') . '/templates',
	),
	'simplechat_buttons' => array(
      'arguments' => array('node'),
    ),
    'simplechat_message_username' => array(
      'arguments' => array('message', 'skip_permission_check'),
    ),
    'simplechat_latest_message' => array(
      'arguments' => array('message'),
    ),
    'simplechat_message' => array(
      'arguments' => array('message', 'node'),
    ),
    'simplechat_system_message' => array(
      'arguments' => array('message', 'node'),
    ),
   );
}

/**
 * Implements theme().
 */
function theme_simplechat_default($element) {
  global $user;

  $node = $variables['node'];

  $output = '<div id="simplechat-container">';
  $output .= '<div id="simplechat-board">';
  foreach ($node->chat->latest_messages as $message) {
    $output .= simplechat_chat_get_themed_message($message, $node);
  }
  $output .= '</div>';
  if(user_access('send chat messages'))
  $output .= theme('simplechat_buttons', array('node' => $node));
  $output .= '</div></div>';
  return $output;
}

/**
 * Format an array of messages.
 *
 * @ingroup themeable
 */
function theme_simplechat_messages(array $messages, $node) {
  $output = '';
  foreach($messages as $message) {
    $output .= simplechat_chat_get_themed_message($message, $node);
  }
  return $output;
}

/**
 * Format a single message for display.
 *
 * @param mixed $variables
 * @return string
 */
function theme_simplechat_message($variables) {
  $message = $variables['message'];
  $node = $variables['node'];
  return check_markup($message->msg, variable_get('simplechat_message_format', NULL),"", FALSE);
}

/**
 * Format a single system message for display.
 *
 * @param mixed $message
 * @param mixed $node
 * @return string
 */
function theme_simplechat_system_message($variables) {
  $node = $variables['node'];
  $message = $variables['message'];
//  return '<div class="simplechat-system-msg new-message">' . check_markup($message, $node->format,"", FALSE) . '</div>';
  return '<div class="simplechat-system-msg new-message">' . $message . '</div>';
}

/**
 * Format a message for display in a summary table.
 */
function theme_simplechat_latest_message($message) {
  $message = $variables['message'];
  $output = check_plain($message->msg) . '<br />';
  $output .= t('Posted by <strong>!user</strong> on !date', array(
    '!user' => $message->uid ? l($message->name, "user/$message->uid") : variable_get('simplechat_guest_user_prefix', t('guest-')) . $message->guest_id,
    '!date' => format_date($message->modified, 'medium'),
  ));
  return $output;
}

/**
 * Form fields for simplechat text entry.
 *
 * @param mixed $form_state
 * @param mixed $node
 * @return array
 */
function simplechat_chat_buttons($form_state, $node) {
  global $user;
  $form['simplechat_chat_form_token'] = array(
    '#type' => 'hidden',
    '#value' => drupal_get_token('simplechat_chat_buttons'),
    '#attributes' => array('id' => 'edit-simplechat-chat-buttons-form-token'),
  );
  $form['simplechat_message_entry_box'] = array(
    '#type' => 'textarea',
    '#title' => t('Enter your message text here'),
    '#size' => 50,
    '#rows' => 1,
    '#maxlength' => variable_get('simplechat_max_message_size', 1000),
  );
  $form['simplechat_message_entry_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Chat'),
  );
  return $form;
}

/**
 * Theme chat buttons.
 *
 * @ingroup themeable
 */
function theme_simplechat_buttons($variables) {
  $node = $variables['node'];
  $output = drupal_render(drupal_get_form('simplechat_chat_buttons', $node));
  return '<div id="simplechat-chat-buttons">' . $output . '</div>';
}


/**
 * Theme the teaser for a chat node.
 *
 * @param mixed $variables
 * @return string
 */
function theme_simplechat_chat_teaser($variables) {
  $node = $variables['node'];
  if (isset($node->chat)) {
    if ($node->chat->when_archived) {
      $params = array(
        '!date' => format_date($node->chat->when_archived, 'medium'),
        '%message_count', $node->chat->msg_count,
      );
      return t('Archived on !date, contains %message_count messages.', $params);
    }
    else {
      if ($node->chat->latest_msg) {
        $params = array(
          '!date' => format_date($node->chat->latest_msg->modified, 'medium'),
          '%message' => $node->chat->latest_msg->msg,
          '!user' => theme('username', array('account' => user_load($node->chat->latest_msg->uid))),
          '%message_count' => $node->chat->msg_count,
        );
        return t("Last message at !date: %message by !user. Total messages: %message_count.", $params);
      }
      else {
        return t('There are no messages in this chat.');
      }
    }
  }
}

/**
 * Format a username in a simplechat message.
 *
 * @param mixed $message
 * @param mixed $skip_permission_check
 * @return string
 */
function theme_simplechat_message_username($variables) {
  $message = $variables['message']; 
  $skip_permission_check = $variables['skip_permission_check'];
  if ($skip_permission_check || user_access('access user profiles')) {
    if ($message->uid > 0) {
      return l($message->name, 'user/'. $message->uid, array('attributes' => array('title' => t('View user profile.'))));
    }
    else {
      if ($message->anon_name) {
        return check_plain($message->anon_name) . ' <span class="anon-label">(' . variable_get('simplechat_guest_user_prefix', t('guest-')) . $message->guest_id . ')</span>';
      }
      else {
        return check_plain($message->name);
      }
    }
  }
  else {
    return check_plain($message->name);
  }
}

/**
 * Implements theme().
 */
function theme_simplechat_disabled($element) {

  return theme_render_template(drupal_get_path('module', 'simplechat') .'/templates/simplechat_disabled.tpl.php');
}

