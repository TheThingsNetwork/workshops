'use strict';

const util = require('util');
const ttn = require('ttn');
const iothub = require('azure-iothub');
const device = require('azure-iot-device');
const amqp = require('azure-iot-device-amqp');
const common = require('azure-iot-common');
const EventEmitter = require('events');

const DEFAULT_TTN_BROKER = 'staging.thethingsnetwork.org';
const SAK_CONNECTION_STRING = 'HostName=%s.azure-devices.net;SharedAccessKeyName=%s;SharedAccessKey=%s';
const DEVICE_CONNECTION_STRING = 'HostName=%s.azure-devices.net;DeviceId=%%s;SharedAccessKey=%%s';

const Bridge = class Bridge extends EventEmitter {
  constructor(appEUI, appAccessKey, hubName, keyName, key, options) {
    super();
    options = options || {};

    this._createMessage = options.createMessage || function(uplink) {
      const metadata = {
        deviceId: uplink.devEUI,
        time: uplink.metadata.server_time
      };
      return Object.assign({}, uplink.fields, metadata);
    }

    this.registry = iothub.Registry.fromConnectionString(util.format(SAK_CONNECTION_STRING, hubName, keyName, key));
    this.deviceConnectionString = util.format(DEVICE_CONNECTION_STRING, hubName);
    this.devices = {};

    this.ttnClient = new ttn.Client(options.ttnBroker || DEFAULT_TTN_BROKER, appEUI, appAccessKey);
    this.ttnClient.on('connect', super.emit.bind(this, 'ttn-connect'));
    this.ttnClient.on('error', super.emit.bind(this, 'error'));
    this.ttnClient.on('uplink', this._handleUplink.bind(this));
  }

  _getDevice(devEUI) {
    if (this.devices[devEUI]) {
      return Promise.resolve(this.devices[devEUI]);
    }

    return new Promise((resolve, reject) => {
      const device = new iothub.Device(null);
      device.deviceId = devEUI;
      this.registry.create(device, (err, deviceInfo) => {
        if (!err) {
          resolve(deviceInfo);
        } else {
          // The device probably exists
          this.registry.get(device.deviceId, (err, deviceInfo) => {
            if (err) {
              reject(err);
            } else {
              resolve(deviceInfo);
            }
          });
        }
      });
    }).then(deviceInfo => {
      const key = deviceInfo.authentication.SymmetricKey.primaryKey;
      const connectionString = util.format(this.deviceConnectionString, devEUI, key);
      const client = amqp.clientFromConnectionString(connectionString);
      return new Promise((resolve, reject) => {
        client.open(err => {
          if (err) {
            reject(err);
          } else {
            this.devices[devEUI] = client;
            resolve(client);
          }
        });
      });
    });
  }

  _handleUplink(uplink) {
    console.log('%s: Handling uplink', uplink.devEUI);

    this._getDevice(uplink.devEUI).then(deviceInfo => {
      const message = JSON.stringify(this._createMessage(uplink));

      deviceInfo.sendEvent(new device.Message(JSON.stringify(message)), (err, res) => {
        if (err) {
          console.warn('%s: Could not send event: %s. Closing connection', uplink.devEUI, err);
          deviceInfo.close(err => {
            // Delete reference even if close failed
            delete this.devices[uplink.devEUI];
          });
          this.emit('error', err);
        } else {
          this.emit('uplink', { devEUI: uplink.devEUI, message: message });
        }
      });
    })
    .catch(err => {
      console.log('%s: Could not get device: %s', uplink.devEUI, err);
      super.emit('error', err);
    });
  }
}

module.exports = {
  Bridge: Bridge
};