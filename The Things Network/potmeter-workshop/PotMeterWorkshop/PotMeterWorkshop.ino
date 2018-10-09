#include <TheThingsNetwork.h>

#define loraSerial Serial1    // This is always the same on The Things Uno.
#define debugSerial Serial    // This is always the same on The Things Uno.

#define freqPlan TTN_FP_EU868 // Change this if you're not in the EU.

TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan); // Initialize the TheThingsNetwork library.

// Set your devAddr, nwkSKey and appSKey.
// You can directly copy-paste these from the Console (bottom of the device info page).
const char *devAddr = "26011D83";
const char *nwkSKey = "39758646045937050FB6D5047BF78BE7";
const char *appSKey = "AF81BC36C7DFEAB4BFA78D8F31558AFD";

int sensorPin = A0; // We connected the sensor on Analog pin 0.
int ledPin = 13; // This is the internal LED.

#define SENSOR_VALUE_LOW    100 // This is the lowest value to allow a NORMAL state
#define SENSOR_VALUE_HIGH   900 // This is the highest value to allow a NORMAL state
#define SENSOR_VALUE_BUFFER 50  // This is to avoid the state changing too often

int sensorValue = 0;       // We'll store the sensor value here.
int sensorValueMin = 1024; // initialize to the maximum possible value
int sensorValueMax = 0;    // initialize to the minimum possible value

// These are the states the sensor can be in:
#define SENSOR_STATE_LOW -1
#define SENSOR_STATE_NORMAL 0
#define SENSOR_STATE_HIGH 1

int sensorState = 0; // We'll store the sensor state here.

void updateSensor() {
  sensorValue = analogRead(sensorPin); // Read the pin voltage.

  debugSerial.print(sensorValue);
  debugSerial.print(" ");

  // Update the min and max values if needed:
  if (sensorValue < sensorValueMin) {
    sensorValueMin = sensorValue;
  }
  if (sensorValue > sensorValueMax) {
    sensorValueMax = sensorValue;
  }

  // Update the sensor state:
  int nextSensorState = sensorState;
  switch (sensorState) {
  case SENSOR_STATE_NORMAL:
    if (sensorValue < SENSOR_VALUE_LOW) {
      nextSensorState = SENSOR_STATE_LOW; // Sensor value dropped: NORMAL -> LOW
      break;
    }
    if (sensorValue > SENSOR_VALUE_HIGH) {
      nextSensorState = SENSOR_STATE_HIGH; // Sensor value spiked: NORMAL -> HIGH
      break;
    }
    break;
  case SENSOR_STATE_LOW:
    if (sensorValue > SENSOR_VALUE_LOW + SENSOR_VALUE_BUFFER) {
      nextSensorState = SENSOR_STATE_NORMAL; // Sensor value went up to NORMAL
      break;
    }
    break;
  case SENSOR_STATE_HIGH:
    if (sensorValue < SENSOR_VALUE_HIGH - SENSOR_VALUE_BUFFER) {
      nextSensorState = SENSOR_STATE_NORMAL; // Sensor value went down to NORMAL
      break;
    }
    break;
  }

  if (nextSensorState != sensorState) {
    debugSerial.println();
    debugSerial.print("State change: ");
    debugSerial.print(sensorState);
    debugSerial.print(" -> ");
    debugSerial.println(nextSensorState);
    sensorState = nextSensorState;
  }

  return sensorState;
}

void waitAndBlink(int duration) {
  switch (sensorState) {
      case SENSOR_STATE_NORMAL:
        // In the NORMAL state we just wait.
        delay(duration);
        break;
      case SENSOR_STATE_LOW:
      case SENSOR_STATE_HIGH:
        // In an ABNORMAL state we blink the LED.
        for (int i = 0 ; i < 10; i++) {
          digitalWrite(ledPin, HIGH);
          delay(duration/20);
          digitalWrite(ledPin, LOW);
          delay(duration/20);
        }
        break;
    }
}

#define SEND_INTERVAL_NORMAL   300000 // When everything is normal; send every 5 minutes.
#define SEND_INTERVAL_ABNORMAL 60000  // When the state is abnormal; send every minute.

unsigned long lastTransmit = 0; // The timestamp when we transmitted the last message.

bool transmitSensorValue(int sensorValue) {
  if (lastTransmit != 0) {
    switch (sensorState) {
      case SENSOR_STATE_NORMAL:
        // If the state is normal, we don't need to transmit that often.
        if (millis() < lastTransmit + SEND_INTERVAL_NORMAL) {
          return false;
        }
        break;
      case SENSOR_STATE_LOW:
      case SENSOR_STATE_HIGH:
        // If the state is abnormal, we transmit more often.
        if (millis() < lastTransmit + SEND_INTERVAL_ABNORMAL) {
          return false;
        }
        break;
    }
  }

  ttn.wake(); // Wake the radio module if it was sleeping.

  debugSerial.println();
  debugSerial.print("Transmit sensor value: ");
  debugSerial.print(float(sensorValue));
  debugSerial.print(" min: ");
  debugSerial.print(float(sensorValueMin));
  debugSerial.print(" max: ");
  debugSerial.println(float(sensorValueMax));
  lastTransmit = millis();

  // We are going to transmit the 3 numbers into a 6-byte payload.
  // For comparison:
  // - 31 bytes of JSON: {"min":234,"val":567,"max":890}
  // - 11 bytes of CSV: 234,567,890
  // A single byte can only go up to 256, so we need two bytes each.
  // See https://www.thethingsnetwork.org/docs/devices/bytes.html for more info.
  byte payload[6];
  payload[0] = highByte(sensorValueMin);
  payload[1] = lowByte(sensorValueMin);
  payload[2] = highByte(sensorValue);
  payload[3] = lowByte(sensorValue);
  payload[4] = highByte(sensorValueMax);
  payload[5] = lowByte(sensorValueMax);

  ttn_response_t result = ttn.sendBytes(payload, 6, 2); // payload of 6 bytes on port 2.
  if (result == TTN_SUCCESSFUL_TRANSMISSION) {
    sensorValueMin = 1024; // Reset to initial values.
    sensorValueMax = 0;    // Reset to initial values.
    ttn.sleep(SEND_INTERVAL_NORMAL); // Switch the radio to low power mode.
    delay(100);
    return true;
  }
  return false;
}

// Use the following function to decode the payload on the server side:
/*
function Decoder(bytes, port) {
  var decoded = {};
  if (port === 2 && bytes.length === 6) {
    decoded.min = (bytes[0] << 8) + bytes[1];
    decoded.val = (bytes[2] << 8) + bytes[3];
    decoded.max = (bytes[4] << 8) + bytes[5];
  }
  return decoded;
}
*/

void setup() {
  pinMode(sensorPin, INPUT); // The sensor pin is an input.

  loraSerial.begin(57600); // This is always the same on The Things Uno.
  debugSerial.begin(9600); // We can change this if needed, but this is the default in the Arduino IDE.

  while (!debugSerial && millis() < 10000) {} // Wait a maximum of 10s for Serial Monitor.

  // We're using personalized devices (ABP) in this workshop. This means that all network settings are hard-coded.
  // In real life situations you'll want to use a proper join-procedure (OTAA).
  debugSerial.println("-- NETWORK SETUP");
  ttn.personalize(devAddr, nwkSKey, appSKey);

  debugSerial.println("-- SETUP DONE");
}

void loop() {
  updateSensor();
  transmitSensorValue(sensorValue);
  waitAndBlink(1000);
}
