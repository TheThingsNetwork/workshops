//This is an example for the MMA7660 1.5 G Accelerometer
//Download the library on https://github.com/Seeed-Studio/Accelerometer_MMA7660/archive/master.zip

#include <Wire.h>
#include "MMA7660.h"

MMA7660 accelerometer;

void setup() {
  accelerometer.init();
}

void loop() {
  float x, y, z;
  accelerometer.getXYZ(&x, &y, &z);

  float ax, ay, az;
  accelerometer.getAcceleration(&ax, &ay, &az);
}
