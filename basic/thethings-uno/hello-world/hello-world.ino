#include "LoRa.h"

LoRa LoRa_Lib;
int counter = 0;

void setup() 
{
  Serial.begin(115200);
  Serial1.begin(57600);
  
  delay(2000);
  Serial.println("Configuring LoRa module...");
  LoRa_Lib.LoRaConfig();
  Serial.println("LoRa module ready.");
}

void loop() {
  counter++;
  String baseData = "hi-";
  String dataToSend = baseData + counter;
  LoRa_Lib.LoRaSendAndReceive(dataToSend);
  Serial.println("LoRa status:");
  delay(30000);
}
