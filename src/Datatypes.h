#pragma once

/*******************************************************
 * STRUCT FOR LED COMMAND
 *******************************************************/
struct sLedCommand {
  int iBrightness;
  int iRed;
  int iGreen;
  int iBlue;
  int iTime;
};

/*******************************************************
 * ENUM for state machine
 *******************************************************/
enum class EventNeoPixel : int {
    On=1, 
    Off=2, 
    StartUp=3, 
    FadeInOut=4, 
    Timer=5
};