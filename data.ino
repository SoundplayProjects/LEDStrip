
uint8_t sprite[] = {23, 15, 9, 4, 2, 1}; // shape of sprite in brightness values (0-31)

int data[SAMPLES][PARAMETERS]; // initialise the data array{serial,pixel,r,g,b}

uint8_t palette[COLOURS][3] = { // initialise the colour palette array{r,g,b} (0-7)
  {0, 0, 7},
//  {0, 0, 5},
  {0, 0, 3},
  {0, 7, 7},
  {0, 3, 3},
//  {0, 5, 5},
  {0, 3, 7},
  {0, 7, 3},
//  {0, 5, 7},
//  {0, 7, 5},
//  {0, 5, 3},
//  {0, 3, 5},
//  {5, 5, 5},
//  {3, 3, 3},
  {7, 7, 7},
};

void dataSetup() {
  for (int i = 0; i < SAMPLES; i++) { // initilalise the serial and pixel parameters for each sample
    data[i][0] = i; // set serial number to pixel position
    data[i][1] = NUMPIXELS; // set initial pixel position to start off-strip
    data[i][2] = 0; // initialise. colour is assigned on trigger
    data[i][3] = 0; // initialise. colour is assigned on trigger
    data[i][4] = 0; // initialise. colour is assigned on trigger
    data[i][5] = 1; //set duration to 1
    data[i][6] = 0; //set velocity to 0
  }
}

void incrementPixels() {
  for (int i = 0; i < SAMPLES; i++) { //step through samples
    data[i][1] = data[i][1] + 1;  // increment position
    //    Serial.println(data[i][1]);
    data[i][1] = min(data[i][1], NUMPIXELS); // constrain position to the strip length
  }
}

void printAll() { // prints all live data
//  Serial.print(knob);
//  Serial.print(", ");
//  Serial.print(sensor);
  for (int i = 0; i < SAMPLES; i++) {
    Serial.print(", ");
    Serial.print(data[i][1]);
  }
  Serial.println(", ");
}
