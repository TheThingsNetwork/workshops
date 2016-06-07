# Access your TTN data in the browser with Socket.io

## Prerequisites

* [Node.JS](https://nodejs.org/)
* An Account and Application in the [Dashboard](https://staging.thethingsnetwork.org/).
* A working device that sends data to The Things Network.

## Getting Started

First install the dependencies:

```
npm install
```

Open `server.js` and change the `appEUI` and `accessKey`. You can find these values by clicking **learn how to get data from this app** in the Dashboard:

```js
var appEUI = '70B3D57ED0000...';
var accessKey = 'SuperSecretAccessKey=';
```

Run the server:

```
node server.js
```

Browse to [localhost:8080](http://localhost:8080/)
