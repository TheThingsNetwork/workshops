# The Things Network Workshop: MultiTech mDot and EVB using ARM mbed

## Introduction

This workshop walks you through the process of building a LoRa enabled
internet-of-things application, from device to end-user application.

The device will be powered by the MultiTech mDot using the ARM mbed development environment. We will be using Node RED and IFTTT to program your cloud application.

## Pre-requisites

- LoRaWAN gateway connected to The Things Network
- MultiTech mDot with UDK2, or MultiTech EVB
- Micro USB cable (2x for EVB)
- ARM mbed developer account

## Setting up The Things Network

Today, we're using the brand new staging environment of The Things Network.

- MQTT broker: `staging.thethingsnetwork.org:1883` (username is the AppEUI and password is the App Access Key)
- Node RED: `http://workshop.thethingsnetwork.org`

### Basic: default settings

AppEUI in hex: `0102030405060708`, in C: `{ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 }`

AppSKey and NwkSKey in hex: `2B7E151628AED2A6ABF7158809CF4F3C`, in C: `{0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C}`

App Access Key: `YaocEnL5ywOuATDtp6Zhg0b/YSF9MBho25IftsXstUY=`

### Advanced: create your own account and application

See [Getting Started with ttnctl](http://staging.thethings.network/wiki/Backend/ttnctl/QuickStart).

## Getting Started

### MultiTech mDot and EVB demo

**EVB**

1. Connect the EVB to the mDot with the flat cable
2. Connect both the EVB and the mDot to your pc
3. Set the EVB in configuration mode

**mDot AT firmware**

[Download mDot AT command firmware](ftp://ftp.multitech.com/wireless/mtdot/mdot-firmware-1.0.8.bin)

[See documentation](./resources/multitech-mdot.pptx)

```
Type “AT+FSB=7”
Type “AT+PN=1”
Type “AT+NI=0,<AppEUI>”
Type “AT+NK=0,<AppKey>”
Type “AT&V”
Type “AT&W”
Type “ATZ”
```

### MultiTech mDot custom application

1. Plug the mDot on the UDK
2. Login to the ARM mbed compiler
3. Import the (Hello world sample)[https://developer.mbed.org/teams/The-Things-Network/code/mDot_Workshop/]
4. Compile and copy the file to your mDot

### Node RED

1. Go to [Node RED](http://workshop.thethingsnetwork.org)
2. Import the `MQTT/All devices` node from Menu -> Import -> Library
3. For the EVB: import the `EVB/Parse` node from Menu -> Import -> Library
4. Add a debug node
5. Connect the nodes
6. Press Deploy

## Resources

- [Installing Node RED](http://nodered.org/docs/getting-started/installation.html)
- [IFTTT Maker Channel](https://ifttt.com/maker)
- Grove examples
  - Slide pot: https://developer.mbed.org/components/Grove-Slide-Potentiometer/
  - Light sensor: https://developer.mbed.org/components/Grove-Seeed-Light-Sensor/
  - Touch sensor: https://developer.mbed.org/components/Grove-Touch-Sensor/
  - Ultrasonic Ranger: https://developer.mbed.org/components/Grove-Ultrasonic-Ranger/
