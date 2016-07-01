# The Things Network Example Integration with AWS IoT

This is an example integration of The Things Network with AWS IoT.

[Learn more about AWS IoT](https://aws.amazon.com/iot/how-it-works/)

## Setup

Make sure you have your device(s) registered in AWS IoT. The ID of your device must be your DevEUI. This integration uses one certificate to publish messages for all devices. Make sure that this certificate is attached to your devices.

## Install

```
npm install --save ttn-aws-iot
```

## Example

```js
'use strict';

const ttnawsiot = require('ttn-aws-iot');

// Replace with your AppEUI and App Access Key
const appEUI = '<insert AppEUI>';
const appAccessKey = '<insert App Access Key>';

// Replace with your certificates from AWS IoT
const caCert = 'examples/keys/root-CA.crt';
const clientCert = 'examples/keys/cert.crt';
const privateKey = 'examples/keys/private.key';
const region = 'eu-west-1';

const bridge = new ttnawsiot.Bridge(appEUI, appAccessKey, caCert, clientCert, privateKey, region);

bridge.on('aws-connect', () => {
  console.log('AWS connected');
});

bridge.on('ttn-connect', () => {
  console.log('TTN connected');
});

bridge.on('error', err => {
  console.warn('Error', err);
});

bridge.on('uplink', data => {
  console.log('Uplink', data);
});
```

## Options

When creating and initializing the `Bridge`, you can specify options:

```js
const options = {};
const bridge = new ttnawsiot.Bridge(appEUI, appAccessKey, caCert, clientCert, privateKey, region, options);
```

### `ttnBroker`

The MQTT broker to connect to. Default:

```js
options.ttnBroker = 'staging.thethingsnetwork.org'
```

### `createMessage`

The function to create a message. By default, the message is a combination of the result of the payload functions `fields`, the unique device ID and the server time:

```js
options.createMessage = function(uplink) {
  return {
    state: {
      reported: uplink.fields
    }
  };
}
```

*Note: if there are no payload functions specified for the concerning application, the `fields` object contains a `raw` property with the bytes received.*
