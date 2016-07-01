'use strict';

const util = require('util');
const ttn = require('ttn');
const awsiot = require('aws-iot-device-sdk');
const EventEmitter = require('events');

const DEFAULT_TTN_BROKER = 'staging.thethingsnetwork.org';

const Bridge = class Bridge extends EventEmitter {
  constructor(appEUI, appAccessKey, caCert, clientCert, privateKey, region, options) {
    super();
    options = options || {};

    this._createMessage = options.createMessage || function(uplink) {
      return {
        state: {
          reported: uplink.fields
        }
      };
    }

    this.awsClient = awsiot.device({
      caCert: caCert,
      clientCert: clientCert,
      privateKey: privateKey,
      clientId: options.clientId || 'ttn-bridge',
      region: region
    });
    this.awsClient.on('connect', super.emit.bind(this, 'aws-connect'));
    this.awsClient.on('error', super.emit.bind(this, 'error'));

    this.ttnClient = new ttn.Client(options.ttnBroker || DEFAULT_TTN_BROKER, appEUI, appAccessKey);
    this.ttnClient.on('connect', super.emit.bind(this, 'ttn-connect'));
    this.ttnClient.on('error', super.emit.bind(this, 'error'));
    this.ttnClient.on('uplink', this._handleUplink.bind(this));
  }

  _handleUplink(uplink) {
    console.log('%s: Handling uplink', uplink.devEUI);
    const message = JSON.stringify(this._createMessage(uplink));

    super.emit('uplink', { devEUI: uplink.devEUI, message: message });
    this.awsClient.publish(util.format('$aws/things/%s/shadow/update', uplink.devEUI), message);
  }
}

module.exports = {
  Bridge: Bridge
};
