#include <TheThingsNetwork.h>

// Set your DevAddr, NwkSKey, AppSKey and the frequency plan
const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

int sensorPin = A0;
int sensorValue = 0;
int SENSOR_THRESHOLD_LOW = 100;
int SENSOR_THRESHOLD_HIGH = 900;

void setup() {
  pinMode(sensorPin, INPUT);

  loraSerial.begin(57600);
  debugSerial.begin(9600);
 
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("-- PERSONALIZE");
  ttn.personalize(devAddr, nwkSKey, appSKey);

  debugSerial.println("-- STATUS");
  ttn.showStatus();
}

void loop() {
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);

  if(sensorValue < SENSOR_THRESHOLD_LOW || sensorValue > SENSOR_THRESHOLD_HIGH){
    // Prepare payload of 2 bytes to indicate LED status
    byte payload[2];
    payload[0] = highByte(sensorValue);
    payload[1] = lowByte(sensorValue);

    // Send it off
    ttn.sendBytes(payload, sizeof(payload));
    debugSerial.println("Sending data");

    delay(10000);
  }

  else{
    delay(100);
  }
}
