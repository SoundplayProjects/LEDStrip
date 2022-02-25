void triggerDecide() {
  int velocity = 31;
  if (bouncerA.fallingEdge() || midiRisingEdge()) { // check for button press / midi start
    press(0, velocity); // do the triggering stuff
    Serial.println("BUTTON A TRIGGER");
    if (midiEnable) {
      int currentPitchA = getPitch(currentPulseA); // [seed % PITCHES];
      Serial.print("MIDI ON ");
      Serial.print(currentPitchA);
      Serial.print(", ");
      Serial.println(MIDICHANNEL_A);
      midiSendOn(currentPitchA, MIDICHANNEL_A); // start a midi note
    }
  } else if (bouncerA.risingEdge() || midiFallingEdge()) { // check for button release / midi end
    release(0);
    Serial.println("BUTTON A DETRIGGER");
    if (midiEnable) {
      int currentPitchA = getPitch(currentPulseA); // [seed % PITCHES];
      Serial.print("MIDI OFF ");
      Serial.print(currentPitchA);
      Serial.print(", ");
      Serial.println(MIDICHANNEL_A);
      midiSendOff(currentPitchA, MIDICHANNEL_A); // end a midi note
    }
  }

  if (bouncerB.fallingEdge()) { // check for button press / midi start / audio on
    press(1, velocity); // do the triggering stuff
    Serial.println("BUTTON B TRIGGER");
    if (midiEnable) {
      int currentPitchB = getPitch(currentPulseB); // - 24; //
      Serial.print("MIDI ON ");
      Serial.print(currentPitchB);
      Serial.print(", ");
      Serial.println(MIDICHANNEL_B);
      midiSendOn(currentPitchB, MIDICHANNEL_B); // start a midi note
    }
  } else if (bouncerB.risingEdge()) { // check for button release / midi end
    release(1);
    Serial.println("BUTTON B DETRIGGER");
    if (midiEnable) {
      int currentPitchB = getPitch(currentPulseB); // - 24; //
      Serial.print("MIDI OFF ");
      Serial.print(currentPitchB);
      Serial.print(", ");
      Serial.println(MIDICHANNEL_B);
      midiSendOff(currentPitchB, MIDICHANNEL_B); // end a midi note
    }
  }

  if (buttonAState || midiState) { // find button held / midi held
    sustain(0);
  }
  if (buttonBState || midiState) { // find button held / midi held
    sustain(1);
  }
}



void collisionDetect(int i, int vect) {
  if (pulses[i].vector == vect) { // check for this vector only
    int startA = pulses[i].position; // get data for this pulse
    int endA = pulses[i].end;
    for (int j = 0; j < NUMPULSES; j++) { // step through pulse object array
      if (pulses[j].active) { // check for active pulses only
        if (pulses[j].vector != vect) { // check for other vectors only
          //          Serial.println(j);
          int startB = pulses[j].position; // get data for this other pulse
          int endB = pulses[j].end;
          if (startA >= startB) {
            //              Serial.println("HERE");/
            if (endA - SPRITELENGTH <= endB + SPRITELENGTH) { // check for crossover
              Serial.println("COLLISION DETECTED");
              pulses[i].collision = true; // flag as this pulse colliding
              pulses[i].colliding = pulses[j].index; // update pointer to other pulse
              pulses[j].collision = true; // flag as other pulse colliding
              pulses[j].colliding = pulses[i].index; // update pointer to this pulse
            }
            else { // if no collision, update collision flag to false
              pulses[i].collision = false;
              pulses[j].collision = false;
            }
          }
        }
      }
    }
    //    Serial.print(i);
    //    Serial.print(", ");
    //    Serial.print(pulses[i].collision);
    //    Serial.print(", ");
    //    Serial.println(pulses[i].prevCollision);
    if (pulses[i].collision == true && pulses[i].prevCollision == false) { // check for collision state change rising
      Serial.println("COLLISION START");
      if (midiEnable) {
        int collidingPitchA = getPitch(pulses[i].index); // + 24; // current pulse pitch
        int collidingPitchB = getPitch(pulses[i].colliding); // + 24; // colliding pulse pitch
        Serial.print("MIDI ON ");
        Serial.print(collidingPitchA);
        Serial.print(", ");
        Serial.println(MIDICHANNEL_CA);
        Serial.print("MIDI ON ");
        Serial.print(collidingPitchB);
        Serial.print(", ");
        Serial.println(MIDICHANNEL_CB);
        midiSendOn(collidingPitchA, MIDICHANNEL_CA); // start a midi note
        midiSendOn(collidingPitchB, MIDICHANNEL_CB); // start a midi note
      }
    }
    if (pulses[i].collision == false && pulses[i].prevCollision == true) { // check for collision state change falling
      Serial.println("COLLISION END");
      if (midiEnable) {
        int collidingPitchA = getPitch(pulses[i].index); // + 24; // current pulse pitch
        int collidingPitchB = getPitch(pulses[i].colliding); // + 24; // colliding pulse pitch
        Serial.print("MIDI OFF ");
        Serial.print(collidingPitchA);
        Serial.print(", ");
        Serial.println(MIDICHANNEL_CA);
        Serial.print("MIDI OFF ");
        Serial.print(collidingPitchB);
        Serial.print(", ");
        Serial.println(MIDICHANNEL_CB);
        midiSendOff(collidingPitchA, MIDICHANNEL_CA); // stop a midi note
        midiSendOff(collidingPitchB, MIDICHANNEL_CB); // stop a midi note
      }
    }

  }
  pulses[i].prevCollision = pulses[i].collision; // update prevCollision flag
}
