#define DEBUG

#include "TheThingsUno.h"

const byte devAddr[4] = { 0x02, 0xDE, 0xAE, 0x00 };
const byte appSKey[16] = { 0x0D, 0x0E, 0x0A, 0x0D, 0x0B, 0x0E, 0x0E, 0x0F, 0x0C, 0x0A, 0x0F, 0x0E, 0x0B, 0x0A, 0x0B, 0x0E };
const byte nwkSKey[16] = { 0x0D, 0x0E, 0x0A, 0x0D, 0x0B, 0x0E, 0x0E, 0x0F, 0x0C, 0x0A, 0x0F, 0x0E, 0x0B, 0x0A, 0x0B, 0x0E };

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
