'use strict';

const util = require('util');
const ttn = require('ttn');
const awsiot = require('aws-iot-device-sdk');

const mqttBroker = 'staging.thethingsnetwork.org'
const appEUI = '<insert your AppEUI>';
const appAccessKey = '<insert your App Access Key>';
const keyPath = './devices';
const region = 'eu-west-1';

// Create the client to The Things Network
var client = new ttn.Client(mqttBroker, appEUI, appAccessKey);

var devices = {};

// A device activates
client.on('activation', activation => {
  console.log('%s: Device activated', activation.devEUI);

  // If we already have a connection, close it first
  if (devices[activation.devEUI]) {
    console.log('%s: Closing existing connection', activation.devEUI);
    devices[activation.devEUI].end();
  }

  // Initialize the device with the security keys. Get these from the AWS IoT
  var device = awsiot.device({
    clientCert: util.format('%s/%s.crt', keyPath, activation.devEUI),
    privateKey: util.format('%s/%s.key', keyPath, activation.devEUI),
    caCert: util.format('%s/root-CA.crt', keyPath),
    clientId: activation.devEUI,
    region: region
  });

  device.on('connect', () => {
    console.log('%s: Connected', activation.devEUI);
    devices[activation.devEUI] = device;
  });

  device.on('error', err => {
    console.log('%s: %s', err);
  })
});

// A device sends a message
client.on('uplink', uplink => {
  console.log('%s: Got uplink: %j', uplink.devEUI, uplink.fields);

  var device = devices[uplink.devEUI];
  if (!device) {
    console.warn('%s: Device is not activated', uplink.devEUI);
    return;
  }

  // Prepare the message to send
  var data = Object.assign({
    counter: uplink.counter,
    time: uplink.metadata.server_time
  }, uplink.fields);

  // Send the message to AWS IoT
  console.log('%s: Sending message: %j', uplink.devEUI, data);
  device.publish('up', JSON.stringify(data));
});

client.connect();
