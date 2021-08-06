#define NUMPIXELS 600 // Total length of strip (tested up to 600)
#define REFRESH 15 // delay for refresh rate
#define SAMPLES 600 // determines how many different 'pulses' or 'samples' can be held in memory at a time - max 1 per pixel (eg 600 samples / 600 pixels)
#define PARAMETERS 7 // must match size of parameter array written to in dataSetup() in data tab
#define COLOURS 7 // must match size of palette array in data tab
#define MIDICHANNEL 1 // you can choose this - stays constant

//int sensor = 0; // sensor variable
//int knob = 0; // knob variable
//int lastSensor = 0; // reference for last sensor reading
uint16_t nextSample = 0; // serial number of next sample
uint16_t lastSample = 0; // serial number of next sample
boolean buttonState = false;
uint8_t mode = 0;
boolean midiState = false;
boolean midiStateChange = false;
boolean audioState = false;
boolean audioStateChange = false;
float leftV = 0;
int rightV = 0;
uint8_t currentPitch = 0;
uint8_t currentResponderPitch = 0;

void setup() {
  Serial.begin(9600);
  Serial.print("SETUP...");
  blinkSetup();
  //  audioSetup();
  dataSetup();
  pixelsSetup();
  buttonSetup();
  midiSetup();
  modeSelectSetup();
  Serial.println("... DONE");
}

void loop() {
  blinkLoop();
  //  Serial.println(millis());
  modeSelectRoutine();
  incrementPixels();
  //  printAll();
  //  audioRead();
  midiRead();
  buttonRead();
  triggerDecide();
  renderPixels();
  delay(REFRESH); // Delay for a period of time (in milliseconds).
}

void renderPixels() {
  if (mode == 1) {
    memoryRiver();
  }
  if (mode == 2) {
    memoryRiver();
    delay(REFRESH);
  }
  if (mode == 3) {
    memoryRiver();
    delay(REFRESH);
    delay(REFRESH);
  }
  if (mode == 5) {
    builder();
  }
  if (mode == 4) {
    int responderPixel=NUMPIXELS/2;
    triggerDecideResponder(responderPixel);
    responder(responderPixel);
  }
}
