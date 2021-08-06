/* Adaptation of Stereo peak meter example, including RMS.
  assumes Audio adapter but just uses terminal so no more parts required.

  This example code is in the public domain
*/
#define GAIN 4 // amp gain

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

const int myInput = AUDIO_INPUT_LINEIN;
// const int myInput = AUDIO_INPUT_MIC;

AudioInputI2S        audioInput;         // audio shield: mic or line-in
AudioAnalyzePeak     peak_L;
AudioAnalyzePeak     peak_R;
//AudioAnalyzeRMS      rms_L;
//AudioAnalyzeRMS      rms_R;
AudioOutputI2S       audioOutput;        // audio shield: headphones & line-out
AudioAmplifier           amp1;
AudioAmplifier           amp2;

//AudioConnection c1(audioInput, 0, peak_L, 0);
//AudioConnection c2(audioInput, 1, peak_R, 0);
AudioConnection c3(audioInput, 0, amp1, 0);
AudioConnection c4(audioInput, 1, amp2, 0);
AudioConnection          c5(amp1, 0, peak_L, 0);
AudioConnection          c6(amp2, 0, peak_R, 0);
AudioConnection c7(audioInput, 0, audioOutput, 0);
AudioConnection c8(audioInput, 1, audioOutput, 1);

AudioControlSGTL5000 audioShield;


void audioSetup() {
  AudioMemory(7);
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(1.0);
  amp1.gain(GAIN);
  amp2.gain(GAIN);
}

// for best effect make your terminal/monitor a minimum of 62 chars wide and as high as you can.

elapsedMillis fps;
//uint8_t cnt = 0;


void audioRead() {
  audioStateChange = false;
  if (fps > 24) {
    if (peak_L.available() || peak_R.available()) {
//      Serial.println("RMS AVAILABLE");
      fps = 0;
      //      uint8_t leftPeak = peak_L.read() * 30.0;
      //      uint8_t rightPeak = peak_R.read() * 30.0;
      leftV = peak_L.read();
      leftV=leftV*255;
      rightV = int(abs(peak_R.readPeakToPeak()) /7);
//      Serial.print(leftV);
//      Serial.print(", ");
//      Serial.println(rightV);
//      Serial.println(freeMemory());
      //      pixelBar(leftRMS, rightRMS);
      if (leftV > 31) {
//        Serial.println("AUDIO OVER 5");
        audioReceiveOn();
      } else {
        audioReceiveOff();
      }
    }
  }
}

boolean audioRisingEdge() {
  if (audioState == true && audioStateChange == true) {
    return true;
  }
  else return false;
}

boolean audioFallingEdge() {
  if (audioState == false && audioStateChange == true) {
//    Serial.println("AUDIO FALLING");
    return true;
  }
  else return false;
}

void audioReceiveOn() {
  if (  audioState == false) {
    audioStateChange = true;
  }
  audioState = true; // set audio flag
}

void audioReceiveOff() {  
  if (  audioState == true) {
    audioStateChange = true;
  }
  audioState = false; // reset audio flag
}
