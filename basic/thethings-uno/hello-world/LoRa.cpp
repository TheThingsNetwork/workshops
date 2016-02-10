#include "Arduino.h"
#include "LoRa.h"

void LoRa::LoRaConfig()
{
  Serial.print("Reset: ");                                                                                //Device reset
  Serial1.write("sys reset\r\n");
  delay(1000);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();
                                                                                             //Receive device model
  Serial.print("Device:  ");
  Serial1.write("sys get ver\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();
  
  Serial.print("Battery: ");                                                                              //Receive voltage feedback
  Serial1.write("sys get vdd\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();
  
  Serial.print("EUI: ");                                                                                  //
  Serial1.write("sys get hweui\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();
  
  Serial.print("addr: ");                                                                                 //Set device address for your device!!!!!!!!!!!
  Serial1.write("mac set devaddr 02010101\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();
  
  Serial.print("nwkskey: ");                                                                              //Set NwkSkey key for your device!!!!!!!!!!!
  Serial1.write("mac set nwkskey 2B7E151628AED2A6ABF7158809CF4F3C\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();
  
  Serial.print("appskey: ");
  Serial1.write("mac set appskey 2B7E151628AED2A6ABF7158809CF4F3C\r\n");                                  //Set AppSkey for your device!!!!!!!!!!
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();
  
  Serial.print("adr: ");
  Serial1.write("mac set adr off\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();
  
  Serial.print("data rate: ");
  Serial1.write("mac get dr\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();

  Serial.print("ch: ");
  Serial1.write("mac get ch\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();

  Serial.print("band: ");
  Serial1.write("mac get band\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();

  
  Serial.print("rx delay1: ");
  Serial1.write("mac get rxdelay1\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();

  Serial.print("rx delay2: ");
  Serial1.write("mac get rxdelay2\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();
  
  Serial.print("Pwr: ");                                                                            
  Serial1.write("mac set pwridx 1\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();
  
  Serial.print("Join: ");
  Serial1.write("mac join abp\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();

  Serial.print("status: ");
  Serial1.write("mac get status\r\n");
  delay(50);
  while(Serial1.available()) Serial.write(Serial1.read());
  LoRaBlink();
  LoRaBlinkOff();
}

void LoRa::LoRaSendAndReceive(String message)
{
    int messageLength = message.length();
    Serial.print("Sending: ");
    Serial.println(message);
    Serial1.write("mac tx uncnf 1 ");
    
    
    for (int i = 0; i < messageLength; i++)
    {
      Serial1.print(message.charAt(i), HEX);
      LoRaBlink();  
      
    }
    Serial1.write("\r\n");
    LoRaBlinkOff();
    delay(100);
    
    Serial.println("RN2483 status:");
    while(Serial1.available()) 
    {
      Serial.print((char)Serial1.read());
    }
}

