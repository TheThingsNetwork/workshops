# The Things Uno + GPS tutorial


Let's add localization functionality to the ExpLoRer board by connecting a GPS module. This tutorial makes use of the uBlox GPS module which is called the `GY-GPS6MV2`.

This module is quite easy to connect and (same applies to almost all GPS modules) very accurate if used outdoor. The downside of GPS is the price, this module is around €10 in price, positioning is very inaccurate if used indoors and it consumes a lot of energy.




## Connect the GPS to the Arduino:

The GPS module uses 3.3 voltage. When the ExpLoRer board is connected to your computer, the output voltage is 5V. To convert the voltage from 5 to 3.3V, we need to add two resistors, see the image below to find out how to properly connect the GPS.

* GPS Vcc to 3.3V
* GND to GND
* GPS RX to pin 8
* GPS TS to pin 9


### Install libraries


There are several open source GPS libraries available. In this tutorial we're making use of the library called `tinyGPSPlus`.


* You can [**download**](https://github.com/mikalhart/TinyGPS) the `TinyGPS` library and add it via: 
	
	**Sketch** > **Include Library** > **Add .ZIP library**


### Read out GPS data in Serial Monitor

First try reading out the GPS data and print it to the Serial Monital. Once we can properly read out the module we will embed the LoRa connectivity. 

* Below you can find the code to read out the GPS module and to print the data to the Serial Monitor. Do you understand what the code stands for?

```
#include <TinyGPS.h>
#include <SoftwareSerial.h>
TinyGPS gps;
SoftwareSerial ss(8, 9);

#define debugSerial Serial

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);

void setup() 
{
  debugSerial.begin(9600);
  ss.begin(9600);
  
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;
}

void loop() 
{  
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  float flat, flon;
  
  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    debugSerial.print("LAT=");
    debugSerial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    debugSerial.print(" LON=");
    debugSerial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    debugSerial.print(" SAT=");
    debugSerial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    debugSerial.print(" PREC=");
    debugSerial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }

  gps.stats(&chars, &sentences, &failed);
  debugSerial.print(" CHARS=");
  debugSerial.print(chars);
  debugSerial.print(" SENTENCES=");
  debugSerial.print(sentences);
  debugSerial.print(" CSUM ERR=");
  debugSerial.println(failed);
  if (chars == 0)
    debugSerial.println("** No characters received from GPS: check wiring **");


  uint32_t lat = flat * 1000000;
  uint32_t lon = flon * 1000000;

  debugSerial.println(lat);
  debugSerial.println(lon);

  delay(10000);
}
```


If all went well, you should see in your Serial Monitor the data as shown below. Let's walk through the data you see:

* **LAT** - Latitude
* **LON** - Longitude
* **SAT** - Number of satalites
* **PREC** - Precision (the lower the better)
* **CSUM ERR** - Checksum Errors

![GPS-data](https://ttnstaticfile.blob.core.windows.net/media/image_uploader/GPS-data.png)



## LoRa connectivity

Now, let's add the LoRa connectivity. We only need to send the latitude and longitude

A list of to-dos

* Set the basics, like ading the `TheThingsNetwork` library, setting the AppEUI and AppKey, define the loraSerial, define frequency plan etc.
* Create two `uint32_t`'s called `lat` and `lon` and multiply the `flat` and `flon` by 1000000. Remember we don't want to send floats, but integers instead?
* Next is creating 8 bytes
	* 4 bytes for the latitude
	* 4 bytes for the longitude (in this example there is no need to send the number of satalites, precision and errors).
* Send the bytes


Having troubles? check the end of this page to find the source file.


## Payload formats

Put your skills into practrice! We need two new variables called `lat` and `lon`. Both contain 4 bytes, the first byte needs to be shifted 24 placed to the left (4 * 8 bits), the second 16 places etc. Finally, return the two variables. Try it yourself first, having troubles? Check the buttom of this page.




![GPS Console](https://ttnstaticfile.blob.core.windows.net/media/image_uploader/GPS-console.png)



### Final code

* [Arduino Sketch](/GPS_localization.ino)
* [Payload formats](/GPS_payload-format.js)


