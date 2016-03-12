#include "LoRa.h"

#define DevAddr "02DEAE00"
#define NwkSKey "2B7E151628AED2A6ABF7158809CF4F3C"
#define AppSKey "2B7E151628AED2A6ABF7158809CF4F3C"

LoRa lora;
int counter = 0;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(57600);

  delay(2000);
  Serial.println("Configuring LoRa module...");
  lora.Config(DevAddr, NwkSKey, AppSKey);
  Serial.println("LoRa module ready.");
  delay(3000);
}

void loop() {
  counter++;
  String baseData = "hi-";
  String message = baseData + counter;
  lora.Send(message);
  delay(10000);
}
