// Get the temperature from the specified pin
float getTemperature(int pin) {
  // Get the voltage based on 5.0 V
  float voltage = analogRead(pin) * 5.0 / 1024.0;
  // Convert to Celcius
  return (voltage - 0.5) * 100.0;
}

// Get the moisture from the specified pin
float getMoisture(int pin) {
  return analogRead(pin);
}
