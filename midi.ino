//#include <usbmidi.h>
#define PITCHES 7 // must match size of pitch array below
uint8_t pitch[] = {36, 39, 42, 45, 46, 48, 51}; // blues scale

void midiReceiveOn() {
  midiState=true; // set midi flag 
  
  midiStateChange=true;
}

void midiReceiveOff() {
  midiState=false; // reset midi flag
  midiStateChange=true;
}

void midiSetup() {
  usbMIDI.setHandleNoteOn(midiReceiveOn);
  usbMIDI.setHandleNoteOff(midiReceiveOff);
}

void midiRead() {
  midiStateChange=false;
  usbMIDI.read();         // All Channels
}

void midiSendOn(int p) {
//  currentPitch=pitch[random(0,6)]; //pick a pitch
  usbMIDI.sendNoteOn(p, 99, MIDICHANNEL);  
}

void midiSendOff(int p) {
  usbMIDI.sendNoteOff(p, 99, MIDICHANNEL);  
}

boolean midiRisingEdge(){
  if (midiState==true && midiStateChange==true){
    return true;
  }
  else return false;
}

boolean midiFallingEdge(){
  if (midiState==false && midiStateChange==true){
    return true;
  }
  else return false;
}
