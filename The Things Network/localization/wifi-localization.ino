#include <TheThingsNetwork.h>
#include <AltSoftSerial.h>
#define HEX_CHAR_TO_NIBBLE(c) ((c >= 'a') ? (c - 'a' + 0x0A) : (c - '0'))
#define HEX_PAIR_TO_BYTE(h, l) ((HEX_CHAR_TO_NIBBLE(h) << 4) + HEX_CHAR_TO_NIBBLE(l))

// Parameters for the esp stream/buffer
#define ESP8266_SERIAL_SPEED 9600
#define MAX_ACCESS_POINTS 3
#define WIFI_BSSID_SIZE 6
#define WAIT_TO_SEND 20000

// Set your AppEUI and AppKey
const char *appEui = "0000000000000000";
const char *appKey = "00000000000000000000000000000000";

// Replace REPLACE_ME with TTN_FP_EU868 or TTN_FP_US915
#define FREQPLAN REPLACE_ME
// Define the used serials
#define debugSerial Serial
#define loraSerial Serial1
AltSoftSerial espSerial;

TheThingsNetwork ttn(loraSerial, debugSerial, FREQPLAN);
// Access point data
struct AP
{
  uint8_t bssid[WIFI_BSSID_SIZE];
  int8_t rssi;
};

// Read OK response from ESP, ignoring echo (starting with AT+)
bool waitForOKFromESP()
{
  char line[128];
  size_t read;
  while ((read = readLineFromESP(line, sizeof(line))) == 0 || strncmp("AT+", line, 3) == 0)
  {
  }
  return strncmp("OK", line, 2) == 0;
}
// Read line from ESP
size_t readLineFromESP(char *buffer, size_t size)
{
  size_t read = 0;
  while ((read = espSerial.readBytesUntil('\n', buffer, size)) == 0)
  {
  }
  buffer[read - 1] = '\0';
  return read - 1;
}
// Send out the collected access points to The Things Network
void sendAPs(AP aps[], int count)
{
  ttn.sendBytes((uint8_t *)aps, count * sizeof(AP));
}
void setup()
{
  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;
  loraSerial.begin(57600);
  debugSerial.begin(115200);
  espSerial.begin(ESP8266_SERIAL_SPEED);
  debugSerial.println("-- STATUS");
  ttn.showStatus();
  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
  // Set ESP in station mode
  espSerial.println(F("AT+CWMODE=1"));
  if (!waitForOKFromESP())
  {
    debugSerial.println(F("Set mode failed! Check if wires are connected the right way"));
    delay(UINT32_MAX);
  }
}
void loop()
{
  debugSerial.println("start loop");
  uint8_t count = 0;
  AP aps[MAX_ACCESS_POINTS];
  char line[128];
  size_t read;
  // Send command to list access points to ESP
  espSerial.println(F("AT+CWLAP"));
  read = readLineFromESP(line, sizeof(line));
  if (strncmp("AT+CWLAP", line, 8) != 0)
  {
    debugSerial.print("ESP did not respond with command echo, expecting AT+CWLAP but got: ");
    debugSerial.println(line);
    goto exit;
  }
  readLineFromESP(line, sizeof(line));
  while (strncmp("OK", line, 2) != 0)
  {
    debugSerial.println(line);
    if (count == MAX_ACCESS_POINTS || strncmp("+CWLAP", line, 2) != 0)
    {
      readLineFromESP(line, sizeof(line));
      continue;
    }
    // Parse the response line
    strtok(line, "\"");
    strtok(NULL, "\"");
    char *rssiStr = strtok(NULL, "\"") + 1;
    rssiStr[strlen(rssiStr) - 1] = '\0';
    int8_t rssi = atoi(rssiStr);
    char *bssid = strtok(NULL, "\"");
    // Check if a valid BSSID string has been found
    if (bssid && strlen(bssid) == WIFI_BSSID_SIZE * 3 - 1)
    {
      debugSerial.print(F("RSSI: "));
      debugSerial.println(rssi);
      debugSerial.print(F("BSSID: "));
      debugSerial.println(bssid);
      aps[count].rssi = rssi;
      // Put the BSSID into an array for easy access
      char *b = strtok(bssid, ":");
      for (int i = 0; i < WIFI_BSSID_SIZE && b; i++, b = strtok(NULL, ":"))
      {
        aps[count].bssid[i] = HEX_PAIR_TO_BYTE(b[0], b[1]);
      }
      count++;
    }
    readLineFromESP(line, sizeof(line));
  }
  if (count > 0)
  {
    sendAPs(aps, count);
  }
exit:
  delay(WAIT_TO_SEND);
  debugSerial.println("void loop");
}
