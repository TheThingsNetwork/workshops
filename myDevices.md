# Cayenne myDevices integration

myDevice allows you to quickly design, prototype, and visualize IoT solutions. You can use myDevices to build a nice-looking dashboard that showcases your data nicely.

![myDevices-dashboard](media/dashboard-mydevices.png)

## Setup your myDevices account

1.  Create an account on [myDevices](https://mydevices.com/)
2.  Log-in and click on LoRa
	<img src="media/myDevices-lora.png" width="130">
3.  Select **The Things Network** at the bottom of the left menu bar, click on **Cayenne LPP** and fill in your **DevEUI** of your device (which you can find in the [**Console**](https://console.thethingsnetwork.org/applications)
	![add-device](media/cayenne-add-device.png)

## Add the myDevices integration in the Console

1.  Go to your application in the [**Console**](https://console.thethingsnetwork.org/applications) and add the myDevices integration via **Add Integration**
	![myDevices-dashboard](media/integrations.png) 

2.  You can find your **Process ID** in the URL of the browser, starting after `/lora/`
3.  Add the integration

## Change the payload format 
In order to display your content in the myDevices dashboard, we need to change the format of the payload in the Arduino Sketch.

Before the 2 bytes of temperature values, we need to explain myDevices what kind of data we are sending. 

In the first byte we explain that we are sending a analog output that contains two bytes of data. The second byte explains that we are sening temperature as data. The third and fourth byte contains the actual data.

Please have a look [here](https://mydevices.com/cayenne/docs/#lora-cayenne-low-power-payload) to find more information.
