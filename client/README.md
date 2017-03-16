
# Client Workshop

In this workshop, we'll use our application and devices from the [introductory
workshop](../) and manage them using the [nodejs application
SDK](https://github.com/TheThingsNetwork/node-app-sdk).


## Pre-requisites

1. An TheThingsNetwork application with at least one device, registered in [the
   Console](https://console.thethingsnetwork.org).
2. [nodejs](https://nodejs.org/en/) installed on your machine.
3. A project folder from which to work, eg. `sdk-workshop`

## Installing the SDK

To install The Things Network node app SDK, run the following command
from your project folder:

```
npm install ttn
```

This should create a folder called `node_modules` inside the project folder.


## Handle events

To listen to messages from your application, create a script called `server.js`
inside the project folder:

```
// server.js
var ttn = require("ttn")

var region = "asia-se"
var appID = "<id>"
var accessKey = "<access_key>"

// create a client
var client = new ttn.Client(region, appID, accessKey)

// listen to messages
client.on("message", function (devID, data) {
  console.log("Message from device", devID)
  console.log(data)
})
```

Where you replace `<app_id>` with the ID of you application (eg. `hi-world`)
and `<access_key>` with the access key of your application.

To run this, run:

```
node server.js
```

Whenever you press the button, you should be seeing messages like the following
on the console:
```
Message from device my-uno
{ app_id: 'lollercopter',
  dev_id: 'my-uno',
  hardware_serial: '006964B20337B2C1',
  port: 1,
  counter: 11,
  payload_raw: <Buffer 03 cd>,
  payload_fields: { celcius: 9.73 },
  metadata: 
   { time: '2017-03-16T16:20:02.370281545Z',
     frequency: 867.1,
     modulation: 'LORA',
     data_rate: 'SF7BW125',
     coding_rate: '4/5',
     gateways: [ [Object] ] } }
```
