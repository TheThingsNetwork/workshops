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

## Deploy

1. In the [Azure Portal](https://portal.azure.com), create a new **Web App**
2. In the Settings of your newly deployed App Service, go to **Deployment source** and select **Local Git Repository** or other Git enabled source
3. In **Deployment credentials**, make sure that your credentials are valid
4. Run `git clone <url>` to clone the repository using the url specified under **Git clone url** to a local folder
5. Run `npm init` to configure the App Service for Node.js. As **entry point**, enter `server.js`
6. Run `npm install --save ttn-azure-iothub` to install this package
7. Create a new file named `iisnode.yml` and add the line `loggingEnabled: true` to enable logging
8. Copy the example from above and set your AppEUI, App Access Key, Azure IoT Hub name, key name and key
9. Run `npm start` to verify that the integration works locally
10. Run `git add server.js package.json iisnode.yml` to add the required files
11. Run `git commit -m "Initial commit"`
12. Run `git push` to push all changes and deploy your App Service
13. In the Azure Portal, go to your App Service, go to **Tools** and open the **Log stream**. Typical logging output looks as follows:
```
0004A30B001B442B: Handling uplink
Uplink { devEUI: '0004A30B001B442B',
  data: '{"lux":57,"temperature":19.33,"deviceId":"0004A30B001B442B","time":"2016-06-12T18:11:27.56550825Z"}' }
```