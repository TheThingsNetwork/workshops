#include "Arduino.h"
#include "LoRa.h"

#define CHANNEL_LOW 48
#define CHANNEL_HIGH 55

void LoRa::Configure(String command, String description, int delayTime)
{
  Serial.print(description + ": ");
  Serial1.print(command + "\r\n");
  delay(delayTime);
  while (Serial1.available())
    Serial.write(Serial1.read());
}

void LoRa::Config(String devAddr, String nwkSKey, String appSKey)
{
  Configure("sys reset", "Reset", 1000);
  Configure("sys get ver", "Version");
  Configure("sys get vdd", "Battery");
  Configure("sys get hweui", "EUI");
  Configure("mac set devaddr " + devAddr, "Set Address");
  Configure("mac set nwkskey " + nwkSKey, "Set Network Key");
  Configure("mac set appskey " + appSKey, "Set Application Key");
  Configure("mac set adr off", "Set ADR off");
  Configure("mac get dr", "Data Rate");
  //Configure("mac get ch", "Channel");
  //Configure("mac get band", "Band");
  Configure("mac get rxdelay1", "RX Delay 1");
  Configure("mac get rxdelay2", "RX Delay 2");
  Configure("mac set pwridx 10", "Set Power");
  Configure("mac join abp", "Join ABP");
  Configure("mac get status", "Status");

  for (int i = 0; i < 72; i++) {
    String command = "mac set ch status " + String(i);
    if (i == 70 || CHANNEL_LOW <= i && i <= CHANNEL_HIGH)
      Configure("mac set ch status " + String(i) + " on", "Enable channel " + String(i));
    else
      Configure("mac set ch status " + String(i) + " off", "Disable channel " + String(i));
  }
}

void LoRa::Send(String message)
{
  int messageLength = message.length();
  Serial.println("Sending: " + message);

  Serial1.write("mac tx uncnf 1 ");
  for (int i = 0; i < messageLength; i++)
    Serial1.print(message.charAt(i), HEX);
  Serial1.write("\r\n");

  delay(100);

  while (Serial1.available())
    Serial.write(Serial1.read());
}
