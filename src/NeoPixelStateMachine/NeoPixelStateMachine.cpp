#include "NeoPixelStateMachine.h"
#include <Arduino.h>

statemachineNeoPixel::statemachineNeoPixel(int iiNoOfPixels, int iiPin)
    : pixels(iiNoOfPixels, iiPin, NEO_GRB + NEO_KHZ800), currentEvent(EventNeoPixel::Off) {

    // write general local variables
    liNoOfPixels = iiNoOfPixels;

    pixels.begin();

    handleEvent(EventNeoPixel::Off);
}

int statemachineNeoPixel::fadeColour(int iiActualValue, int iiSetValue, int iiStep){
    if (iiActualValue < iiSetValue) {iiActualValue = min(iiActualValue + iiStep, iiSetValue);}
    if (iiActualValue > iiSetValue) {iiActualValue = max(iiActualValue - iiStep, iiSetValue);}
    return iiActualValue;
}

void statemachineNeoPixel::handleEvent(EventNeoPixel ev) {
    switch(ev){
        case EventNeoPixel::On: liStep = 1000; break;
        case EventNeoPixel::Off: liStep = 2000; break;
        case EventNeoPixel::StartUp: liStep = 3000; break;
        case EventNeoPixel::FadeInOut: liStep = 4000; break;
        case EventNeoPixel::Timer: liStep = 5000; break;
    }
}

void statemachineNeoPixel::tick(int iiRed, int iiGreen, int iiBlue, int iiBrightness, int iiStep, int iiTime){
    int tiSetValueRed = (iiRed * iiBrightness) / 100;
    int tiSetValueGreen = (iiGreen * iiBrightness) / 100;
    int tiSetValueBlue = (iiBlue * iiBrightness) / 100;
    
    switch (liStep) {
    // ==========
    case 0:
        break;

    // ==========
    case 1000: // --> on
        for (int i = 0; i < liNoOfPixels; i++){
            pixels.setPixelColor(i, pixels.Color(tiSetValueGreen, tiSetValueRed, tiSetValueBlue));
        }
        pixels.show();  

        liStep = 0;
        break;

    // ==========
    case 2000: // --> off
        pixels.clear();
        pixels.show();

        liStep = 0;
        break;
    
    // ==========
    case 4000: // --> fade in out

        // Fade for each colour
        liActFadeValueRed = fadeColour(liActFadeValueRed, tiSetValueRed, iiStep);
        liActFadeValueGreen = fadeColour(liActFadeValueGreen, tiSetValueGreen, iiStep);
        liActFadeValueBlue = fadeColour(liActFadeValueBlue, tiSetValueBlue, iiStep);

        // set neopixel
        for (int i = 0; i < liNoOfPixels; i++) {
            pixels.setPixelColor(i, pixels.Color(liActFadeValueGreen, liActFadeValueRed, liActFadeValueBlue));
        }
        pixels.show();

        // Check state
        if (liActFadeValueRed == tiSetValueRed &&
            liActFadeValueGreen == tiSetValueGreen && 
            liActFadeValueBlue == tiSetValueBlue)  {
                
            liStep = 0;
        }

        break;
    
    // ==========
    case 5000: // --> timer

        // Fade for each colour
        liActFadeValueRed = fadeColour(liActFadeValueRed, tiSetValueRed, iiStep);
        liActFadeValueGreen = fadeColour(liActFadeValueGreen, tiSetValueGreen, iiStep);
        liActFadeValueBlue = fadeColour(liActFadeValueBlue, tiSetValueBlue, iiStep);

        // set neopixel
        for (int i = 0; i < liNoOfPixels; i++) {
            pixels.setPixelColor(i, pixels.Color(liActFadeValueGreen, liActFadeValueRed, liActFadeValueBlue));
        }
        pixels.show();

        // Check state
        if (liActFadeValueRed == tiSetValueRed &&
            liActFadeValueGreen == tiSetValueGreen && 
            liActFadeValueBlue == tiSetValueBlue)  {
            liStartTime = millis(); // safe time
            liStep = 5010;
        }

        break;

    case 5010:

        if ((millis() - liStartTime ) >= (iiTime * 1000 * 60)){
            liStep = 5020;
        }
        break;
    
    case 5020: 

        // Fade for each colour
        liActFadeValueRed = fadeColour(liActFadeValueRed, 0, iiStep);
        liActFadeValueGreen = fadeColour(liActFadeValueGreen, 0, iiStep);
        liActFadeValueBlue = fadeColour(liActFadeValueBlue, 0, iiStep);

        // set neopixel
        for (int i = 0; i < liNoOfPixels; i++) {
            pixels.setPixelColor(i, pixels.Color(liActFadeValueGreen, liActFadeValueRed, liActFadeValueBlue));
        }
        pixels.show();

        // Check state
        if (liActFadeValueRed == 0 &&
            liActFadeValueGreen == 0 && 
            liActFadeValueBlue == 0)  {

            liStep = 0;
        }

        break;
    }

    if (liLStep != liStep){
        Serial.printf("Going from %d to %d.\n", liLStep, liStep);
        liLStep = liStep;
    }
}