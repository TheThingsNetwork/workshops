var app = require('http').createServer(handler)
var io = require('socket.io')(app);
var fs = require('fs');
var ttn = require('ttn');

// Run the webserver on port 8080
app.listen(8080);

// Configuration for The Things Network
// Find your values with "ttnctl applications" or on the Dashboard:
// https://staging.thethingsnetwork.org/applications/
// From your application overview go to "learn how to get data from this app"
var appEUI = '70B3D57ED0000...';
var accessKey = 'SuperSecretAccessKey=';

// Start the TTN Client
var client = new ttn.Client('staging.thethingsnetwork.org', appEUI, accessKey);

// Forward uplink to appEUI room in Socket.io
client.on('uplink', function (msg) {
  console.log("Uplink from Device: " + msg.devEUI)
  io.to(appEUI).emit('uplink', msg)
});

// Forward activations to appEUI room in Socket.io
client.on('activation', function (evt) {
  console.log("Activated Device: " + evt.devEUI)
  io.to(appEUI).emit('activation', evt)
});

// Print errors to the console
client.on('error', function (err) {
  console.log("Error: " + err)
});

// Close the TTN client on exit
process.on('exit', function(code) {
  client.end()
});

// Serve the index.html file
function handler (req, res) {
  fs.readFile(__dirname + '/index.html',
  function (err, data) {
    if (err) {
      res.writeHead(500);
      return res.end('Error loading index.html');
    }
    res.writeHead(200);
    res.end(data);
  });
}

// Add Socket.io clients to the appEUI room
io.on('connection', function (socket) {
  socket.join(appEUI);
});
