// Copyright © 2016 Johan Stokking <johan@thethingsnetwork.org>
// MIT Licensed

#define DEBUG
#include <Sodaq_RN2483.h>

#define debugSerial SerialUSB
#define loraSerial Serial1

// Change the DevAddr
const uint8_t devAddr[4] =
{
  0x00, 0x00, 0xAA, 0x01
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
  pinMode(BEE_VCC, OUTPUT);

  debugSerial.begin(57600);
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  
  // Wait for the serial te become available
  while (!debugSerial);
  debugSerial.println("Start");

  // Turn the LoRaBee on
  digitalWrite(BEE_VCC, HIGH);

  // Connect the LoRabee
  LoRaBee.setDiag(debugSerial);
  if (LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, true))
    debugSerial.println("Connection to the network was successful.");
  else
    debugSerial.println("Connection to the network failed!");
}

void loop()
{
  debugSerial.println("Sleeping for 5 seconds before starting sending out test packets.");
  delay(5000);

  // Send 10 packets, with at least 5 seconds delay after each transmission (more seconds if the device is busy)
  uint8_t i = 10;
  while (i > 0)
  {
    String message = "Hello world! This is message #" + String(i, DEC);
    uint8_t payload[message.length() + 1];
    message.getBytes(payload, message.length() + 1);

    switch (LoRaBee.send(1, payload, message.length()))
    {
      case NoError:
        debugSerial.println("Successful transmission.");
        i--;
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
    delay(5000);
  }

  while (1) { } // block forever
}
