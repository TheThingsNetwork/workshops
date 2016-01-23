/*
 * For any new row in the table, send a notification
 * DOC here: https://parse.com/docs/rest/guide#push-notifications-uploading-installation-data
 */
Parse.Cloud.afterSave("datas", function(request) {
    var query = new Parse.Query(Parse.Installation); // Everyone!
    Parse.Push.send({
      where: query, // Set our Installation query
      data: {
        alert: "New input data from TTN!"
      }
    }, {
      success: function() {
        // Push was successful
      },
      error: function(error) {
        // Handle error
      }
    });
});
