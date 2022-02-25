#include <Bounce.h>

#define BUTTONA 16 // button pin
#define BUTTONB 17 // button pin

// Instantiate a Bounce object with a 25 millisecond debounce time
Bounce bouncerA = Bounce(BUTTONA, 25); // (pin, milliseconds)
Bounce bouncerB = Bounce(BUTTONB, 25); // (pin, milliseconds)

void buttonSetup() {
  pinMode(BUTTONA, INPUT_PULLUP); // sets up a button input pin
  pinMode(BUTTONB, INPUT_PULLUP); // sets up a button input pin
}

void buttonRead() {
  boolean buttonAChanged = bouncerA.update(); // Update debouncer
  boolean buttonBChanged = bouncerB.update(); // Update debouncer
  if (bouncerA.read())
  {
    buttonAState = false; // flag as pressed
  } else {
    buttonAState = true; // flag as released
  }
  if (bouncerB.read())
  {
    buttonBState = false; // flag as pressed
  } else {
    buttonBState = true; // flag as released
  }
}
