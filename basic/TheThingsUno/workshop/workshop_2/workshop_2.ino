#include "TheThingsUno.h"

//{ <insert AppEui> }; //for example:
// Set your AppEUI and AppKey
const byte appEui[8] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x00, 0x04, 0xD4 };
const byte appKey[16] = { 0x5A, 0xD3, 0x2E, 0x74, 0xD7, 0xE8, 0x64, 0x11, 0xB3, 0x11, 0x8D, 0x8B, 0xD1, 0x5F, 0x17, 0xEE };

//define AnalogPins for sensors
#define Light A0
#define Temperature A1
#define Sound A2
#define Moisture A3

//buffers for smoothing out sensor values
uint16_t bl[16], bt[16], bs[16], bm[16];

//delay counter
int x = 0;

//data array for transmitting data
byte data[8];

void sensorRead(int , byte [], int, uint16_t [], int, bool );

#define debugSerial Serial
#define loraSerial Serial1

#define debugPrintLn(...) { if (debugSerial) debugSerial.println(__VA_ARGS__); }
#define debugPrint(...) { if (debugSerial) debugSerial.print(__VA_ARGS__); }

TheThingsUno ttu;

void setup() {
  debugSerial.begin(115200);
  loraSerial.begin(57600);

  delay(1000);
  ttu.init(loraSerial, debugSerial);
  ttu.reset();

  //the device will attempt a join every second till the join is successfull
   while(!ttu.join(appEui, appKey)){
       delay(1000);
   }
   //loraSerial.println("mac join otaa");

  digitalWrite(13, HIGH); //turn on LED to confirm join

  delay(6000);
  ttu.showStatus();
  debugPrintLn("Setup for The Things Network complete");

  delay(1000);
}

void loop() {

  //put data into the data array
  debugPrint("Light Sensor: ");
  sensorRead(Light, data, 0, bl, 16, true);
  debugPrint("Temperature Sensor: ");
  sensorRead(Temperature, data, 2, bt, 16, true);
  debugPrint("Sound Sensor: ");
  sensorRead(Sound, data, 4, bs, 16, true);
  debugPrint("Moisture Sensor: ");
  sensorRead(Moisture, data, 6, bm, 16, true);
  debugPrintLn("");

  //send data every 20 secconds
  if(x==20){
    ttu.sendBytes(data, sizeof(data));
    debugPrintLn("Transmitting data");
    x=0;
  }
  x++;

  delay(500); //1/4th seccond
}

//function to: read sensor values, transform readings into bytes and add them into an array.
//Also there is an option for smoothing out the sensor readings.
void sensorRead(int pin, byte data[] , int p, uint16_t buf[], int length, bool buffer){
  int i; //counter
  uint16_t sum = 0;

  //use buffer if enabled
  if(buffer){
    //debugPrint("Buffer enabled, ");
    //debugPrint(length);
    for(i = 0; i<length; i++){
        buf[(length-1)-i] = buf[(length-2)-i];
    }
    buf[0] = analogRead(pin);

    //average
    for(i=0; i<=(length-1); i++)
        sum += buf[i];
        sum /= length;
    }else{
      sum = analogRead(pin);
  }

  //debugPrint("Sensor reading = ");
  debugPrintLn(sum);

  data[p] = lowByte(sum);
  data[p+1] = highByte(sum);
}
