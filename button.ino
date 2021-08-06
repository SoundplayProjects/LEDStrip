#include <Bounce.h>

#define BUTTON1 16 // button pin
#define BUTTON2 17 // button pin
//#define SENSOR1 A9 // sensor pin
//#define KNOB1 A8 // knob pin
//#define THRESHOLD 2 //sensor threshold


// Instantiate a Bounce object with a 25 millisecond debounce time
Bounce bouncer1 = Bounce(BUTTON1, 25); // (pin, milliseconds)

void buttonSetup() {
  pinMode(BUTTON1, INPUT_PULLUP); // sets up a button input pin
}

void buttonRead() {
  //  knob = analogRead(KNOB1); // read the calibration knob
  //  sensor = analogRead(SENSOR1); // read sensor input
  //  int sensorDiff = abs(sensor - lastSensor); // calculate change
  //  lastSensor = sensor; // shift sensor data to lastSensor
  boolean button1Changed = bouncer1.update(); // Update debouncer
  if (bouncer1.read())
  {
    buttonState = false; // flag as pressed
  } else {
    buttonState = true; // flag as released
  }
//  Serial.println(buttonState);
}
