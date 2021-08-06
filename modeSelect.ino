#define MODE1 1 // modeSelect pins
#define MODE2 2 // modeSelect pins
#define MODE3 3 // modeSelect pins
#define MODE4 4 // modeSelect pins

void modeSelectSetup() {
  pinMode(MODE1, INPUT_PULLUP);
  pinMode(MODE2, INPUT_PULLUP);
  pinMode(MODE3, INPUT_PULLUP);
  pinMode(MODE4, INPUT_PULLUP);
}

void modeSelectRoutine() {
  mode = 0;
  if (digitalRead(MODE1) == LOW) {
    mode = mode + 1;
  }
  if (digitalRead(MODE2) == LOW) {
    mode = mode + 2;
  }
  if (digitalRead(MODE3) == LOW) {
    mode = mode + 4;
  }
  if (digitalRead(MODE4) == LOW) {
    mode = mode + 8;
  }
//  Serial.println(mode);
}
