int loudnessPin = A0;

void setup() {
  pinMode(loudnessPin, INPUT);
}

void loop() {
  float loudness;
  loudness = analogRead(loudnessPin);
}
