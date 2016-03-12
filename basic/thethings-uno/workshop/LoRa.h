#include <avr/pgmspace.h>

#include <Arduino.h>

#define DEFAULT_NWKSKEY "2B7E151628AED2A6ABF7158809CF4F3C"
#define DEFAULT_APPSKEY "2B7E151628AED2A6ABF7158809CF4F3C"

class LoRa
{
  private:
    void Configure(String command, String description, int delayTime = 50);

  public:
    void Config(String devAddr, String nwkSKey, String appSKey);
    void Send(String message);
};
