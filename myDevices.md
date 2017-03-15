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
 
 
*  Add the following code at the very beginning of the Arduino sketch:

```
#include <CayenneLPP.h>
```
* Add the following code right before `void setup()` which sets the maximum payload size to 51 bytes.

```
CayenneLPP lpp(51);
```

* The payload we need to send is a bit different than before. You can replace the `void loop()` with the code below and upload the sketch to The Things Uno.

```
void loop() {
  debugSerial.println("-- LOOP");
  
  // Read the temperature
  float celcius = getCelcius(A2);

  lpp.reset();
  lpp.addTemperature(1, celcius);

  ttn.sendBytes(lpp.getBuffer(), lpp.getSize());

  delay(10000);
}
```

## Build your myDevices dashboard
Click the device on the left side of your dashboard in myDevices and you data is should be shown right away. After a bit of editing you can make it look something like this:

![myDevices-dashboard](media/mydevices-data.png) 


🎉 Great job! Now impress some folks with this amazing dashboard you built on Twitter. *Don't forget to add The Things Network in your Tweet: @thethingsntwrk*
