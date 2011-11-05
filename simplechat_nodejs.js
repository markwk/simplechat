
(function ($) {

Drupal.Nodejs.callbacks.simplechatNodejsMessageHandler = {
  callback: function (message) {
    switch (message.type) {
      case 'newMessage':
        Drupal.simplechat.addMessagesToBoard([message.data]);
        break;
      case 'userSeen':
        Drupal.simplechat.updateUserList(message.data);
        break;
      case 'newCommandMessage':
        Drupal.simplechat.addCommandMessage(response.data);
        break;
    }
  }
};

})(jQuery);

