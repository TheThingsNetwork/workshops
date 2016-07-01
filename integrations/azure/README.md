# The Things Network Azure IoT Hub Integration

This is an example integration of The Things Network with Azure IoT Hub. This integration features creating devices in the Azure IoT Hub device registry as well as sending events from uplink messages.

## Example

This integration requires an shared access policy key name with Registry write and Device connect permissions. In this example, we use the **iothubowner** policy which has these permissions enabled by default.

```js
'use strict';

const ttnazureiot = require('ttn-azure-iothub');

// Replace with your AppEUI and App Access Key
const appEUI = '<insert AppEUI>';
const appAccessKey = '<insert App Access Key>';

// Replace with your Azure IoT Hub name and key
const hubName = '<insert hub name>';
const keyName = 'iothubowner';
const key = '<insert key>';

const bridge = new ttnazureiot.Bridge(appEUI, appAccessKey, hubName, keyName, key);

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
const bridge = new ttnazureiot.Bridge(appEUI, appAccessKey, hubName, keyName, key, options);
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
  const metadata = {
    deviceId: uplink.devEUI,
    time: uplink.metadata.server_time
  };
  return Object.assign({}, uplink.fields, metadata);
}
```

*Note: if there are no payload functions specified for the concerning application, the `fields` object contains a `raw` property with the bytes received.*

*Note: if the result of your payload functions contain the fields `deviceId` or `time`, these fields will be overwritten by the metadata. Use a custom `createMessage` function to use custom field names.*

## Deploying to Azure

### Create Azure WebJob

Follow these steps to deploy an Azure WebJob using Node.js that runs the integration between The Things Network and Azure IoT Hub.

1. In a new folder, run `npm init` to initialize a new WebJob using Node.js. Use `server.js` as entry point
2. Run `npm install --save ttn-azure-iothub` to install this package
3. Create a new file `server.js`, copy the example from above and insert your keys
4. Run `npm start` to verify that the bridge works. This is example output:
```
TTN connected
0004A30B001B442B: Handling uplink
Uplink { devEUI: '0004A30B001B442B',
  message: '{"lux":1000,"temperature":19.82,"deviceId":"0004A30B001B442B","time":"2016-06-14T16:19:15.402956092Z"}' }
0004A30B001B442B: Handling uplink
Uplink { devEUI: '0004A30B001B442B',
  message: '{"lux":1000,"temperature":19.82,"deviceId":"0004A30B001B442B","time":"2016-06-14T16:19:37.546601639Z"}' }
...
```

### Deploy Azure WebJob

1. Compress the WebJob files (including the `node_modules` folder) as ZIP file
2. In the [Azure Portal](https://portal.azure.com), create a new **Web App**
3. Under **Settings**, go to **WebJobs** and click **Add**
4. Enter a name, e.g. **bridge**
5. Select your ZIP file as file to upload
6. Ensure that the type is set to **Continuous**
7. Set the scale to **Single Instance**
8. Click **OK** to deploy the WebJob
9. Once deployed, select the WebJob and click **Logs** to verify that the bridge works. This is example output:
```
[06/14/2016 16:27:47 > 996af8: INFO] TTN connected
[06/14/2016 16:28:07 > 996af8: INFO] 0004A30B001B442B: Handling uplink
[06/14/2016 16:28:10 > 996af8: INFO] Uplink { devEUI: '0004A30B001B442B',
[06/14/2016 16:28:10 > 996af8: INFO]   message: '{"lux":1000,"temperature":19.82,"deviceId":"0004A30B001B442B","time":"2016-06-14T16:28:06.766772461Z"}' }
[06/14/2016 16:28:29 > 996af8: INFO] 0004A30B001B442B: Handling uplink
[06/14/2016 16:28:29 > 996af8: INFO] Uplink { devEUI: '0004A30B001B442B',
[06/14/2016 16:28:29 > 996af8: INFO]   message: '{"lux":1000,"temperature":19.82,"deviceId":"0004A30B001B442B","time":"2016-06-14T16:28:28.908965124Z"}' }
```

You are now ready to process your data in an Azure Stream Analytics job.