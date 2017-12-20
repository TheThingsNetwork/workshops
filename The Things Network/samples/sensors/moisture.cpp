int moisturePin = A0;

void setup() {
  pinMode(moisturePin, INPUT);
}

// moisture::air        = 0
// moisture::soil       = 20 +/-
// moisture::humid soil = 580 +/-
// moisture::water      = 940 +/-

void loop() {
  float moisture;
  moisture = analogRead(moisturePin);
}
