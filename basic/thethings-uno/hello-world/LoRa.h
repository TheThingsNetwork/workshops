#ifndef LoRa_h
#define LoRa_h

#include <avr/pgmspace.h>

#include <Arduino.h>

class LoRa
{
  private:
    int red = A3; //this sets the red led pin
    int green = A2; //this sets the green led pin
    int blue = A0; //this sets the blue led pin
    
    void LoRaBlink()
    {
      analogWrite(red, 0);
      analogWrite(blue, 255);
      analogWrite(green, 255);
      delay(50);
      analogWrite(red, 255);
      analogWrite(blue, 255);
      analogWrite(green, 255);
      delay(50);
    }

    void LoRaBlinkOff()
    {
      analogWrite(red, 255);
      analogWrite(blue, 0);
      analogWrite(green, 0);
    }

      

  public:
    void LoRaConfig();
    void LoRaSendAndReceive(String message);
};
#endif
