/*******************************************************
 * INCLUDES
 *******************************************************/
#include <Arduino.h>
#include <WebServer.h>
#include <htmlPages.h>
#include <WifiHandler/WifiHandler.h>
#include <NeoPixelStateMachine/NeoPixelStateMachine.h>
#include <Datatypes.h>

/*******************************************************
 * CONSTANTS
 *******************************************************/
const char* ssid = "Your_SSID";
const char* password = "Your_PW";

/*******************************************************
 * GLOBAL OBJECTS
 *******************************************************/
WifiHandler wlan(ssid, password);
statemachineNeoPixel sm_neopixel(300, 22);
WebServer server(80);

// FreeRTOS queue for LED commands
QueueHandle_t sLedQueue;

/*******************************************************
 * FUNCTION DECLARATIONS
 *******************************************************/
void handleRoot();
void handleFadeInOut();
void taskWebserver(void* pvParameters);
void taskNeoPixel(void* pvParameters);

/*******************************************************
 * SETUP
 *******************************************************/
void setup() {
  Serial.begin(9600);
  delay(1000); // for debug
  Serial.println("System starts...");

  // initialize wifi
  wlan.begin();
  while(wlan.connectionState() != 4){
    delay(500);
  }

  // define webserver routes
  server.on("/", handleRoot);
  server.on("/FadeInOut", handleFadeInOut);
  server.onNotFound(handleRoot);
  server.begin();

  // make queue with place for 5 commands
  sLedQueue = xQueueCreate(5, sizeof(sLedCommand));

  // start tasks
  xTaskCreatePinnedToCore(taskWebserver, "WebServerTask", 8192, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(taskNeoPixel, "NeoPixelTask", 8192, NULL, 1, NULL, 1);

}

/*******************************************************
 * MAIN LOOP
 *******************************************************/
void loop() {
  delay(1000);
}

/*******************************************************
 * WEB SERVER HANDLER
 *******************************************************/
void handleRoot(){
  server.send(200, "text/html", htmlPage);
}

void handleFadeInOut(){
  sLedCommand tsCmd = {0,0,0,0,0};
  String tstStatus = "";

  //Serial.println("Aufgerufene URL: " + server.uri());

  // read URL arguments
  if (server.hasArg("brightness")) {tsCmd.iBrightness = server.arg("brightness").toInt();}
  if (server.hasArg("r")) {tsCmd.iRed = server.arg("r").toInt();}
  if (server.hasArg("g")) {tsCmd.iGreen = server.arg("g").toInt();}
  if (server.hasArg("b")) {tsCmd.iBlue = server.arg("b").toInt();}
  if (server.hasArg("time")) {tsCmd.iTime = server.arg("time").toInt();}

  // log values
  //Serial.printf("From tastkWebServer!\n");
  //Serial.printf("Brightness: %d, R: %d, G: %d, B: %d, Time: %d\n",
  //              tsCmd.iBrightness, tsCmd.iRed, tsCmd.iGreen, tsCmd.iBlue, tsCmd.iTime);
  tstStatus = "RGB: " + String(tsCmd.iRed) + ", " + String(tsCmd.iGreen) + ", " + String(tsCmd.iBlue) + " - Sleep in " + String(tsCmd.iTime) + "min";

  // put command in queue
  xQueueSend(sLedQueue, &tsCmd, 0);

  // refresh page
  server.send(200, "text/plain", tstStatus);
}

/*******************************************************
 * FREERTOS TASKS
 *******************************************************/
void taskWebserver(void* pvParameters){
  while(1) {
    server.handleClient();
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void taskNeoPixel(void* pvParameters){
  sLedCommand lsCmd = {0,0,0,0,0};

  while(1){

    // check if data is in queue
    if (xQueueReceive(sLedQueue, &lsCmd, 10 / portTICK_PERIOD_MS) == pdPASS) {
     
      if(lsCmd.iTime > 0) {
        sm_neopixel.handleEvent(EventNeoPixel::Timer);
      }
      else
      {
        sm_neopixel.handleEvent(EventNeoPixel::FadeInOut);
      }   

      //Serial.printf("From taskNeoPixel!\n");
      //Serial.printf("Brightness: %d, R: %d, G: %d, B: %d, Time: %d\n",
      //              lsCmd.iBrightness, lsCmd.iRed, lsCmd.iGreen, lsCmd.iBlue, lsCmd.iTime);

    }

    sm_neopixel.tick(lsCmd.iRed, lsCmd.iGreen, lsCmd.iBlue, lsCmd.iBrightness, 2, lsCmd.iTime);
  }
}