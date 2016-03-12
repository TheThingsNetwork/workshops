void loop() {
  // Get the temperature in Celcius
  float celcius = getTemperature(TEMPSENSOR);
  Serial.println(String(celcius) + " degrees C");

  // Get the soil moisture
  float moisture = getMoisture(SOILSENSOR);
  Serial.println(String(moisture) + " moisture");

  // Create a message
  String message = String(celcius) + ", " + String(moisture);

  // Send the message
  ttn.Send(message);

  // Wait 10 seconds
  delay(10000);
}
