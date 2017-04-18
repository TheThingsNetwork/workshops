# IOT HACKATHON
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
3.  Search for **TheThingsNetwork** and click the result to select it.
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


## Connect to The SODAQ One


Our friends at SODAQ already wrote elaborate documentation on how to setup your SODAQ One and connect it to The Things Network. 
