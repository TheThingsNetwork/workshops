#include "LoRa.h"

// Change this to your own address
#define DEVADDR "02DEAE00"

// These are the input pins of your sensors
#define TEMPSENSOR 0
#define SOILSENSOR 1

LoRa ttn;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(57600);

  delay(2000);
  Serial.println("Configuring...");
  ttn.Config(DEVADDR, DEFAULT_NWKSKEY, DEFAULT_APPSKEY);
  Serial.println("Configured for The Things Network.");
  delay(3000);
}

void loop() {
  // Wait 10 seconds  
  delay(10000);
}
