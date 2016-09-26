//This is an example for the LSM6DS3 16 G Accelerometer and Gyroscope
//Download the library on https://github.com/Seeed-Studio/Accelerometer_And_Gyroscope_LSM6DS3/archive/master.zip

#include "SparkFunLSM6DS3.h"
#include "Wire.h"

LSM6DS3 myIMU(I2C_MODE, 0x6A);  //I2C device address 0x6A

void setup(){
  if (myIMU.begin() != 0) {
	  Serial.println("Device error");
  } else {
    Serial.println("Device OK!");
  }
}

void loop() {
  float accelX = myIMU.readFloatAccelX();
  float accelY = myIMU.readFloatAccelY();
  float accelZ = myIMU.readFloatAccelZ();

  float gyroX = myIMU.readFloatGyroX();
  float gyroY = myIMU.readFloatGyroY();
  float gyroZ = myIMU.readFloatGyroZ();

  float tempC = myIMU.readTempC();
  float tempF = myIMU.readTempF();
}
