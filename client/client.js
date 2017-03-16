// server.js
var ttn = require("ttn")

var region = "asia-se"
var appID = "<app_id>"
var accessKey = "<access_key>"

// create a client
var client = new ttn.Client(region, appID, accessKey)

// listen to messages
client.on("message", function (devID, data) {
  console.log("Message from device", devID)
  console.log(data)
})
