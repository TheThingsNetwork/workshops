'use strict';

const ttnawsiot = require('..');

// Replace with your AppEUI and App Access Key
const appEUI = '<insert AppEUI>';
const appAccessKey = '<insert App Access Key>';

// Replace with your certificates from AWS IoT
const caCert = 'keys/root-CA.crt';
const clientCert = 'keys/cert.crt';
const privateKey = 'keys/private.key';
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
