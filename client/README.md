
# Client Workshop

In this workshop, we'll use our application and devices from the [introductory
workshop](../) and manage them using the [nodejs application
SDK](https://github.com/TheThingsNetwork/node-app-sdk).

このWSでは、[introductory workshop](../) で設定したアプリケーションとデバイスを[nodejs application
SDK](https://github.com/TheThingsNetwork/node-app-sdk)で管理します。

## Pre-requisites

1. A The Things Network application with at least one device, registered in [the
   Console](https://console.thethingsnetwork.org).

   [the Console](https://console.thethingsnetwork.org) に設定した、The Things Networkアプリケーションと最低1つのデバイス
2. [nodejs](https://nodejs.org/en/) installed on your machine.

    ノートPCにインストールした[nodejs](https://nodejs.org/en/)
3. A project folder from which to work, eg. `sdk-workshop`

    プロジェクトフォルダ、 例えば `sdk-workshop`

## Installing the SDK

To install The Things Network node app SDK, run the following command from your project folder:

The Things Networkのノードapp SDKをインストールするには、プロジェクトフォルダから次のコマンドを打ちます

```
npm install ttn
```

This should create a folder called `node_modules` inside the project folder.

プロジェクトフォルダ内に`node_modules`というフォルダができます

## Handle events

To listen to messages from your application, create a script called `server.js` inside the project folder:

アプリケーションからメッセージを待ち受けるためには、プロジェクトフォルダ内に`server.js` というscriptを作成します

```
// server.js
var ttn = require("ttn")

var region = "asia-se"
var appID = "<id>"
var accessKey = "<access_key>"

// create a client
var client = new ttn.Client(region, appID, accessKey)

// listen to messages
client.on("message", function (devID, data) {
  console.log("Message from device", devID)
  console.log(data)
})
```

Where you replace `<app_id>` with the ID of you application (eg. `hi-world`)
and `<access_key>` with the access key of your application.

アプリケーションIDとアクセスキーを入れ替えます

To run this, run:

```
node server.js
```

Whenever you press the button, you should be seeing messages like the following
on the console:

ボタンを押すと、コンソールに以下のようなメッセージが表示されるはずです

```
Message from device my-uno
{ app_id: 'lollercopter',
  dev_id: 'my-uno',
  hardware_serial: '006964B20337B2C1',
  port: 1,
  counter: 11,
  payload_raw: <Buffer 03 cd>,
  payload_fields: { celcius: 9.73 },
  metadata:
   { time: '2017-03-16T16:20:02.370281545Z',
     frequency: 867.1,
     modulation: 'LORA',
     data_rate: 'SF7BW125',
     coding_rate: '4/5',
     gateways: [ [Object] ] } }
```
