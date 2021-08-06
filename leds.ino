#include <WS2812Serial.h>

#define LEDPIN 5

// Usable pins:
//   Teensy LC:   1, 4, 5, 24
//   Teensy 3.2:  1, 5, 8, 10, 31   (overclock to 120 MHz for pin 8)
//   Teensy 3.5:  1, 5, 8, 10, 26, 32, 33, 48
//   Teensy 3.6:  1, 5, 8, 10, 26, 32, 33

byte drawingMemory[NUMPIXELS * 3];       //  3 bytes per LED
DMAMEM byte displayMemory[NUMPIXELS * 12]; // 12 bytes per LED

WS2812Serial leds(NUMPIXELS, displayMemory, drawingMemory, LEDPIN, WS2812_GRB);

byte gammaArray[] = {
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

void pixelsSetup() {
  leds.begin();
}


void pixelBar(int i, int k) { // bar (brightness, length)
  for (int j = 0; j < NUMPIXELS; j++) { // clears the strip
    clearPixels(j);
  }
  for (int j = 0; j < k; j++) { // decides length
    leds.setPixel(j, i, i, i); // decides brightness
  }
}

void clearStrip() {
  for (int i; i < NUMPIXELS; i++) {
    leds.setPixel(i, 0, 0, 0); // set the pixel
  }
}

void clearPixels(int i) { // clears a block of pixels
  for (int j = 0; j < sizeof(sprite) / sizeof(uint8_t); j++) {
    leds.setPixel(i - j, 0, 0, 0); // off
  }
}

void clearPixel(int i) { // clears a block of pixels
  leds.setPixel(i, 0, 0, 0); // off
}

void memoryRiver() {
  for (int j = 0; j < SAMPLES; j++) { // step through each sample
    int pixel = data[j][1]; // get index no
    if (pixel < NUMPIXELS) { // check if sample is active
      int sampleDuration = data[j][5]; // get duration
      int velocity = data[j][6]; // get velocity

      int v2 = int(map(velocity, 0, 31, 0, 255)); // scale up to fit gamma array range
      //      Serial.println(v2);
      int v3 = gammaArray[v2]; // adjusts the brightness using the gamma array
      //      Serial.println(v3);
      int v4 = int(map(v3, 0, 255, 0, 31)); // scale back down to fit normal range
      //      Serial.println(v4);
      velocity = v4;
      //      Serial.println(velocity);

      //      Serial.println(sampleDuration);
      for (int d = 0; d < sampleDuration; d++) { // print duration
        leds.setPixel(pixel, velocity * data[j][2], velocity * data[j][3], velocity * data[j][4]);
        clearPixel(pixel - sampleDuration - 1); // clear trailing pixel
        //        Serial.println(freeMemory());
      }

      int spriteLength = sizeof(sprite) / sizeof(uint8_t);
      for (int k = 0; k < spriteLength; k++) {  // print end sprite
        //      Serial.print(clearFlag);
        //              Serial.println("PRINT");
        // set pixel colour based on sprite shape and sample colour values
        leds.setPixel(pixel - k - sampleDuration, sprite[k]* data[j][2], sprite[k]* data[j][3], sprite[k]* data[j][4]);
        clearPixel(pixel - k - sampleDuration - 1); // clear trailing pixel
      }
    }
  }
  leds.show(); // This sends the updated pixel colors to the hardware.
}

void builder() {
  if (buttonState == true) { // if button is pressed
    int sampleDuration2 = int(data[nextSample][5] / 2); // get duration 
    for (int i = 0; i < sampleDuration2; i++) {
      float sampleDuration = data[nextSample][5]; // get duration - different variable to last time though
      sampleDuration = constrain(sampleDuration, 0, 300); // limit the duration range
      sampleDuration = map(sampleDuration, 0, 300, 31, 255); // scale duration to gamma range
      sampleDuration = gammaArray[int(sampleDuration)]; // adjusts the brightness using the gamma array
      sampleDuration = map(sampleDuration, 1, 255, 1, 31); // scale back down to fit brightness range
      Serial.println(sampleDuration);
      leds.setPixel((NUMPIXELS / 4) + i, int(sampleDuration * data[nextSample][2]), int(sampleDuration * data[nextSample][3]), int(sampleDuration * data[nextSample][4])); // set the pixel
      leds.setPixel((NUMPIXELS / 4) - i, int(sampleDuration * data[nextSample][2]), int(sampleDuration * data[nextSample][3]), int(sampleDuration * data[nextSample][4])); // set the pixel
    }
  }
  else
  {
    clearStrip(); // clear everything
    // as before but now for lastSample as deTriggering moves the data on in the array.
    int sampleDuration2 = int(data[lastSample][5] / 2); // get duration
    for (int i = 0; i < sampleDuration2; i++) {
      float sampleDuration = data[lastSample][5]; // get duration - different variable to last time though
      sampleDuration = constrain(sampleDuration, 0, 300); // limit the duration range
      sampleDuration = map(sampleDuration, 0, 300, 31, 255); // scale duration to gamma range
      sampleDuration = gammaArray[int(sampleDuration)]; // adjusts the brightness using the gamma array
      sampleDuration = map(sampleDuration, 1, 255, 1, 31); // scale back down to fit brightness range
      Serial.println(sampleDuration);
      leds.setPixel((NUMPIXELS / 4) + i, int(sampleDuration * data[lastSample][2]), int(sampleDuration * data[lastSample][3]), int(sampleDuration * data[lastSample][4])); // set the pixel
      leds.setPixel((NUMPIXELS / 4) - i, int(sampleDuration * data[lastSample][2]), int(sampleDuration * data[lastSample][3]), int(sampleDuration * data[lastSample][4])); // set the pixel
    }
    data[lastSample][5]=data[lastSample][5]-2; // gradually decrease the lastSample duration
  }
  leds.show(); // This sends the updated pixel colors to the hardware.
}

void responder(int responderPixel) {
  for (int j = 0; j < SAMPLES; j++) { // step through each sample
    int pixel = data[j][1]; // get index no
    if (pixel < NUMPIXELS) { // check if sample is active
      int sampleDuration = data[j][5]; // get duration
      int velocity = data[j][6]; // get velocity

      int v2 = int(map(velocity, 0, 31, 0, 255)); // scale up to fit gamma array range
      //      Serial.println(v2);
      int v3 = gammaArray[v2]; // adjusts the brightness using the gamma array
      //      Serial.println(v3);
      int v4 = int(map(v3, 0, 255, 0, 31)); // scale back down to fit normal range
      //      Serial.println(v4);
      velocity = v4;
      //      Serial.println(velocity);

      //      Serial.println(sampleDuration);
      for (int d = 0; d < sampleDuration; d++) { // print duration
        leds.setPixel(pixel, velocity * data[j][2], velocity * data[j][3], velocity * data[j][4]);
        if (pixel > responderPixel) {
          leds.setPixel(pixel, velocity * 7, 0, 0);
        }
        clearPixel(pixel - sampleDuration - 1); // clear trailing pixel
        //        Serial.println(freeMemory());
      }

      int spriteLength = sizeof(sprite) / sizeof(uint8_t);
      for (int k = 0; k < spriteLength; k++) {  // print end sprite
        //      Serial.print(clearFlag);
        //              Serial.println("PRINT");
        // set pixel colour based on sprite shape and sample colour values
        leds.setPixel(pixel - k - sampleDuration, sprite[k]* data[j][2], sprite[k]* data[j][3], sprite[k]* data[j][4]);
        if (pixel - k - sampleDuration > responderPixel) {
          leds.setPixel(pixel - k - sampleDuration, sprite[k] * 7, 0, 0);
        }
        clearPixel(pixel - k - sampleDuration - 1); // clear trailing pixel
      }
    }
  }
  leds.setPixel(responderPixel, 255, 0, 0);
  leds.setPixel(responderPixel + 1, 255, 0, 0);
  leds.setPixel(responderPixel + 2, 255, 0, 0);
  leds.show(); // This sends the updated pixel colors to the hardware.
}
