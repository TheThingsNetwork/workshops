#include "mbed.h"

float readTemperature(AnalogIn pin) {
    float value = pin.read();
    float celcius = (value * 3.3f - 0.6f) * 100.0f;
    float fahrenheit = celcius * 1.8f + 32;

    //return celcius;

    return fahrenheit;
}
