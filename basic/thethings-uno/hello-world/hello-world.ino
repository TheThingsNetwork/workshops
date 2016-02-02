#include <SoftwareSerial.h>

#include "LoRa.h"

LoRa LoRa_Lib;
int counter = 0;

SoftwareSerial loraSerial(10, 11); // RX, TX

void setup() 
{
  Serial.begin(115200);
  
  delay(2000);
  Serial.println("Configuring LoRa module...");
  loraSerial.begin(57600);
  LoRa_Lib.LoRaConfig(loraSerial);
  Serial.println("LoRa module ready.");
}

void loop() {
  counter++;
  String baseData = "hi-";
  String dataToSend = baseData + counter;
  LoRa_Lib.LoRaSendAndReceive(dataToSend, loraSerial);
  Serial.println("LoRa status:");
  delay(30000);
}
