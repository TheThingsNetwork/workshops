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
3. Import the [Hello world sample](https://developer.mbed.org/teams/The-Things-Network/code/mDot_Workshop/)
4. Compile and copy the file to your mDot

### Node RED

1. Go to [Node RED](http://workshop.thethingsnetwork.org)
2. Import the `MQTT/All devices` node from Menu -> Import -> Library
3. For the EVB: import the `EVB/Parse` node from Menu -> Import -> Library
4. Add a debug node
5. Connect the nodes
6. Press Deploy

**Flow**

```
[{"id":"5d8ff017.a2701","type":"mqtt-broker","z":"3d8e136c.c271ec","broker":"croft.thethings.girovito.nl","port":"1883","clientid":"","usetls":false,"verifyservercert":true,"compatmode":true,"keepalive":"60","cleansession":true,"willTopic":"","willQos":"0","willRetain":"false","willPayload":"","birthTopic":"","birthQos":"0","birthRetain":"false","birthPayload":""},{"id":"a10d34e2.5ef2c8","type":"mqtt in","z":"3d8e136c.c271ec","name":"","topic":"nodes/AA0000FF/packets","broker":"5d8ff017.a2701","x":649,"y":56,"wires":[["862b405c.79d4c"]]},{"id":"86e5ad42.791a5","type":"debug","z":"3d8e136c.c271ec","name":"","active":true,"console":"false","complete":"true","x":725,"y":402,"wires":[]},{"id":"862b405c.79d4c","type":"json","z":"3d8e136c.c271ec","name":"","x":688,"y":140,"wires":[["9f707ffe.608f8"]]},{"id":"9f707ffe.608f8","type":"function","z":"3d8e136c.c271ec","name":"Decode","func":"var data = new Buffer(msg.payload.data, 'base64');\n\nvar p = [];\nfor (var i = 0; i < data.length; i++)\n  p.push(data[i]);\n\nreturn{\n    payload: p\n}\n","outputs":1,"noerr":0,"x":661,"y":219,"wires":[["7e831941.817ce8"]]},{"id":"7e831941.817ce8","type":"function","z":"3d8e136c.c271ec","name":"Parse EVB payload","func":"var data_type = {\n\tnone : 0x00,\n\tled1 : 0x01,\n\tled2 : 0x02,\n\tlux_max : 0x03,\n\tlux_min : 0x04,\n\tlux_curr : 0x05,\n\tbaro_max : 0x06,\n\tbaro_min : 0x07,\n\tbaro_curr : 0x08,\n\ttemp_max : 0x09,\n\ttemp__min : 0x0A,\n\ttemp_curr : 0x0B,\n\taccel_max : 0x0C,\n\taccel_min : 0x0D,\n\taccel_curr : 0x0E,\n\tconfiguration : 0x0F,\n\tgpio_in : 0x10,\n\tgpio_out : 0x11,\n\tcurrent_max : 0x12,\n\tcurrent_min : 0x13,\n\tcurrent_curr : 0x14,\n\tgps_latitude : 0x15,\n\tgps_longitude : 0x16,\n\tgps_time : 0x17,\n\tgps_date : 0x18,\n\tgps_lock : 0x19,\n\tqos_up : 0x1A,\n\tqos_dwn : 0x1B,\n\trf_out : 0x1C,\n\tdata_mark : 0x1D,\n};\n\nvar data_struc = {\n\tdata_valid : 0,\n\tblock_start :0,\n\ttemperature : 0,\n\tx_pos : 0,\n\ty_pos : 0,\n\tz_pos : 0,\n\tbaro_pressure : 0,\n\tlux : 0,\n\tpkt_timer :0,\n\trf_pwr : 0,\n\tsf_val : 0,\n\trssi_up : 0,\n\tsnr_up : 0,\n\trssi_dwn : 0,\n\tsnr_dwn :0 ,\n\tlat_deg : 0 ,\n\tlat_min : 0,\n\tlong_deg : 0,\n\tlong_min : 0,\n\tnum_sats : 0 ,\n\tgps_status : 0,\n};\n\ncontext.global.data_out = context.global.data_out || data_struc;\nvar pData = context.global.data_out;\n\nvar msg_pntr = 0;\nvar temp = 0;\n\nwhile (msg_pntr < msg.payload.length){\n\tswitch (msg.payload[msg_pntr]){\n\tcase data_type.lux_max:\n\tcase data_type.lux_min:\n\tcase data_type.lux_curr:\n\t\tpData.lux = msg.payload[++msg_pntr] << 8 \n\t\tpData.lux |= msg.payload[++msg_pntr];\n\t\tpData.lux = pData.lux * 0.24;\n\t\tmsg_pntr++;\n\t\tbreak;\n\tcase data_type.baro_max:\n\tcase data_type.baro_min:\n\tcase data_type.baro_curr:\n\t\tpData.baro_pressure = msg.payload[++msg_pntr]<<16;\n\t\tpData.baro_pressure |= msg.payload[++msg_pntr]<<8;\n\t\tpData.baro_pressure |= msg.payload[++msg_pntr];\n\t\tpData.baro_pressure = pData.baro_pressure * 0.25;\n\t\tmsg_pntr++;\n\t\tbreak;\n\tcase data_type.accel_max:\n\tcase data_type.accel_min:\n\tcase data_type.accel_curr:\n\t\tpData.x_pos = ((msg.payload[++msg_pntr] << 24) >> 24) * 0.0625;\n\t\tpData.y_pos = ((msg.payload[++msg_pntr] << 24) >> 24) * 0.0625;\n\t\tpData.z_pos = ((msg.payload[++msg_pntr] << 24) >> 24) * 0.0625;\n\t\tmsg_pntr++;\n\t\tbreak;\n\tcase data_type.temp_min:\n\tcase data_type.temp_max:\n\tcase data_type.temp_curr:\n\t\tpData.temperature = msg.payload[++msg_pntr] << 24;\n\t\tpData.temperature |= msg.payload[++msg_pntr] << 16;\n\t\tpData.temperature = (pData.temperature >> 16) * .0625;\n\t\tmsg_pntr++;\n\t\tbreak;\n\tcase data_type.configuration:\n\t\tpData.pkt_timer = msg.payload[++msg_pntr];\n\t\tmsg_pntr++\n\t\tbreak;\n\tcase data_type.current_max:\n\tcase data_type.current_min:\n\tcase data_type.current_curr:\n\t\tmsg_pntr++;\n\t\tmsg_pntr++;\n\t\tmsg_pntr++;\n\t\tbreak;\n\tcase data_type.gps_latitude:\n\t\tpData.lat_deg = (msg.payload[++msg_pntr] << 24) >> 24;\n\t\tpData.lat_min = msg.payload[++msg_pntr];\n\t\ttemp = msg.payload[++msg_pntr] << 8 \n\t\ttemp |= msg.payload[++msg_pntr];\n\t\tpData.lat_min = pData.lat_min + (temp * 0.0001);\n\t\tmsg_pntr++;\n\t\tbreak;\n\tcase data_type.gps_longitude:\n\t\tpData.long_deg = (msg.payload[++msg_pntr] << 24);\n\t\tpData.long_deg |= (msg.payload[++msg_pntr] << 16);\n\t\tpData.long_deg = pData.long_deg >> 16;\n\t\tpData.long_min = msg.payload[++msg_pntr];\n\t\ttemp = msg.payload[++msg_pntr] << 8 \n\t\ttemp |= msg.payload[++msg_pntr];\n\t\tpData.long_min = pData.long_min + (temp * 0.0001);\n\t\tmsg_pntr++;\n\t\tbreak;\n\tcase data_type.gps_time:\n\t\tmsg_pntr++;\n\t\tmsg_pntr++;\n\t\tmsg_pntr++;\n\t\tmsg_pntr++;\n\t\tbreak;\n\tcase data_type.gps_date:\n\t\tmsg_pntr++;\n\t\tmsg_pntr++;\n\t\tmsg_pntr++;\n\t\tmsg_pntr++;\n\t\tbreak;\n\tcase data_type.gps_lock:\n\t\tmsg_pntr++;\n\t\tpData.gps_status = msg.payload[msg_pntr] & 0x0F;\n\t\tpData.num_sats = msg.payload[msg_pntr++] >> 4;\n\t\tbreak;\n\tcase data_type.qos_up:\n\t\tpData.rssi_up = msg.payload[++msg_pntr] << 24;\n\t\tpData.rssi_up |= msg.payload[++msg_pntr] << 16;\n\t\tpData.rssi_up = pData.rssi_up >> 16;\n\t\tpData.snr_up = (msg.payload[++msg_pntr] << 24) >> 24;\n\t\tpData.rf_pwr = (msg.payload[++msg_pntr] << 24) >> 24;\n\t\tmsg_pntr++;\n\t\tbreak;\n\tcase data_type.qos_dwn:\n\t\tpData.rssi_dwn = msg.payload[++msg_pntr] << 24;\n\t\tpData.rssi_dwn |= msg.payload[++msg_pntr] << 16;\n\t\tpData.rssi_dwn = pData.rssi_dwn >> 16;\n\t\tpData.snr_dwn = ((msg.payload[++msg_pntr] << 24) >> 24) / 4;\n\t\tmsg_pntr++;\n\t\tbreak;\n\tcase data_type.rf_out:\n\t\tpData.rf_pwr = (msg.payload[++msg_pntr] << 24) >> 24;\n\t\tmsg_pntr++;\n\t\tbreak;\n\tcase data_type.data_mark:\n\t\tif (msg_pntr == 0) {\n\t\t\tpData = data_struc;\n\t\t\tpData.block_start = 1;\n\t\t\tmsg_pntr++;\n\t\t}\n\t\telse if (msg_pntr == (msg.payload.length - 1) && (pData.block_start === 1)) {\n\t\t\t\tpData.data_valid = 1;\n\t\t\t\tmsg_pntr++;\n\t\t\t}\n\t\t\telse {\n\t\t\t\tpData = data_struc;\n\t\t\t\tmsg_pntr = msg.payload.length;\n\t\t\t\t}\n\t\tbreak;\n\tdefault:\n\t\tpData = data_struc;\n\t\tmsg_pntr = msg.payload.length;\n\t}\n}\n\ncontext.global.data_out = pData;\n\nreturn pData;","outputs":1,"noerr":0,"x":734,"y":306,"wires":[["86e5ad42.791a5"]]}]
```

## Resources

- [Installing Node RED](http://nodered.org/docs/getting-started/installation.html)
- [IFTTT Maker Channel](https://ifttt.com/maker)
- Grove examples
  - Slide pot: https://developer.mbed.org/components/Grove-Slide-Potentiometer/
  - Light sensor: https://developer.mbed.org/components/Grove-Seeed-Light-Sensor/
  - Touch sensor: https://developer.mbed.org/components/Grove-Touch-Sensor/
  - Ultrasonic Ranger: https://developer.mbed.org/components/Grove-Ultrasonic-Ranger/
