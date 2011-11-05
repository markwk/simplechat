<?php
/**
 * Implements hook_insert().
 */
function simplechat_chat_insert($node) {
  cache_clear_all('simplechat_chat_list', 'cache');
  $chat = array(
    'nid' => $node->nid,
    'crid' => isset($node->simplechat_nid) ? $node->simplechat_nid : 0,
    'max_users' => isset($node->max_users) ? $node->max_users : 0,
    'module' => isset($node->module) ? $node->module : 'simplechat',
    'previous_messages_display_count' => isset($node->previous_messages_display_count) ? $node->previous_messages_display_count : 20,
    // 'profile_picture' => isset($node->profile_picture) ? $node->profile_picture : '',
    // 'imagecache_preset' => isset($node->imagecache_preset) ? $node->imagecache_preset : '',
  );
  drupal_write_record('simplechat_chat', $chat);
  simplechat_chat_update_cache($node->nid, 0);
}

/**
 * Implements hook_delete().
 */
function simplechat_chat_delete(&$node) {
  cache_clear_all('simplechat_chat_list', 'cache');
  db_query('DELETE FROM {simplechat_chat} WHERE nid = :nid', array(':nid' => $node->nid));
  $cache_file = file_directory_temp() . '/simplechat.chat.' . $node->nid . '.cache';
  unlink($cache_file);
}
/**
 * Implements hook_load().
 */
function simplechat_chat_load($nodes) {
  $result = db_query("SELECT * FROM {simplechat_chat} WHERE nid IN (:nids)", array(':nids' => array_keys($nodes)));
  foreach ($result as $chat) {
    $chat->simplechat = $chat->crid ? node_load($chat->crid) : FALSE;
    $chat->latest_msg = simplechat_chat_get_latest_message($chat->nid);
    $chat->msg_count = simplechat_chat_get_message_count($chat->nid);
    $nodes[$chat->nid]->chat = $chat;
  }
}

/**
 * Get the latest message id for a given chat.
 *
 * @param mixed $ccid
 */
function simplechat_chat_get_latest_message($ccid) {
  $sql = "SELECT cm.*, u.name
          FROM {simplechat_msg} cm
          INNER JOIN {users} u ON u.uid = cm.uid
          WHERE ccid = :ccid
          ORDER BY cmid
          DESC LIMIT 1";
  return db_query($sql, array(':ccid' => $ccid))->fetchObject();
}

/**
 * Get the message count for a chat.
 */
function simplechat_chat_get_message_count($ccid) {
  $sql = 'SELECT COUNT(*) FROM {simplechat_msg} WHERE ccid = :ccid';
  return db_query($sql, array(':ccid' => $ccid))->fetchField();
}

/**
 * Get messages for a given chat.
 *
 * @param $chat_id
 *   The chat id.
 * @param $last_cmid
 *   Only load messages with cmids greater than this value.
 * @param $limit
 *   Default: FALSE.
 * @return array $messages
*/
function simplechat_chat_load_messages($chat_id, $last_cmid = 0, $limit = FALSE) {
  $sql = "SELECT cm.*, u.name, 0 AS guest_id
          FROM {simplechat_msg} cm
          INNER JOIN {users} u ON u.uid = cm.uid
          WHERE cm.ccid = :ccid
          AND cm.cmid > :cmid
          ORDER BY cm.cmid ASC";
  $args = array(':ccid' => $chat_id, ':cmid' => $last_cmid);
  if ($limit) {
    $sql .= ' LIMIT :limit';
    $args[':limit'] = $limit;
  }
  return simplechat_chat_load_messages_helper($sql, $args);
}

/**
 * Load the latest messages from a chat.
 *
 * @param mixed $node
 * @return array
 */
function simplechat_chat_load_latest_messages($node) {
  if (preg_match('/^\d+$/', $node->chat->previous_messages_display_count)) {
    $limit = $node->chat->previous_messages_display_count;
  }
  else {
    $limit = 10;
  }
  $sql = "SELECT cm.*, u.name, 0 AS guest_id
          FROM {simplechat_msg} cm
          INNER JOIN {users} u ON u.uid = cm.uid
          WHERE cm.ccid = :ccid 
          ORDER BY cm.cmid DESC
          LIMIT $limit";
  $messages = simplechat_chat_load_messages_helper($sql, array(':ccid' => $node->nid));
  return array_reverse($messages, TRUE);
}

/**
 * Helper function to load messages.
 *
 * @param mixed $sql
 * @param mixed $args
 * @return array
 */
function simplechat_chat_load_messages_helper($sql, $args) {
  $messages = array();
  $guest_sids = array();
  $guest_prefix = variable_get('simplechat_guest_user_prefix', t('guest-'));
  $allow_anon_names = variable_get('simplechat_allow_anon_name', TRUE);

  $result = db_query($sql, $args)->fetchAll(PDO::FETCH_OBJ);
  foreach ($result as $message) {
    if ($message->uid == 0 && !in_array($message->sid, $guest_sids)) {
      $guest_sids[] = $message->sid;
    }
    $messages[$message->cmid] = $message;
  }

  if (!empty($guest_sids)) {
    $result = db_select('simplechat_chat_online_list')
      ->fields('sid')
      ->fields('guest_id')
      ->condition('sid', $guest_sids, 'IN')
      ->execute()
      ->fetchAll(PDO::FETCH_OBJ);
    foreach ($result as $guest) {
      foreach ($messages as $message) {
        if ($message->sid == $guest->sid) {
          if ($message->anon_name && $allow_anon_names) {
            $messages[$message->cmid]->name = $message->anon_name . ' <span class="anon-label">(' . $guest_prefix . $guest->guest_id . ')</span>';
          }
          else {
            $messages[$message->cmid]->name = $guest_prefix . $guest->guest_id;
          }
          $messages[$message->cmid]->guest_id = $guest->guest_id;
        }
      }
    }
  }
  return $messages;
}

/**
 * Load a simplechat message from it's $cmid
 * 
 * @param mixed $cmid
 * @return mixed
 */
function simplechat_chat_load_message($cmid) {
  $guest_prefix = variable_get('simplechat_guest_user_prefix', t('guest-'));
  $allow_anon_names = variable_get('simplechat_allow_anon_name', TRUE);

  $sql = "SELECT cm.*, u.name
          FROM {simplechat_msg} cm
          INNER JOIN {users} u ON u.uid = cm.uid
          WHERE cm.cmid = :cmid";
  if ($message = db_query($sql, array(':cmid' => $cmid))->fetchObject()) {
    $message->guest_id = 0;
    if ($message->uid == 0) {
      $sql = 'SELECT * FROM {simplechat_chat_online_list} WHERE sid = :sid';
      $guest = db_query($sql, array(':sid' => $message->sid))->fetchObject();
      if ($message->anon_name && $allow_anon_names) {
        $message->name = $message->anon_name . ' <span class="anon-label">(' . $guest_prefix . $guest->guest_id . ')</span>';
      }
      else {
        $message->name = $guest_prefix . $guest->guest_id;
      }
      $message->guest_id = $guest->guest_id;
    }
  }
  return $message;
}

/**
 * Prepare a message to be sent down to a client.
 *
 * @param mixed $message 
 * @return stdClass $msg
 */
function simplechat_chat_prepare_message($message, $node) {
  global $user;

  $msg = new StdClass();
  $msg->html = simplechat_chat_get_themed_message($message, $node);
  $msg->text = strip_tags($message->msg);
  $msg->name = $message->name;
  $msg->name_stripped = strip_tags($message->name);
  $msg->cmid = $message->cmid;
  $msg->type = $message->msg_type;
  $msg->uid = $message->uid;
  $msg->modified = $message->modified;
  $msg->date_format = variable_get('simplechat_msg_date_format', 'H:i:s');
  $msg->themed_username = theme('simplechat_message_username', array('message' => $message, 'skip_permission_check' => TRUE));
  $msg->themed_message = theme('simplechat_message', array('message' => $message, 'node' => $node));
  $msg->recipient_uid = $message->recipient_uid;
  $msg->viewed_uid = $user->uid;
  return $msg;
}

/**
 * Wrapper around theme('simplechat_message').
 *
 * @param mixed $message
 * @param mixed $node
 */
function simplechat_chat_get_themed_message($message, $node) {
  global $user;

  if ($message->uid && user_access('access user profiles')) {
    $username = theme('simplechat_message_username', array('message' => $message, 'skip_permission_check' => FALSE));
  }
  else {
    $username = $message->name;
  }
  $public_css_class = variable_get('simplechat_msg_public_class', 'simplechat-msg');
  $private_css_class = variable_get('simplechat_msg_private_class', 'simplechat-private');
  $class = "new-message $public_css_class" . ($message->msg_type == 'private_message' ? " $private_css_class" : '');
  $output = '<div class="' . $class . '">';
  $output .= '(' . simplechat_get_message_time_string($message->modified) . ') <strong>' . $username . ':</strong> ';
  $output .= theme('simplechat_message', array('message' => $message, 'node' => $node));
  $output .= "</div>";
  return $output;
}

/**
 * Get the time string for a message from a UTC timestamp.
 *
 * @param mixed $timestamp
 * @return string
 */
function simplechat_get_message_time_string($timestamp) {
  $offset_in_hours = simplechat_get_user_timezone_offset() / 60 / 60;
  $offset_in_hours = $offset_in_hours >= 0 ? '+' . $offset_in_hours : $offset_in_hours;
  $date = new DateTime('@' . ($timestamp + simplechat_get_user_timezone_offset()), new DateTimeZone('Etc/GMT' . $offset_in_hours));
  return $date->format(variable_get('simplechat_msg_date_format', 'H:i:s'));
}

/**
 * Return a UTC timestamp.
 *
 * @return int timestamp
 */
function simplechat_get_utc_timestamp() {
  $date = new DateTime('now', new DateTimeZone('UTC'));
  return $date->format('U');
}

/**
 * Posted messages are handled here.
 *
 * @return boolean
 */
function simplechat_check_token($token, $form_id) {
  return $token == drupal_get_token($form_id);
}

/**
 * Posted messages are handled here.
 *
 * @return void
 */
function simplechat_chat_post_message($node, $last_msg_id) {
  global $user;

  // Check we have a message posted.
  if (!isset($_POST['message'])) {
    exit;
  }

  if (!simplechat_check_token($_POST['formToken'], $_POST['formId'])) {
    exit;
  }

  $message = array(
    'ccid' => $node->chat->nid,
    'uid' => $user->uid,
    'msg' => $_POST['message'],
    'sid' => session_id(),
    'msg_type' => simplechat_chat_get_message_type($_POST['message']),
    'recipient_uid' => 0,
    'modified' => simplechat_get_utc_timestamp(),
    'anon_name' => isset($_POST['anonName']) ? $_POST['anonName'] : '',
  );
  if ($message['msg_type'] == 'command') {
    $result = simplechat_chat_call_command($message, $node);
  }
  else {
    $result = simplechat_chat_save_message($message, $node);
  }
}

/**
 * Return the current user's offset from UTC.
 */
function simplechat_get_user_timezone_offset() {
  global $user;

  $offset = variable_get('date_default_timezone', 0);
  if (variable_get('configurable_timezones', TRUE) && isset($user->timezone)) {
    $offset = $user->timezone;
  }
  return $offset;
}

/**
 * Write a message into a chat, and allow any interested modules to react.
 *
 * @param array $message
 * @param StdClass $node
 * @return boolean
 */
function simplechat_chat_save_message($message, $node) {
  drupal_alter('simplechat_msg', $message);
  if (drupal_write_record('simplechat_msg', $message)) {
    $node->chat->latest_msg_id = $message['cmid'];
    // from simplechat_node.js module
    global $user;
    $nodejs_message = (object) array(
                    'channel' => 'simplechat_' . $node->nid,
                    'broadcast' => FALSE,
                    'type' => 'newMessage',
                    'callback' => 'simplechatNodejsMessageHandler',
                    'data' => simplechat_chat_prepare_message(simplechat_chat_load_message($message['cmid']), $node),
    );
    nodejs_send_message($nodejs_message);
    nodejs_add_user_to_channel($user->uid, 'simplechat_' . $node->nid);
    // --
    foreach (module_implements('chat_msg_saved') as $module) {
      $function = $module . '_chat_msg_saved';
      $function($message, $node);
    }
    return $message['cmid'];
  }
  return FALSE;
}

