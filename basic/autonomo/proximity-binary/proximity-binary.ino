// Copyright © 2016 Hylke Visser <htdvisser@gmail.com>
// MIT Licensed - See http://htdvisser.mit-license.org/

#include "Ultrasonic.h"
#include "RunningAverage.h"

#include "Sodaq_RN2483.h"

#define debugSerial SerialUSB
#define loraSerial Serial1

#define ultrasonicPin 4

Ultrasonic ultrasonic(ultrasonicPin);

// Change the DevAddr
const uint8_t devAddr[4] =
{
  0x48, 0x56, 0x55, 0x52
};

// Public Semtech key
const uint8_t appSKey[16] =
{
  0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};

// Public Semtech key
const uint8_t nwkSKey[16] =
{
  0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};

void setup()
{
  debugSerial.begin(57600);
  loraSerial.begin(LoRaBee.getDefaultBaudRate());

  pinMode(BEE_VCC, OUTPUT);

  // Wait for the serial te become available
  while (!debugSerial);

  // Turn the modem on
  digitalWrite(BEE_VCC, HIGH);

  // Connect the LoRabee
  LoRaBee.setDiag(debugSerial); // optional
  if (LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, true))
  {
    debugSerial.println("Connection to the network was successful.");
  }
  else
  {
    debugSerial.println("Connection to the network failed!");
  }

  // Countdown
  debugSerial.print("Starting in... ");
  for (uint8_t i = 5; i > 0; i--)
  {
    debugSerial.print(i);
    debugSerial.print("... ");
    delay(1000);
  }
  debugSerial.println();
}

RunningAverage averageValues(60);

int counter = 0;

void toBytes(long minVal, long avgVal, long maxVal, byte b[6])
{
  b[0] = (byte)((minVal >> 8) & 0xff);
  b[1] = (byte)((minVal) & 0xff);

  b[2] = (byte)((avgVal >> 8) & 0xff);
  b[3] = (byte)((avgVal) & 0xff);

  b[4] = (byte)((maxVal >> 8) & 0xff);
  b[5] = (byte)((maxVal) & 0xff);
}

void loop()
{
  long range;

  range = ultrasonic.MeasureInCentimeters();
  averageValues.addValue(range);
  counter++;

  debugSerial.print(range);
  debugSerial.println(" cm");

  if (counter >= 60) {
    long minVal = averageValues.getMin();
    long avgVal = averageValues.getAverage();
    long maxVal = averageValues.getMax();

    // Construct the payload MIN AVG MAX
    byte payload[6];
    toBytes(minVal, avgVal, maxVal, payload);

    // Debug info
    debugSerial.println("Sending summary:");
    debugSerial.print("min: ");
    debugSerial.print(minVal);
    debugSerial.print(" avg: ");
    debugSerial.print(avgVal);
    debugSerial.print(" max: ");
    debugSerial.print(maxVal);
    debugSerial.println(" ...");

    // Debug Info
    for(int i = 0; i < 6; i ++) {
      debugSerial.print(payload[i], HEX);
      debugSerial.print(" ");
    }
    debugSerial.println();

    switch (LoRaBee.send(1, payload, 6))
    {
      case NoError:
        debugSerial.println("Successful transmission.");
        break;
      case NoResponse:
        debugSerial.println("There was no response from the device.");
        break;
      case Timeout:
        debugSerial.println("Connection timed-out. Check your serial connection to the device! Sleeping for 20sec.");
        delay(20000);
        break;
      case PayloadSizeError:
        debugSerial.println("The size of the payload is greater than allowed. Transmission failed!");
        break;
      case InternalError:
        debugSerial.println("Oh No! This shouldn't happen. Something is really wrong! Try restarting the device!\r\nThe program will now halt.");
        while (1) {};
        break;
      case Busy:
        debugSerial.println("The device is busy. Sleeping for 10 extra seconds.");
        delay(10000);
        break;
      case NetworkFatalError:
        debugSerial.println("There is a non-recoverable error with the network connection. You should re-connect.\r\nThe program will now halt.");
        while (1) {};
        break;
      case NotConnected:
        debugSerial.println("The device is not connected to the network. Please connect to the network before attempting to send data.\r\nThe program will now halt.");
        while (1) {};
        break;
      case NoAcknowledgment:
        debugSerial.println("There was no acknowledgment sent back!");
        break;
      default:
        break;
    }

    counter = 0;
    averageValues.clear();

  }

  delay(1000);
}
