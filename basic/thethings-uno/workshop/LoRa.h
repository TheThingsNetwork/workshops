#include <avr/pgmspace.h>

#include <Arduino.h>

class LoRa
{
  private:
    void Configure(String command, String description, int delayTime = 50);

  public:
    void Config(String devAddr, String nwkSKey, String appSKey);
    void Send(String message);
};
