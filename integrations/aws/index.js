'use strict';

const util = require('util');
const ttn = require('ttn');
const awsiot = require('aws-iot-device-sdk');
const EventEmitter = require('events');

const defaultTTNBroker = 'staging.thethingsnetwork.org';

const Bridge = class Bridge extends EventEmitter {
  constructor(appEUI, appAccessKey, caCert, clientCert, privateKey, region, options) {
    super();
    options = options || {};

    this.awsClient = awsiot.device({
      caCert: caCert,
      clientCert: clientCert,
      privateKey: privateKey,
      clientId: options.clientId || 'ttn-bridge',
      region: region
    });
    this.awsClient.on('connect', super.emit.bind(this, 'aws-connect'));
    this.awsClient.on('error', super.emit.bind(this, 'error'));

    this.ttnClient = new ttn.Client(options.ttnBroker || defaultTTNBroker, appEUI, appAccessKey);
    this.ttnClient.on('connect', super.emit.bind(this, 'ttn-connect'));
    this.ttnClient.on('error', super.emit.bind(this, 'error'));
    this.ttnClient.on('uplink', this._handleUplink.bind(this));
  }

  _handleUplink(uplink) {
    var data = {
      state: {
        reported: uplink.fields
      }
    };

    super.emit('uplink', data);
    this.awsClient.publish(util.format('$aws/things/%s/shadow/update', uplink.devEUI), JSON.stringify(data));
  }
}

module.exports = {
  Bridge: Bridge
};
