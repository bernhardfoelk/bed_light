// ========== WIFI Handler ==========

#pragma once

#include <Arduino.h>
#include <WiFi.h>

class WifiHandler {
  private:
    const char* lstSSID = "";
    const char* lstPassword = "";

    // local variables --> connectionState
    int liState = 0;

  public:
    WifiHandler(const char* istSSID, const char* istPassword);
    void begin();
    int connectionState();
};

// ========== Vorlage ==========
/*
class WifiHandler {
  private:
    const char* lstSSID = "";
    const char* lstPassword = "";

    // local variables --> connectionState
    int liState = 0;

  public:
    WifiHandler(const char* istSSID, const char* istPassword) {
      lstSSID = istSSID;
      lstPassword = istPassword;
    }

    void begin() {
      Serial.println("Initialize wifi");
      WiFi.begin(lstSSID, lstPassword);
    }

    int connectionState() {
      if ((WiFi.status() == WL_IDLE_STATUS) || (WiFi.localIP().toString() == "0.0.0.0")) {
        if (liState != 1) {Serial.println("WLAN in idle");}
        liState = 1;
        return liState;
      }
      else if (WiFi.status() == WL_CONNECT_FAILED) {
        if (liState != 2) {Serial.println("WLAN Verbindung fehlgeschlagen");}
        liState = 2;
        return liState;
      }
      else if (WiFi.status() == WL_CONNECTION_LOST) {
        if (liState != 3) {Serial.println("WLAN Verbindung verloren");}
        liState = 3;
        return liState;
      }
      else if ((WiFi.status() == WL_CONNECTED) && (WiFi.localIP().toString() != "0.0.0.0")) {
        if (liState != 4) {Serial.println("WLAN verbunden " + WiFi.localIP().toString());}
        liState = 4;
        return liState;
      }
      else {
        if (liState != 5) {Serial.println("WLAN hat ungültigen Zustand");}
        liState = 5;
        return liState;
      }
    }
};
*/