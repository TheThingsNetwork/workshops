# The Things Network Example Integration with AWS IoT

This is an example integration of The Things Network with AWS IoT.

[Learn more about AWS IoT](https://aws.amazon.com/iot/how-it-works/)

## Setup

Make sure you have your device(s) registered in AWS IoT. The ID of your device must be your DevEUI. This integration uses one certificate to publish messages for all devices. Make sure that this certificate is attached to your devices.

## Install

```
npm install ttn-aws-iot
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
