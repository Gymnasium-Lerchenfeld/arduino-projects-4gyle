#include "globals.h"
#include "CO2-Ampel.h"
#include "WiFiGateway.h"

#include "CO2-Ampel.h"
#include <WiFiManager.h>

#include <Arduino.h>

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 30000;        // Interval at which to publish sensor readings

void setup() {
    hardware_setup();
    wifi_setup();
}

/*******************************************************************/
void loop() {
  unsigned long currentMillis = millis();
  // Every X number of seconds (interval = 10 seconds) 
  // it publishes a new MQTT message
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    hardware_loop();
    String topic = "/" + String(raum) + "/co2amp2l";
    wifi_loop(topic, createJsonMsg());
  }
}

/*
* Create a JSON message as a String for MQTT
   {
    "temperature": 23.20,
    "humidity": 43.70,
   "pressure" : 1025,
   "co2": 780
**/
String createJsonMsg() {
  char str[12];
  sprintf(str, "%d", co2Wert);

  String message = "{\n";
  message = message + "\"temperature\": " + String(temp) + ",\n";
  message = message + "\"humidity\": " + String(humi) + ",\n";
  message = message + "\"pressure\": " + String(pres) + ",\n";
  message = message + "\"co2\": " + str + "\n";
  message = message + "}\n";
  return message;
}
