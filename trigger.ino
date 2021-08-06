void triggerDecide() {
  if (bouncer1.fallingEdge() || midiRisingEdge() || audioRisingEdge()) { // check for button press / midi start / audio on
    int velocity = 31;
    if (leftV > 0) {
      //      Serial.println(leftRMS);
      velocity = leftV;
    }
    triggerRoutine(velocity); // do the triggering stuff
  } else if (bouncer1.risingEdge() || midiFallingEdge() || audioFallingEdge()) { // check for button release / midi end / audio end
    deTriggerRoutine();//SHOULD THIS BE NOT AN 'ELSE', AND SHOULD IT BE AFTER THE 'HOLD' CHECK BELOW?
  }
  if (buttonState || midiState || audioState ) { // find button held / midi held / audio on
    int velocity = 31;
    //    if (leftV > 0) {
    //      //      Serial.println(leftRMS);
    //      velocity = leftV;
    //    }
    holdRoutine(velocity);
  }
}

void triggerDecideResponder(int responderPixel) {
  int velocity = 31;
  for (int j = 0; j < SAMPLES; j++) { // step through each sample
    int pixel = data[j][1]; // get index no
    if (pixel < NUMPIXELS) { // check if sample is active
      int sampleDuration = data[j][5]; // get duration
      if (pixel == responderPixel) {
        triggerResponderRoutine(velocity);
      }
      if (pixel > responderPixel) {
        if (pixel - sampleDuration < responderPixel) {
//          holdRoutine(velocity);
        }
      }
      if (pixel - sampleDuration == responderPixel){
      deTriggerResponderRoutine();
      }
    }
  }
}

void triggerRoutine(int v) {
  //  Serial.println(v);
  Serial.println("TRIGGER");
  int seed = random(0, COLOURS);
  currentPitch = pitch[seed % PITCHES];
  Serial.println(currentPitch);
  midiSendOn(currentPitch); // start a midi note
  //  Serial.println(seed);
  //  Serial.println(nextSample);
  clearPixels(data[nextSample][1]); // clear pixels at current position
  data[nextSample][1] = 0; // reset pixel position to start
  data[nextSample][2] = palette[seed][0]; // set red value from colour palette;
  data[nextSample][3] = palette[seed][1]; // set green value from colour palette;
  data[nextSample][4] = palette[seed][2]; // set blue value from colour palette;
  data[nextSample][5] = 1; // reset duration of current sample
  data[nextSample][6] = v; // set velocity
}

void holdRoutine(int v) {
  data[nextSample][5]++; // increment duration of current sample
  data[nextSample][6] = v; // set velocity
  //    Serial.println("HOLD");
  //    Serial.println(data[nextSample][6]);
  //    Serial.println(data[nextSample - 1][5]);
}

void deTriggerRoutine() {
  lastSample = nextSample;
  nextSample = (nextSample + 1) % SAMPLES; // increment nextSample counter
  midiSendOff(currentPitch); // end a midi note
  Serial.println("DETRIGGER");
  //  Serial.println(currentPitch);
}

void triggerResponderRoutine(int v) {
  //  Serial.println(v);
  Serial.println("TRIGGER R");
//  int seed = random(0, COLOURS);
  currentResponderPitch = currentPitch +12;
  Serial.println(currentResponderPitch);
  midiSendOn(currentResponderPitch); // start a midi note
  //  Serial.println(seed);
  //  Serial.println(nextSample);
//  clearPixels(data[nextSample][1]); // clear pixels at current position
//  data[nextSample][1] = 0; // reset pixel position to start
//  data[nextSample][2] = 255; // set red value from colour palette;
//  data[nextSample][3] = 0; // set green value from colour palette;
//  data[nextSample][4] = 0; // set blue value from colour palette;
//  data[nextSample][5] = 1; // reset duration of current sample
//  data[nextSample][6] = v; // set velocity
}

void deTriggerResponderRoutine() {
//  nextSample = (nextSample + 1) % SAMPLES; // increment nextSample counter
  midiSendOff(currentResponderPitch); // end a midi note
  Serial.println("DETRIGGER R");
  //  Serial.println(currentPitch);
}
