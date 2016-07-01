'use strict';

const ttnawsiot = require('../src');

// Replace with your AppEUI and App Access Key
const appEUI = '70B3D57ED0000002';
const appAccessKey = 'vIfs1MdSwxlDevULfj50xXcblAGXwY7lI7aFkk8CcnI=';

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

bridge.on('uplink', e => {
  console.log('%s: Uplink', e.devEUI, e.message);
});
