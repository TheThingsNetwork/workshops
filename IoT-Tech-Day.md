# IOT HACKATHON

![IoT_Tech_Day](media/ITD_rgb.jpg)


## Hack the world of IoT with the Things Network

The Things Network (TTN) is back at the IoT Tech Day with a new, bigger hackathon. Novel Internet of Things technology and abundant data connectivity open up a new world of opportunities. We challenge the tech-savvy and the creative minded people to start exploring the new world of IoT and change the way we use technology. During the hackathon we will use the open standard LoRaWAN.
 
## About The Things Network
The Things Network is on a mission to build an open, global and crowdsourced Internet of Things data network, owned and operated by its users. In July 2015, The Things Network managed to cover the entire city of Amsterdam with a new type of wireless technology called LoRaWAN (Long Range Wide Area Network). The network was built, bottom-up, funded by its users. Now, hundreds of cities in over 80 countries worldwide are covered and lots of use cases are built on top of it. Join us in our mission to create abundant data connectivity so applications and businesses can flourish.

### Your The Things Network Support Team
Face any difficulties during the Hackathon? Ask for these guys who can help you out:
![Library Manager](media/TTN-team.png)



## Useful links
* [The Things Network Console](https://console.thethingsnetwork.org/) (developer environment)
* [SODAQ One support page](http://support.sodaq.com/sodaq-one/)
* [Official TTN documentation](https://www.thethingsnetwork.org/docs/)


## Getting started with SODAQ ONE
This workshop will support you in setting-up your SODAQ ONE board and send data over The Things Network to the IBM Watson IoT Platform.

### Pre-requisites

1. SODAQ ONE board 
2. Active GPS antenna
3. Antenna 
4. Micro-USB cable
5. LiPo battery 240mAh
6. Headers Sensors
7. Computer running Windows 7 or higher, Mac OS X or Linux
8. Wifi for your laptop.


### Register with The Things Network and join Slack

To use the console, you need an account.

1. [Create an account][https://account.thethingsnetwork.org/register].
2. Once your account has been created, go to your [profile page](https://account.thethingsnetwork.org/) and `request a Slack invite`
3. A mail has been sent with an invitation to join Slack. Open up Slack and join our dedicated channel `iot-tech-day`



## Set up your Arduino IDE
Set up the Arduino Software (IDE) and connect to your SODAQ One.

1.  [Download](https://www.arduino.cc/en/Main/Software) and install the latest version of the Arduino Software.
2.  Navigate to **Sketch > Include Library > Manage Libraries...**.
3.  Search for **TheThingsNetwork**, select version **2.5.2** and click the result to select it.
4.  Click the **Install** button which should appear:

    ![Library Manager](media/arduino_library.png)

5.  Install the SODAQ ONE board files. Click on **File **(or **Arduino** on a MAC) **> Preferences** and at the bottom you should see **Additional Boards Manager URLs**. Paste the following URL and click **OK** 
 ```
 http://downloads.sodaq.net/package_sodaq_samd_index.json
 ```
6.  Click on **Tools > Board > Boards Manager...**.
7. Search for **SODAQ SAMD Boards**, select the latest version and click and **Install**

 ![Library Manager](media/arduino_librarySODAQ.png)

8. We’re almost there! Now go to **Tools > Board** and select **SODAQ ONE** at the bottom. 



### Add an Application in the Console

Add your first The Things Network Application.

1.  In the [console][https://console.thethingsnetwork.org/], click [add application][https://console.thethingsnetwork.org/applications/add].

	* For **Application ID**, choose a unique ID of lower case, alphanumeric characters and nonconsecutive `-` and `_` (e.g. `hi-world`).
	* For **Application Description**, enter anything you like (e.g. `Hi, World!`).

	![Add Application](media/add_application.png)

2.  Click **Add application** to finish.

    You will be redirected to the newly added application, where you can find the generated **Application EUI** and default **Access Key** which we'll need later.
    
    > If the Application ID is already taken, you will end up at the Applications overview with the following error. Simply go back and try another ID.
    
    ![ID exists](media/id-exists.png)    

### Register the Device

The Things Network supports the two LoRaWAN mechanisms to register devices: Over The Air Activation (OTAA) and Activation By Personalization (ABP). In this workshop, we will use ABP.

> In production, you'll want to use OTAA, which is the default. This is more reliable because the activation will be confirmed and more secure because the session keys will be negotiated with every activation. ABP is useful for workshops because you don't have to wait for a downlink window to become available to confirm the activation.

1.  On the Application screen, scroll down to the **Devices** box and click **register device**.

    * For **Device ID**, choose a - for this application - unique ID of lower case, alphanumeric characters and nonconsecutive `-` and `_` (e.g. `my-sodaq-one`).
    * For **Device EUI**, click the **randomize** button. <img src="media/randomize.png" height="20">

    ![Register Device (OTAA)](media/register_device.png)

2.  Click **Register**.

    You will be redirected to the newly registered device.
    
3.  On the device screen, select **Settings** from the top right menu.

    ![switch-abp](media/switch_abp.png)

    * You can give your device a description like `My SODAQ ONE - IoT Tech Day`
    * Change *Activation method* to *ABP*.
    * Uncheck **Frame counter checks** at the bottom of the page.

        > **Note:** This allows you to restart your device for development purposes without the routing services keeping track of the frame counter. This does make your application vulnerable for replay attacks, e.g. sending messages with a frame counter equal or lower than the latest received. Please do not disable it in production.

4.  Click **Save** to finish.

    You will be redirected to the device, where you can find the **Device Address**, **Network Session Key** and **App Session Key** that we'll need next.
    
    ![device-info](media/device_info_abp.png)



## Send a Message

Activate your device and send your first byte to verify that it works.

### Configure

1. Copy and Paste the following code to your Arduino IDE

```
#include <TheThingsNetwork.h>

// Set your DevAddr, NwkSKey, AppSKey and the frequency plan
const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial SERIAL_PORT_MONITOR

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

void setup()
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("-- PERSONALIZE");
  ttn.personalize(devAddr, nwkSKey, appSKey);

  debugSerial.println("-- STATUS");
  ttn.showStatus();
}

void loop()
{
  debugSerial.println("-- LOOP");

  // Prepare payload of 1 byte to indicate LED status
  byte payload[1];

  // Send it off
  ttn.sendBytes(payload, sizeof(payload));

  delay(10000);
}
```


2.  Set the values for `devAddr`, `nwkSKey` and `appSKey` using the information from the device in the console. Use the 📋 buttons next to fields to copy their (hidden) value.
   
    * For `devAddr ` use the **Device Address**.
    * For `nwkSKey ` use the **Network Session Key**.
    * For `appSKey` use **App Session Key**.


### Upload

1.  Select **Sketch > Upload** `Ctrl/⌘ U` to upload the sketch.
 
    Wait for the status bar to say *Done uploading*.
 
2.  Select **Tools > Serial Monitor** `Ctrl/⌘ Shift M` to open the Serial Monitor.

    Soon, you should see something like this:

    ```
    Sending: mac tx uncnf 1 010203
    Successful transmission
    ```

### Monitor

From the device or application in the console, select **Data** in the top right menu. You should soon see the messages come in. Click on the blue ▶ to see all data:

![messages-test](media/messages_test.png)

As you can see you are sending 1 byte. In the sketch you have uploaded you can find we do this in the [`loop()`](https://www.arduino.cc/en/Reference/Loop) function:

```c
void loop() {
  byte payload[1];
  payload[0] = (digitalRead(LED_BUILTIN) == HIGH) ? 1 : 0;

  // Send it off
  ttn.sendBytes(payload, sizeof(payload));
}
```

## OK. Done. What's Next?

Instead of sending 1 byte, it's time to get real!

Our proposed challenges to you:
* Start sending real data (GPS Coordinates, Time Stamp, Battery Voltage, Board Temperature, Speed, etc.)
  *You might need to go outside to get a GPS fix)*
* Decode the Payload in the Console
* Display the data in myDevices
* Link The Things Network to IBM Watson IoT



