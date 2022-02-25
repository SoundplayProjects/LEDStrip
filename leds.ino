#include <WS2812Serial.h>

#define LEDPIN 5

// Usable pins:
//   Teensy LC:   1, 4, 5, 24
//   Teensy 3.2:  1, 5, 8, 10, 31   (overclock to 120 MHz for pin 8)
//   Teensy 3.5:  1, 5, 8, 10, 26, 32, 33, 48
//   Teensy 3.6:  1, 5, 8, 10, 26, 32, 33

byte drawingMemory[NUMPIXELS * 4];       //  4 bytes per LED
DMAMEM byte displayMemory[NUMPIXELS * 16]; // 16 bytes per LED

WS2812Serial leds(NUMPIXELS, displayMemory, drawingMemory, LEDPIN, WS2812_GRBW);

uint8_t sprite[] = {23, 15, 9, 4, 2, 1}; // shape of sprite in brightness values (0-31)

uint8_t paletteA[COLOURS][4] = { // initialise the colour palette array for vector 0 {r,g,b,w} (0-7)
  {0, 1, 2, 1}, //  {0, 0, 2, 1},
  {0, 1, 2, 1}, //  {0, 0, 1, 0},
  {0, 1, 2, 1}, // {0, 1, 1, 0},
  {0, 1, 2, 1}, // {0, 1, 1, 1},
  {0, 1, 2, 1}, // {0, 2, 1, 1},
  {0, 1, 2, 1},  // {0, 1, 2, 1};
  {0, 1, 2, 1}, // {0, 0, 0, 1},
};
uint8_t paletteB[COLOURS][4] = { // initialise the colour palette array for vector 1 {r,g,b,w} (0-7)
  {1, 1, 0, 1}, //  {0, 2, 0, 1},
  {1, 1, 0, 1}, //  {0, 1, 0, 1},
  {1, 1, 0, 1}, //  {1, 1, 0, 0},
  {1, 1, 0, 1}, //  {1, 1, 0, 1},
  {1, 1, 0, 1}, //  {2, 1, 0, 1},
  {1, 1, 0, 1}, //  {1, 2, 0, 1},
  {1, 1, 0, 1}, //  {0, 0, 0, 1},
};
uint8_t paletteC[4] = {2, 0, 0, 0}; // initialise the colour palette array for collisions {r,g,b,w} (0-7)

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
  leds.setBrightness(200); // 0=off, 255=brightest
}

void colorWipe(int color, int wait_us) {
  for (int i = 0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
    leds.show();
    delayMicroseconds(wait_us);
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

void clearPixel(int i) { // clears a pixel
  leds.setPixel(i, 0, 0, 0); // off
}

void alternatePixels() { // clear every other pixel
  for (int i = 0; i < NUMPIXELS; i++) {
    if (i % 2 == 0) {
      clearPixel(i);
      //      Serial.println(i);
    }
  }
}

void render() {
  for (int j = 0; j < NUMPULSES; j++) { // step through each sample
    int pixel = pulses[j].position; // get index no
    if (pulses[j].active) { // check if sample is active

      int sampleDuration = pulses[j].hold; // get duration
      int velocity = pulses[j].velocity; // get velocity
      int vector = pulses[j].vector; // get vector
      bool collision = pulses[j].collision; // get collision state
      Pulse collidingPulse = pulses[pulses[j].colliding];

      // initialise colour variables
      int red = 0;
      int green = 0;
      int blue = 0;
      int white = 0;

      //get the colours from the collision palette
      int redC = paletteC[0];
      int greenC = paletteC[1];
      int blueC = paletteC[2];
      int whiteC = paletteC[3];

      // gamma correction
      int v2 = int(map(velocity, 0, 31, 0, 255)); // scale up to fit gamma array range
      int v3 = gammaArray[v2]; // adjusts the brightness using the gamma array
      int v4 = int(map(v3, 0, 255, 0, 31)); // scale back down to fit normal range
      velocity = v4;

      if (vector == 0) {

        //get colours from palette A
        int redA = paletteA[pulses[j].seed][0];
        int greenA = paletteA[pulses[j].seed][1];
        int blueA = paletteA[pulses[j].seed][2];
        int whiteA = paletteA[pulses[j].seed][3];

        // print hold/duration/sustain
        int red = velocity * redA;
        int green = velocity * greenA;
        int blue = velocity * blueA;
        int white = velocity * whiteA;
        for (int d = 0; d < sampleDuration; d++) { // print duration
          if (collision) {
            if (pulses[j].crossoverZone(d, collidingPulse)) { // check if this pixel is within the crossover zone (offset position, index of colliding pulse)
              red = velocity * redC;
              green = velocity * greenC;
              blue = velocity * blueC;
              white = velocity * whiteC;
              //              Serial.print(pulses[j].index);
              //              Serial.print(", ");
              //              Serial.print(d);
              //              Serial.print(", ");
              //              Serial.println("HERE");
            } else {
              red = velocity * redA;
              green = velocity * greenA;
              blue = velocity * blueA;
              white = velocity * whiteA;
            }
          }
          int position = pixel - d;
          leds.setPixel(position, red, green, blue, white); // set this pixel
          clearPixel(pixel - sampleDuration - 1); // clear trailing pixel
        }

        // print trailing sprite
        int spriteLength = sizeof(sprite) / sizeof(uint8_t);
        for (int k = 0; k < spriteLength; k++) {  // print end sprite
          int red = sprite[k] * redA;
          int green = sprite[k] * greenA;
          int blue = sprite[k] * blueA;
          int white = sprite[k] * whiteA;
          if (collision) {
            if (pulses[j].crossoverZone(k + sampleDuration, collidingPulse)) { // check if this pixel is within the crossover zone
              red = sprite[k] * redC;
              green = sprite[k] * greenC;
              blue = sprite[k] * blueC;
              white = sprite[k] * whiteC;
            } else {
              red = sprite[k] * redA;
              green = sprite[k] * greenA;
              blue = sprite[k] * blueA;
              white = sprite[k] * whiteA;
            }
          }
          // set pixel colour based on sprite shape and sample colour values
          int position = pixel - k - sampleDuration;
          leds.setPixel(position, red, green, blue, white);
          clearPixel(position - 1); // clear trailing pixel
        }
      }

      if (vector == 1) {
        //get colours from palette B
        int redB = paletteB[pulses[j].seed][0];
        int greenB = paletteB[pulses[j].seed][1];
        int blueB = paletteB[pulses[j].seed][2];
        int whiteB = paletteB[pulses[j].seed][3];

        // print hold/duration/sustain
        int red = velocity * redB;
        int green = velocity * greenB;
        int blue = velocity * blueB;
        int white = velocity * whiteB;
        for (int d = 0; d < sampleDuration; d++) { // print duration
          if (collision) {
            if (pulses[j].crossoverZone(-d, collidingPulse)) { // check if this pixel is within the crossover zone
              red = velocity * redC;
              green = velocity * greenC;
              blue = velocity * blueC;
              white = velocity * whiteC;
            } else {
              red = velocity * redB;
              green = velocity * greenB;
              blue = velocity * blueB;
              white = velocity * whiteB;
            }
          }
          int position = pixel + d;
          leds.setPixel(position, red, green, blue,  white);
          clearPixel(pixel + sampleDuration + 1); // clear trailing pixel
        }

        // print trailing sprite
        int spriteLength = sizeof(sprite) / sizeof(uint8_t);
        for (int k = 0; k < spriteLength; k++) {  // print end sprite
          int red = sprite[k] * redB;
          int green = sprite[k] * greenB;
          int blue = sprite[k] * blueB;
          int white = sprite[k] * whiteB;
          if (collision) {
            if (pulses[j].crossoverZone(-k - sampleDuration, collidingPulse)) { // check if this pixel is within the crossover zone
              red = sprite[k] * redC;
              green = sprite[k] * greenC;
              blue = sprite[k] * blueC;
              white = sprite[k] * whiteC;
            } else {
              red = sprite[k] * redB;
              green = sprite[k] * greenB;
              blue = sprite[k] * blueB;
              white = sprite[k] * whiteB;
            }
          }
          // set pixel colour based on sprite shape and sample colour values
          int position = pixel + k + sampleDuration;
          leds.setPixel(position, red, green, blue, white);
          clearPixel(position + 1); // clear trailing pixel
        }
      }
    }
  }

  //  alternatePixels();

  leds.show(); // This sends the updated pixel colors to the hardware.
}
