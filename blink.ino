#define BLINKPIN 13

void blinkSetup() {
  pinMode(BLINKPIN, OUTPUT);
  digitalWrite(BLINKPIN, HIGH);
//  Serial.begin(9600);
}

void blinkLoop() {
  if (millis() % 2000 < 1000) {
    digitalWrite(BLINKPIN, LOW);
  } else {
    digitalWrite(BLINKPIN, HIGH);
  }
}
