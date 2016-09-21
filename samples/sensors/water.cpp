int waterPin = 4;

void setup() {
  pinMode(waterPin, INPUT);
}

void loop() {
  int water;
  water = digitalRead(waterPin);
}
