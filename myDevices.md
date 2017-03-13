# Cayenne myDevices integration

myDevice allows you to quickly design, prototype, and visualize IoT solutions. You can use myDevices to build a nice-looking dashboard that showcases your data nicely.

![myDevices-dashboard](media/dashboard-mydevices.png)


## Setup your myDevices account

1.  Create an account on [myDevices](https://mydevices.com/)
2.  Log-in and click on **LoRa**
	
	<img src="media/myDevices-lora.png" width="130">
3.  Select **The Things Network** at the bottom of the left menu bar, click on **Cayenne LPP** and fill in your **DevEUI** of your device (which you can find in the [**Console**](https://console.thethingsnetwork.org/applications))
	![add-device](media/cayenne-add-device.png)


## Add the myDevices integration in the Console

1.  Go to your application in the [**Console**](https://console.thethingsnetwork.org/applications) and add the myDevices integration via **Add Integration**
	![myDevices-dashboard](media/integrations.png) 

2.  You can find your **Process ID** in the URL of the myDevices dashboard, starting after `/lora/`
3.  Add the integration


## Change the payload format 

In order to display your content in the myDevices dashboard, we need to change the format of the payload in the Arduino Sketch.

> We need to send extra data for myDevices to understand what data comes into their dashboard. Before we send the sensor data, we need to define what data is sent. The first byte is the so-called **Channel ID**. The sencond bytes explains the **Data Type** (so myDevices knows that the data contains temperature values). The latter bytes contain the actualy sensor values.
 Please have a look [here](https://mydevices.com/cayenne/docs/#lora-cayenne-low-power-payload) to find more information.
 
 
*  Add the following code at the beginning of the Arduino sketch:

```
#define LPP_DIGITAL_INPUT       0       // 1 byte
#define LPP_DIGITAL_OUTPUT      1       // 1 byte
#define LPP_ANALOG_INPUT        2       // 2 bytes, 0.01 signed
#define LPP_ANALOG_OUTPUT       3       // 2 bytes, 0.01 signed
#define LPP_LUMINOSITY          101     // 2 bytes, 1 lux unsigned
#define LPP_PRESENCE            102     // 1 byte, 1
#define LPP_TEMPERATURE         103     // 2 bytes, 0.1°C signed
#define LPP_RELATIVE_HUMIDITY   104     // 1 byte, 0.5% unsigned
#define LPP_ACCELEROMETER       113     // 2 bytes per axis, 0.001G
#define LPP_BAROMETRIC_PRESSURE 115     // 2 bytes 0.1 hPa Unsigned
#define LPP_GYROMETER           134     // 2 bytes per axis, 0.01 °/s
#define LPP_GPS                 136     // 3 byte lon/lat 0.0001 °, 3 bytes alt 0.01 meter

// Data ID + Data Type + Data Size
#define LPP_DIGITAL_INPUT_SIZE       3       // 1 byte
#define LPP_DIGITAL_OUTPUT_SIZE      3       // 1 byte
#define LPP_ANALOG_INPUT_SIZE        4       // 2 bytes, 0.01 signed
#define LPP_ANALOG_OUTPUT_SIZE       4       // 2 bytes, 0.01 signed
#define LPP_LUMINOSITY_SIZE          4       // 2 bytes, 1 lux unsigned
#define LPP_PRESENCE_SIZE            3       // 1 byte, 1
#define LPP_TEMPERATURE_SIZE         4       // 2 bytes, 0.1°C signed
#define LPP_RELATIVE_HUMIDITY_SIZE   3       // 1 byte, 0.5% unsigned
#define LPP_ACCELEROMETER_SIZE       8       // 2 bytes per axis, 0.001G
#define LPP_BAROMETRIC_PRESSURE_SIZE 4       // 2 bytes 0.1 hPa Unsigned
#define LPP_GYROMETER_SIZE           8       // 2 bytes per axis, 0.01 °/s
#define LPP_GPS_SIZE                 11      // 3 byte lon/lat 0.0001 °, 3 bytes alt 0.01 meter
```

*  If you are sending temperature, you are sending four bytes:
  - byte1: Channel ID
  - byte2: Data type
  - byte3: highByte temperature
  - byte4: lowByte temperature
	
	
  to send this, add the following code is added in the `void loop()` and Upload the sketch to The Things Uno.

```
  // Read the temperature
  float celcius = getCelcius(A2);

  // Log the value
  debugSerial.print("Temperature: ");
  debugSerial.println(celcius);

  // Encode float as int (20.98 becomes 2098)
  int16_t celciusInt = round(celcius * 10);
  uint8_t cursor = 0;

// Encode int as bytes
  byte payload[4];
  payload[cursor++] = 0x03; // Channel ID
  payload[cursor++] = LPP_TEMPERATURE; // Data type
  payload[cursor++] = highByte(celciusInt); //celciusInt >> 8;
  payload[cursor++] = lowByte(celciusInt);// celciusInt;
  
  ttn.sendBytes(payload, sizeof(payload));
```

## Build your myDevices dashboard
Click the device on the left side of your dashboard in myDevices and you data is should be shown right away. After a bit of editing you can make it look something like this:

![myDevices-dashboard](media/mydevices-data.png) 


🎉 Great job! Now impress some folks with this amazing dashboard you built on Twitter. *Don't forget to add The Things Network in your Tweet: @thethingsntwrk*
