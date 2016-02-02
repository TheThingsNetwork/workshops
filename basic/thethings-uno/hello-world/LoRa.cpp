#include <SoftwareSerial.h>
#include "Arduino.h"
#include "LoRa.h"

void LoRa::LoRaConfig(SoftwareSerial loraSerial)
{
  Serial.print("Reset: ");                                                                                //Device reset
  loraSerial.write("sys reset\r\n");
  delay(1000);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();
                                                                                             //Receive device model
  Serial.print("Device:  ");
  loraSerial.write("sys get ver\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();
  
  Serial.print("Battery: ");                                                                              //Receive voltage feedback
  loraSerial.write("sys get vdd\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();
  
  Serial.print("EUI: ");                                                                                  //
  loraSerial.write("sys get hweui\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();
  
  Serial.print("addr: ");                                                                                 //Set device address for your device!!!!!!!!!!!
  loraSerial.write("mac set devaddr 02010101\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();
  
  Serial.print("nwkskey: ");                                                                              //Set NwkSkey key for your device!!!!!!!!!!!
  loraSerial.write("mac set nwkskey 2B7E151628AED2A6ABF7158809CF4F3C\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();
  
  Serial.print("appskey: ");
  loraSerial.write("mac set appskey 2B7E151628AED2A6ABF7158809CF4F3C\r\n");                                  //Set AppSkey for your device!!!!!!!!!!
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();
  
  Serial.print("adr: ");
  loraSerial.write("mac set adr off\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();
  
  Serial.print("data rate: ");
  loraSerial.write("mac get dr\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();

  Serial.print("ch: ");
  loraSerial.write("mac get ch\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();

  Serial.print("band: ");
  loraSerial.write("mac get band\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();

  
  Serial.print("rx delay1: ");
  loraSerial.write("mac get rxdelay1\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();

  Serial.print("rx delay2: ");
  loraSerial.write("mac get rxdelay2\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();
  
  Serial.print("Pwr: ");                                                                            
  loraSerial.write("mac set pwridx 1\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();
  
  Serial.print("Join: ");
  loraSerial.write("mac join abp\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();

  Serial.print("status: ");
  loraSerial.write("mac get status\r\n");
  delay(50);
  while(loraSerial.available()) Serial.write(loraSerial.read());
  LoRaBlink();
  LoRaBlinkOff();
}

void LoRa::LoRaSendAndReceive(String message, SoftwareSerial loraSerial)
{
    int messageLength = message.length();
    Serial.print("Sending: ");
    Serial.println(message);
    loraSerial.write("mac tx uncnf 1 ");
    
    
    for (int i = 0; i < messageLength; i++)
    {
      loraSerial.print(message.charAt(i), HEX);
      LoRaBlink();  
      
    }
    loraSerial.write("\r\n");
    LoRaBlinkOff();
    delay(100);
    
    Serial.println("RN2483 status:");
    while(loraSerial.available()) 
    {
      Serial.print((char)loraSerial.read());
    }
}

