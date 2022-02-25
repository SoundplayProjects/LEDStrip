// Changes:
// - simplified colours - single colours for Button A (pale cyan), Button B (pale yellow) and Collisions (red).
// - added in separate MIDI channels for Button A / Button B / Collisions A / Collision B (1/2/3/4)
// - fixed nextpulse rollover (strip was crashing after pulse 150)
// - fixed midi glitching by implementing a periodical midi kill on channels 3 and 4 - not ideal but works
// Issues:
// WIP:
// 25/2/22 RMS

#include "pulse.h"

#define NUMPULSES 150
#define NUMPIXELS 150 // 30px/m x 5m
#define SPRITELENGTH 7
#define MIDICHANNEL 1 // you can choose this - stays constant
#define MIDICHANNEL_A 1 // channel for button A
#define MIDICHANNEL_B 2 // channel for button B
#define MIDICHANNEL_CA 3 // channel for collision A
#define MIDICHANNEL_CB 4 // channel for collision B
#define COLOURS 7 // must match size of palette arrays
#define WAIT 25 // 25 for 30px/m strip - increased from 15 (60px/m strip)

boolean midiEnable = true;

int nextPulse = 0;
int currentPulseA = 0;
int currentPulseB = 0;

int count = 0;

boolean buttonAState = false;
boolean buttonBState = false;

int activeCount = 0;

int killCount = 0;
int collisionTimeOut = 100;

Pulse pulses[NUMPULSES]; // = {Pulse(),Pulse()};

// the setup function runs once when you press reset or power the board
void setup() {
  for (int i = 0; i < NUMPULSES; i++) {
    pulses[i] = Pulse();
    pulses[i].init(NUMPIXELS, SPRITELENGTH);
    pulses[i].index = i;
  }
  buttonSetup();
  pixelsSetup();
  buttonBreatheSetup();
  blinkSetup();
}

// the loop function runs over and over again forever
void loop() {
  //  colorWipe(0x00FF0000, 1500000);

  blinkLoop();
  buttonBreatheLoop();
  buttonRead();
  updatePulses();
  if (activeCount < NUMPULSES) {
    triggerDecide();
  }
  render();

  //  autoPresser();

  killCount++;
  if (killCount >= collisionTimeOut) {
    Serial.println("KILL CHANNELS 3 & 4");
    midiPanic34();
    killCount = 0;
  }

  delayMicroseconds(WAIT * 1000);
}

void updatePulses() {
  activeCount = 0;
  for (int i = 0; i < NUMPULSES; i++) { // step through the pulse object array
    if (pulses[i].active == true) { // check for active pulses
      activeCount++;
      debugPulses(i); // print debug info for this pulseSerial.print(pulses[j].index);
      collisionDetect(i, 0); // check for collisions on this pulse from POV of vector 0
      int colliding = pulses[i].update();// update this pulse
      //      Serial.println(colliding);

      if (colliding != 9999) {

        // this whole section is probably redundant now because of the killCount
//        if (colliding != 8888) {
//          Serial.println("KILL");
//          int currentPitchA = getPitch(i); // [seed % PITCHES];
//          int currentPitchB = getPitch(colliding); // [seed % PITCHES];
//          midiSendOff(currentPitchA, MIDICHANNEL_CA);
//          Serial.print("MIDI OFF ");
//          Serial.print(currentPitchA);
//          Serial.print(", ");
//          Serial.println(MIDICHANNEL_CA);
//          midiSendOff(currentPitchB, MIDICHANNEL_CB);
//          Serial.print("MIDI OFF ");
//          Serial.print(currentPitchB);
//          Serial.print(", ");
//          Serial.println(MIDICHANNEL_CB);
//        }
        // end of redundant bit

        pulses[i].expire(); //expire this pulse
      }
    }
  }
}

void debugPulses(int i) {
  Serial.print(pulses[i].index);
  Serial.print(", ");
  Serial.print(pulses[i].vector);
  Serial.print(", ");
  Serial.print(pulses[i].position);
  Serial.print(", ");
  Serial.print(pulses[i].end);
  Serial.print(", ");
  Serial.print(pulses[i].hold);
  Serial.print(", ");
  Serial.print(pulses[i].seed);
  Serial.print(", ");
  Serial.print(pulses[i].collision);
  Serial.print(", ");
  Serial.println("");
}

void incrementPulse() {
  nextPulse++;
  Serial.println(nextPulse);
  if (nextPulse >= NUMPULSES) {
    Serial.println("=========ROLLOVER PULSES==========");
    nextPulse = 0;
  }
}

void press(int vector, int velocity) {
  if (vector == 0) {
    incrementPulse();
    currentPulseA = nextPulse; // claim the current pulse
    pulses[currentPulseA].trigger(vector, velocity);
  }
  if (vector == 1) {
    incrementPulse();
    currentPulseB = nextPulse; // claim the current pulse
    pulses[currentPulseB].trigger(vector, velocity);
  }
}

void sustain(int vector) {
  if (vector == 0) {
    pulses[currentPulseA].sustain();
  }
  if (vector == 1) {
    pulses[currentPulseB].sustain();
  }
}

void release(int vector) {
  if (vector == 0) {
    pulses[currentPulseA].detrigger();
  }
  if (vector == 1) {
    pulses[currentPulseB].detrigger();
  }
}

void autoPresser() {
  int velocity = 31;

  if ((count + 300) % 400 == 0) {
    press(0, velocity);
  }
  if ((count + 200) % 400 == 0) {
    release(0);
  }

  if ((count + 100) % 400 == 0) {
    press(1, velocity);
  }
  if (count % 400 == 0) {
    release(1);
  }
  count++;
}
