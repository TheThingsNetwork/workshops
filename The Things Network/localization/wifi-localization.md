# WiFi localization

## Important things

When connecting the WiFi chip to the Arduino.
Arduino Tx pin: 5
Arduino Rx pin: 13


## Set the ESP module to the right baud rate

Most of the ESP modules are preconfigured with a baud rate of 115200. Unfortunately, this is too high for The Things Uno to communicate with (the Rx can handle a baud rate up to 57600). Therefore, we first have to connect the ESP modules directly to our laptop to lower the rate.

To set the baud rate some extra hardware is needed. For our use case, an FTDI programmer can be used. With the FTDI it is possible to connect the ESP directly to a laptop and issue commands to change the baud rate.

Here is how you connect the FTDI to the ESP module:

![Connect](https://ttnstaticfile.blob.core.windows.net/media/django-summernote/2018-04-20/5cadb4ec-ce55-4b2b-aaee-e6704381fbdd.png)

When the ESP is connected to your pc through the FTDI programmer, you can communicate with the module. This can be done via the Serial Monitor of the Arduino:

* Open the **Serial Monitor** (you can open this, even if you didn't upload a sketch beforehand)
* Experiment with different baud rates until you found the one that works (which will probably be 115200 or 57600), you can do so by:
 * Selecting a baud rate at the right bottom of the Serial Monitor
 * Type in `at`, if the Serial Monitor prints `ok` you have found the baud rate the module is currently using
* Set the baud rate to 9600 via the command: `AT+UART_DEF=9600,8,1,0,0`


## Connect and configure

How to connect your hardware together:

* VCC to 3.3V
* CH_PD to 3.3V
* GND to GND
* RX to pin 5
* TX to pin 13


Next up (after creating an application and registering your device in [The Things Network Console](https://console.thethingsnetwork.org/)) is creating the Arduino Sketch:

* [Arduino Sketch](wifi-localization.ino)


## Payload Format

Add the code below to change the raw payload to a humand-readable text in the Payload Format.

```
function pad(b) {
  var h = b.toString(16);
  return (h + "").length < 2 ? "0" + h : h;
}

function Decoder(bytes, port) {
  var result = {
    access_points: []
  };
  for (var i = 0; i < bytes.length; i += 7) {
    var bssid = bytes.slice(i, i + 6).map(pad).join(":");
    var rssi = bytes[i + 6] - 0x100;
    result.access_points.push({
      bssid: bssid,
      rssi: rssi
    });
  }
  return result;
}
```

## From BSSIDs to GPS

There are two options for doing so.

### Option 1: Integrate with Collos

* [Tutorial on the Collos integration](https://www.thethingsnetwork.org/docs/applications/collos/)


### Option 2: Use the Google API

* [Tutorial on Google Integration](https://github.com/lorawan-academy/week7/tree/master/app)
