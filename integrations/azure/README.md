# The Things Network Azure IoT Hub Integration

This is an example integration of The Things Network with Azure IoT Hub. This integration features creating devices in the Azure IoT Hub device registry as well as sending events from uplink messages.

## Example

This integration requires an shared access policy key name with Registry write and Device connect permissions. In this example, we use the **iothubowner** policy which has these permissions enabled by default.

```js
'use strict';

const http = require('http');
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

http.createServer((req, res) => {
    res.writeHead(200, { 'Content-Type': 'text/html' });
    res.end('The Things Network Azure IoT Hub Integration is running');
}).listen(process.env.PORT || 8080);
```

## Deploy

1. In the [Azure Portal](https://portal.azure.com), create a new **Empty Node JS Web App**
2. In the Settings of your newly deployed App Service, go to **Deployment source** and select **Local Git Repository** or other Git enabled source
3. In **Deployment credentials**, make sure that your credentials are valid
4. Run `git clone <url>` to clone the repository using the url specified under **Git clone url** to a local folder
5. Run `npm install --save ttn-azure-iothub` to install this package
6. Create a new file named `iisnode.yml` and add the line `loggingEnabled: true` to enable logging
7. Copy the example from above and set your AppEUI, App Access Key, Azure IoT Hub name, key name and key
8. Remove the entry `"engines" : { ... }` from `package.json` to let Azure select the Node.js version 
9. Run `git add .` to stage the changed files
10. Run `git commit -m "Initial commit"`
11. Run `git push` to push all changes and deploy your App Service
12. Open the URL of your App Service and verify that it prints that it runs
13. In the Azure Portal, go to your App Service, go to **Tools** and open the **Log stream**. Typical logging output looks as follows:
```
0004A30B001B442B: Handling uplink
Uplink { devEUI: '0004A30B001B442B',
  data: '{"lux":57,"temperature":19.33,"deviceId":"0004A30B001B442B","time":"2016-06-12T18:11:27.56550825Z"}' }
```

You are now ready to process your data in a Stream Analytics job.