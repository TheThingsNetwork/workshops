
#include "AirQuality.h"
#include "Arduino.h"

AirQuality airqualitysensor;

int current_quality = -1;
unsigned long counter = 2000;
int counter_step = 2000; //counter makes steps of 2 seconds

void setup() {
  airqualitysensor.init(14);
}

void loop() {
  //counter
  if (millis() >= counter) //set 2 seconds as a detected duty
  {
    airqualitysensor.last_vol = airqualitysensor.first_vol;
    airqualitysensor.first_vol = analogRead(A0); // change this value if you use another A port
    airqualitysensor.counter = 0;
    airqualitysensor.timer_index = 1;

    counter = millis() + counter_step;
  }

  current_quality = airqualitysensor.slope();
  if (current_quality >= 0) {
    if (current_quality == 0)
      Serial.println("High pollution! Force signal active");
    else if (current_quality == 1)
      Serial.println("High pollution!");
    else if (current_quality == 2)
      Serial.println("Low pollution!");
    else if (current_quality == 3)
      Serial.println("Fresh air");
  }
}
