int luminancePin = A0;

void setup() {
  pinMode(luminancePin, INPUT);
}

void loop() {
  float sensorVoltage;
  float lightlevel;
  lightlevel = analogRead(lightPin) * (5.0 / 1023.0);
}
