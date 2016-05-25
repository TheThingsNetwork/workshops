# The Things Network Example Integration with AWS IoT

This is an example integration of The Things Network with AWS IoT.

[Learn more about AWS IoT](https://aws.amazon.com/iot/how-it-works/)

## Setup

1. Make sure you have your device(s) registered in AWS IoT

2. Copy your device's certificate to `./devices/<DevEUI>.crt` and the private key to `./devices/<DevEUI>.key`

3. If you're using your own CA, replace `./devices/root-CA.crt`

# Install

Install the NPM packages:
```
npm install
```

Configure the example by setting your AppEUI and App Access Key:
```
const mqttBroker = 'staging.thethingsnetwork.org'
const appEUI = '<insert your AppEUI>';
const appAccessKey = '<insert your App Access Key>';
const keyPath = './devices';
const region = 'eu-west-1';
```

# Run

```
node .
```
