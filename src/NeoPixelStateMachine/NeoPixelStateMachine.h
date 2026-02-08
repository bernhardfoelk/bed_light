#pragma once
#include <Adafruit_NeoPixel.h>
#include <Datatypes.h>

class statemachineNeoPixel {
  private: 
    Adafruit_NeoPixel pixels;
    EventNeoPixel currentEvent;

    // local variables
    int liNoOfPixels;
    int liStep = 0;
    int liLStep = -1;

    int liActFadeValueRed = 0;
    int liActFadeValueGreen = 0;
    int liActFadeValueBlue = 0;

    unsigned long liStartTime = 0;

    int fadeColour(int iiActualValue, int iiSetValue, int iiStep);

  public:
    statemachineNeoPixel(int iiNoOfPixels, int iiPin);
    void handleEvent(EventNeoPixel ev);
    void tick(int iiRed, int iiGreen, int iiBlue, int iiBrightness, int iiStep, int iiTime);
};


// ========== Vorlage ==========
/*
class statemachineNeoPixel {
  private: 
    Adafruit_NeoPixel pixels;
    EventNeoPixel currentEvent;

    // local variables --> general
    int liNoOfPixels;
    int liStep = 0;

    int liActFadeValueRed = 0;
    int liActFadeValueGreen = 0;
    int liActFadeValueBlue = 0;

    unsigned long liStartTime = 0;

  public:
    statemachineNeoPixel(int iiNoOfPixels, int iiPin)
      : pixels(iiNoOfPixels, iiPin, NEO_GRB + NEO_KHZ800), currentEvent(EventNeoPixel::Off) {

      // write general local variables
      liNoOfPixels = iiNoOfPixels;

      pixels.begin();

      handleEvent(EventNeoPixel::Off);
    }

    void handleEvent(EventNeoPixel ev) {
      if (ev == EventNeoPixel::On) {liStep = 1000;}
      if (ev == EventNeoPixel::Off) {liStep = 2000;}
      if (ev == EventNeoPixel::StartUp) {liStep = 3000;}
      if (ev == EventNeoPixel::FadeInOut) {liStep = 4000;}
      if (ev == EventNeoPixel::Timer) {liStep = 5000;}
    }

    void tick(int iiRed, int iiGreen, int iiBlue, int iiBrightness, int iiStep, int iiTime){
      int tiSetValueRed = (iiRed * iiBrightness) / 100;
      int tiSetValueGreen = (iiGreen * iiBrightness) / 100;
      int tiSetValueBlue = (iiBlue * iiBrightness) / 100;
     
      switch (liStep) {
        // ==========
        case 0:
          break;

        // ==========
        case 1000: // --> on

          Serial.printf("In step 'ON'\n");

          for (int i = 0; i < liNoOfPixels; i++){
            pixels.setPixelColor(i, pixels.Color(tiSetValueGreen, tiSetValueRed, tiSetValueBlue));
          }
          pixels.show();  

          Serial.printf("Going to 'IDLE'\n");
          liStep = 0;
          break;

        // ==========
        case 2000: // --> off
          Serial.printf("In step 'OFF'\n");

          pixels.clear();
          pixels.show();

          Serial.printf("Going to 'IDLE'\n");
          liStep = 0;
          break;
        
        // ==========
        case 4000: // --> fade in out

          // Fade for each colour
          if (liActFadeValueRed < tiSetValueRed) {
              liActFadeValueRed = min(liActFadeValueRed + iiStep, tiSetValueRed);
          } 
          else if (liActFadeValueRed > tiSetValueRed) {
              liActFadeValueRed = max(liActFadeValueRed - iiStep, tiSetValueRed);
          }

          if (liActFadeValueGreen < tiSetValueGreen) {
              liActFadeValueGreen = min(liActFadeValueGreen + iiStep, tiSetValueGreen);
          } 
          else if (liActFadeValueGreen > tiSetValueGreen) {
              liActFadeValueGreen = max(liActFadeValueGreen - iiStep, tiSetValueGreen);
          }

          if (liActFadeValueBlue < tiSetValueBlue) {
              liActFadeValueBlue = min(liActFadeValueBlue + iiStep, tiSetValueBlue);
          } 
          else if (liActFadeValueBlue > tiSetValueBlue) {
              liActFadeValueBlue = max(liActFadeValueBlue - iiStep, tiSetValueBlue);
          }

          // set neopixel
          for (int i = 0; i < liNoOfPixels; i++) {
              pixels.setPixelColor(i, pixels.Color(liActFadeValueGreen, liActFadeValueRed, liActFadeValueBlue));
          }
          pixels.show();

          // Debug
          //Serial.print("Red: "); Serial.print(liActFadeValueRed);
          //Serial.print(", Green: "); Serial.print(liActFadeValueGreen);
          //Serial.print(", Blue: "); Serial.println(liActFadeValueBlue);

          // Check state
          if (liActFadeValueRed == tiSetValueRed &&
              liActFadeValueGreen == tiSetValueGreen && 
              liActFadeValueBlue == tiSetValueBlue)  {
              Serial.println("Going to IDLE");
              liStep = 0;
          }

          break;
        
        // ==========
        case 5000: // --> timer

          // Fade for each color
          if (liActFadeValueRed < tiSetValueRed) {
              liActFadeValueRed = min(liActFadeValueRed + iiStep, tiSetValueRed);
          } 
          else if (liActFadeValueRed > tiSetValueRed) {
              liActFadeValueRed = max(liActFadeValueRed - iiStep, tiSetValueRed);
          }

          if (liActFadeValueGreen < tiSetValueGreen) {
              liActFadeValueGreen = min(liActFadeValueGreen + iiStep, tiSetValueGreen);
          } 
          else if (liActFadeValueGreen > tiSetValueGreen) {
              liActFadeValueGreen = max(liActFadeValueGreen - iiStep, tiSetValueGreen);
          }

          if (liActFadeValueBlue < tiSetValueBlue) {
              liActFadeValueBlue = min(liActFadeValueBlue + iiStep, tiSetValueBlue);
          } 
          else if (liActFadeValueBlue > tiSetValueBlue) {
              liActFadeValueBlue = max(liActFadeValueBlue - iiStep, tiSetValueBlue);
          }

          // set neopixel
          for (int i = 0; i < liNoOfPixels; i++) {
              pixels.setPixelColor(i, pixels.Color(liActFadeValueGreen, liActFadeValueRed, liActFadeValueBlue));
          }
          pixels.show();

          // Debug
          //Serial.print("Red: "); Serial.print(liActFadeValueRed);
          //Serial.print(", Green: "); Serial.print(liActFadeValueGreen);
          //Serial.print(", Blue: "); Serial.println(liActFadeValueBlue);

          // Check state
          if (liActFadeValueRed == tiSetValueRed &&
              liActFadeValueGreen == tiSetValueGreen && 
              liActFadeValueBlue == tiSetValueBlue)  {
              liStartTime = millis(); // safe time
              liStep = 5010;
          }

          break;

        case 5010:

          if ((millis() - liStartTime ) >= (iiTime * 1000)){
            liStep = 5020;
          }
          break;
        
        case 5020: 

          // Fade for each color
          if (liActFadeValueRed < 0) {
              liActFadeValueRed = min(liActFadeValueRed + iiStep, 0);
          } 
          else if (liActFadeValueRed > 0) {
              liActFadeValueRed = max(liActFadeValueRed - iiStep, 0);
          }

          if (liActFadeValueGreen < 0) {
              liActFadeValueGreen = min(liActFadeValueGreen + iiStep, 0);
          } 
          else if (liActFadeValueGreen > 0) {
              liActFadeValueGreen = max(liActFadeValueGreen - iiStep, 0);
          }

          if (liActFadeValueBlue < 0) {
              liActFadeValueBlue = min(liActFadeValueBlue + iiStep, 0);
          } 
          else if (liActFadeValueBlue > 0) {
              liActFadeValueBlue = max(liActFadeValueBlue - iiStep, 0);
          }

          // set neopixel
          for (int i = 0; i < liNoOfPixels; i++) {
              pixels.setPixelColor(i, pixels.Color(liActFadeValueGreen, liActFadeValueRed, liActFadeValueBlue));
          }
          pixels.show();

          // Debug
          //Serial.print("Red: "); Serial.print(liActFadeValueRed);
          //Serial.print(", Green: "); Serial.print(liActFadeValueGreen);
          //Serial.print(", Blue: "); Serial.println(liActFadeValueBlue);

          // Check state
          if (liActFadeValueRed == 0 &&
              liActFadeValueGreen == 0 && 
              liActFadeValueBlue == 0)  {

              Serial.println("Going to IDLE");
              liStep = 0;
          }

          break;
      }
    }
};
*/