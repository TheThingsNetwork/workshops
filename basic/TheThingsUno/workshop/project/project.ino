#define DEBUG

#include "TheThingsUno.h"

const byte devAddr[4] = { /* copy DevAddr here */ };
const byte appSKey[16] = { /* copy AppSKey here */ };
const byte nwkSKey[16] = { /* copy NwkSKey here */ };

#define debugSerial Serial
#define loraSerial Serial1

// These are the input pins of your sensors
#define TEMPSENSOR 0
#define SOILSENSOR 1

TheThingsUno ttu;

void setup()
{
  debugSerial.begin(115200);
  loraSerial.begin(57600);

  ttu.init(loraSerial, debugSerial);
  ttu.reset();
  ttu.personalize(devAddr, nwkSKey, appSKey);

#ifdef DEBUG
  ttu.debugStatus();
#endif
  
  Serial.println("Setup for The Things Network.");
}

void loop() {
  while (debugSerial.available())
    loraSerial.write((char)debugSerial.read());

  while (loraSerial.available())
    debugSerial.write((char)loraSerial.read());

  delay(1);
}
