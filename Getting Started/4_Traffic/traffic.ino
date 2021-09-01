#include <TheThingsNetwork.h>
// Set your AppEUI and AppKey
const char *appEui = "800000000000007B";
const char *appKey = "bbad768dd0e311d470524d12c28b903f";

#define loraSerial Serial1
#define debugSerial Serial

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define freqPlan TTN_FP_EU868

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

int sensorPin = A0;
int sensorValue = 0;
int SENSOR_THRESHOLD_LOW = 640;
int SENSOR_THRESHOLD_HIGH = 670;

void setup() {
  pinMode(sensorPin, INPUT);

  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
}

void loop() {
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);

  if(sensorValue < SENSOR_THRESHOLD_LOW || sensorValue > SENSOR_THRESHOLD_HIGH){
    // Prepare payload of 2 bytes to indicate sensor value
    byte payload[2];
    payload[0] = highByte(sensorValue);
    payload[1] = lowByte(sensorValue);

    // Send it as an Uplink
    ttn.sendBytes(payload, sizeof(payload));
    debugSerial.println("Sending data");

    delay(10000);
  }

  else{
    delay(1000);
  }
}
