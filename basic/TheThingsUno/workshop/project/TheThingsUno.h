// Copyright © 2016 The Things Network
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#ifndef _THETHINGSUNO_H_
#define _THETHINGSUNO_H_

#define DEBUG

#include <Arduino.h>
#include <Stream.h>

#define DEFAULT_WAIT_TIME 120
#define DEFAULT_FSB 7

#ifndef PWRIDX_868
  #define PWRIDX_868 1
#endif
#ifndef PWRIDX_915
  #define PWRIDX_915 5
#endif

#ifdef DEBUG
  #warning Debug mode
  #define debugPrintLn(...) { if (debugStream) debugStream->println(__VA_ARGS__); }
  #define debugPrint(...) { if (debugStream) debugStream->print(__VA_ARGS__); }
#else
  #define debugPrintLn(...)
  #define debugPrint(...)
#endif

class TheThingsUno
{
  private:
    Stream* modemStream;
    Stream* debugStream;
    String model;

    String readLine(int waitTime = DEFAULT_WAIT_TIME);
    bool waitForOK(int waitTime = DEFAULT_WAIT_TIME);
    String readValue(String key);
    bool sendCommand(String cmd, int waitTime = DEFAULT_WAIT_TIME);
    bool sendCommand(String cmd, String value, int waitTime = DEFAULT_WAIT_TIME);
    bool sendCommand(String cmd, const byte* buffer, int length, int waitTime = DEFAULT_WAIT_TIME);
    bool enableFsbChannels(int fsb);

  public:
    void init(Stream& modemStream, Stream& debugStream);
    void reset(bool adr = true, int fsb = DEFAULT_FSB);
    bool personalize(const byte devAddr[4], const byte nwkSKey[16], const byte appSKey[16]);
    bool join(const byte appEui[8], const byte appKey[16]);
    void sendBytes(const byte* buffer, int length, int port = 1, bool confirm = false);
    void sendString(String message, int port = 1, bool confirm = false);

#ifdef DEBUG
    void debugStatus();
#endif
};

#endif
