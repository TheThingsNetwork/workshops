#include <TheThingsNetwork.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>
TinyGPS gps;
SoftwareSerial ss(8, 9);

// Set your AppEUI and AppKey 
const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);
static void print_int(unsigned long val, unsigned long invalid, int len);
static void print_date(TinyGPS &gps);
static void print_str(const char *str, int len);

void setup() 
{
  loraSerial.begin(57600);
  debugSerial.begin(9600);
  ss.begin(9600);
  
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;
  
  debugSerial.println("-- NETWORK SETUP");
  ttn.personalize(devAddr, nwkSKey, appSKey);
  debugSerial.println("-- SETUP DONE");
  
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


  byte payload[8];
  payload[0] = lat >> 24;
  payload[1] = lat >> 16;
  payload[2] = lat >> 8;
  payload[3] = lat;
  
  payload[4] = lon >> 24;
  payload[5] = lon >> 16;
  payload[6] = lon >> 8;
  payload[7] = lon;
  
  ttn.sendBytes(payload, sizeof(payload));

  delay(10000);
}
