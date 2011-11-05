(function ($) {

Drupal.simplechat = Drupal.simplechat || {'initialised' : false};

/**
 * Add behaviours to simplechat elements.
 */
Drupal.behaviors.simplechat = {
  attach: function (context, settings) {
    if (!Drupal.simplechat.initialised) {
      Drupal.settings.simplechat.pageTitle = document.title;
      Drupal.settings.simplechat.hasFocus = true;
      if (Drupal.settings.simplechat.latestMsgId > 0) {
        var targetOffset = $('div.new-message:last').offset().top;
        var boardOffset = $('#simplechat-board').offset().top;
        var scrollAmount = targetOffset - boardOffset;
        $('#simplechat-board').animate({scrollTop: '+='+ scrollAmount +'px'}, 500);
        $('.new-message').removeClass('new-message');
      }
      $(self).focus(
        function() {
          clearInterval(Drupal.settings.simplechat.warnInterval);
          Drupal.settings.simplechat.hasFocus = true;
          document.title = Drupal.settings.simplechat.pageTitle;
        }
      );
      $(self).blur(
        function() {
          Drupal.settings.simplechat.hasFocus = false;
        }
      );
      Drupal.simplechat.initialised = true;
    }

    $('#edit-simplechat-message-entry-box').keyup(function(e) {
      var messageText = $('#edit-simplechat-message-entry-box').val().replace(/^\s+|\s+$/g, '');
      var anonNameText = '';
      if ($('#edit-simplechat-anon-name').length) {
        anonNameText = $('#edit-simplechat-anon-name').val().replace(/^\s+|\s+$/g, '');
      }
      if (messageText && e.keyCode == 13 && !e.shiftKey && !e.ctrlKey) {
        Drupal.simplechat.postMessage(messageText, anonNameText);
        $('#edit-simplechat-message-entry-box').val('').focus();
      }
      else {
        return true;
      }
    });
    $('#edit-simplechat-message-entry-submit').click(function (e) {
      e.preventDefault();
      e.stopPropagation();
      var messageText = $('#edit-simplechat-message-entry-box').val().replace(/^\s+|\s+$/g, '');
      var anonNameText = '';
      if ($('#edit-simplechat-anon-name').length) {
        anonNameText = $('#edit-simplechat-anon-name').val().replace(/^\s+|\s+$/g, '');
      }
      if (messageText) {
        Drupal.simplechat.postMessage(messageText, anonNameText);
        $('#edit-simplechat-message-entry-box').val('').focus();
      }
    });

    $('.simplechat-kick-user-link').click(function (e) {
      e.preventDefault();
      e.stopPropagation();
      Drupal.simplechat.kickUser(e.target.parentNode.id);
    });

    $('.simplechat-ban-user-link').click(function (e) {
      e.preventDefault();
      e.stopPropagation();
      Drupal.simplechat.banUser(e.target.parentNode.id);
    });

    $('.simplechat-remove-user-link').click(function (e) {
      e.preventDefault();
      e.stopPropagation();
      Drupal.simplechat.removeUser(e.target.parentNode.id);
    });
  }
};

Drupal.simplechat.banUser = function(uid) {
  $.ajax({
    type: 'POST',
    url: Drupal.settings.basePath + Drupal.settings.simplechat.banUserPath + '/' + Drupal.settings.simplechat.chatId,
    dataType: 'json',
    success: Drupal.simplechat.pollHandler,
    data: {
      uid: uid,
      formToken: $('#simplechat-chat-management-form input[name="form_token"]').val(),
      formId: 'simplechat_chat_management_form'
    }
  });
}

Drupal.simplechat.kickUser = function(uid) {
  $.ajax({
    type: 'POST',
    url: Drupal.settings.basePath + Drupal.settings.simplechat.kickUserPath + '/' + Drupal.settings.simplechat.chatId,
    dataType: 'json',
    success: Drupal.simplechat.pollHandler,
    data: {
      uid: uid,
      formToken: $('#simplechat-chat-management-form input[name="form_token"]').val(),
      formId: 'simplechat_chat_management_form'
    }
  });
}

Drupal.simplechat.removeUser = function(uid) {
  $.ajax({
    type: 'POST',
    url: Drupal.settings.basePath + Drupal.settings.simplechat.removeUserPath + '/' + Drupal.settings.simplechat.chatId,
    dataType: 'json',
    success: Drupal.simplechat.pollHandler,
    data: {
      uid: uid,
      formToken: $('#simplechat-chat-management-form input[name="form_token"]').val(),
      formId: 'simplechat_chat_management_form'
    }
  });
}


Drupal.simplechat.updateUserList  = function(usersHtml) {
  $('#simplechat-user-list-wrapper').replaceWith(usersHtml);
  Drupal.attachBehaviors('#simplechat-user-list-wrapper');
}

Drupal.simplechat.addMessagesToBoard = function(messages) {
  var newMessage = false;
  for (var i = 0; i < messages.length; i++) {
    // Poll requests can pass each other over the wire, so we can't rely on
    // getting a given message once only, so only add if we haven't already
    // done so.
    if (messages[i].cmid > Drupal.settings.simplechat.latestMsgId) {
      Drupal.settings.simplechat.latestMsgId = messages[i].cmid;
      $('#simplechat-board').append(messages[i].html);
      newMessage = messages[i];
      if (messages[i].newDayHtml) {
        $('#simplechat-board').append(messages[i].newDayHtml);
      }
    }
  }
  if (newMessage) {
    Drupal.simplechat.scrollToLatestMessage();
    if (Drupal.settings.simplechat.hasFocus == false) {
      Drupal.settings.simplechat.newMsg = newMessage;
      clearInterval(Drupal.settings.simplechat.warnInterval);
      Drupal.settings.simplechat.warnInterval = setInterval("Drupal.simplechat.warnNewMsgLoop()", 1500);
    }
  }
}

Drupal.simplechat.addCommandMessage = function(response) {
  $('#simplechat-board').append('<div class="new-message command-message">** ' + response.msg + '</div>');
  Drupal.simplechat.scrollToLatestMessage();
}

Drupal.simplechat.addCommandMessage = function(response) {
  $('#simplechat-board').append('<div class="new-message command-message">** ' + response.msg + '</div>');
  Drupal.simplechat.scrollToLatestMessage();
}

Drupal.simplechat.scrollToLatestMessage = function() {
  var boardOffset = $('#simplechat-board').offset().top;
  var targetOffset = $('div.new-message:last').offset().top;
  var scrollAmount = targetOffset - boardOffset;
  $('#simplechat-board').animate({scrollTop: '+='+ scrollAmount +'px'}, 500);
  $('.new-message').removeClass('new-message');
}

Drupal.simplechat.postMessage = function(message, anonName) {
  // Fix the resolution of the form tokens
  $.ajax({
    type: 'POST',
    url: Drupal.settings.basePath + Drupal.settings.simplechat.postMessagePath + '/' + Drupal.settings.simplechat.chatId + '/' + Drupal.settings.simplechat.latestMsgId,
    dataType: 'json',
    success: Drupal.simplechat.pollHandler,
    data: {
      message: message,
      anonName: anonName,
      formToken: $('#edit-simplechat-chat-buttons-form-token').val(),
      formId: 'simplechat_chat_buttons'
    }
  });
}

Drupal.simplechat.warnNewMsgLoop = function() {
  if (document.title == Drupal.settings.simplechat.pageTitle) {
    document.title = Drupal.settings.simplechat.newMsg.name_stripped + ' says: ' + Drupal.settings.simplechat.newMsg.text;
  }
  else {
    document.title = Drupal.settings.simplechat.pageTitle;
  }
}

})(jQuery);

