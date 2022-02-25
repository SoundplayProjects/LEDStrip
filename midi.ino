#define PITCHES 7 // must match size of pitch array below
uint8_t pitch[] = {36, 39, 42, 45, 46, 48, 51}; // blues scale

boolean midiState = false;
boolean midiStateChange = false;

void midiSetup() {
  usbMIDI.setHandleNoteOn(midiReceiveOn);
  usbMIDI.setHandleNoteOff(midiReceiveOff);
}

int getPitch(int index) {
  int seed = pulses[index].seed; // random(0, COLOURS);
  int currentPitch = pitch[seed % PITCHES]; // [seed % PITCHES];
  return currentPitch;
}

void midiReceiveOn() {
  midiState = true; // set midi flag
  midiStateChange = true;
}

void midiReceiveOff() {
  midiState = false; // reset midi flag
  midiStateChange = true;
}

void midiPanic34() {
  for (int c = 3; c <= 4; c++) {
    for (int p = 0; p < 128; p++) {
      midiSendOff(p, c);
    }
  }
}

void midiRead() {
  midiStateChange = false;
  usbMIDI.read();         // All Channels
}

void midiSendOn(int p, int c) {
  usbMIDI.sendNoteOn(p, 99, c);
}

void midiSendOff(int p, int c) {
  usbMIDI.sendNoteOff(p, 99, c);
}

boolean midiRisingEdge() {
  if (midiState == true && midiStateChange == true) {
    return true;
  }
  else return false;
}

boolean midiFallingEdge() {
  if (midiState == false && midiStateChange == true) {
    return true;
  }
  else return false;
}
