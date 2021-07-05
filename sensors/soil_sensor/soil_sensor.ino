/*
   AskSensors IoT Platform.
   Description: Soil moisture monitoring using ESP8266 and the AskSensors IoT cloud.
    Author: https://asksensors.com, 2018 - 2020
    github: https://github.com/asksensors/AskSensors-ESP8266-Moisture
*/

#include "config.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

int LED = 2; // Assign LED pin i.e: D1 on NodeMCU
const char* wifi_ssid = SECRET_SSID;             // SSID
const char* wifi_password = SECRET_PASS;         // WIFI
const char* apiKeyIn = SECRET_DEVICE_KEY;      // API KEY IN

// ASKSENSORS config.
String host = "http://api.asksensors.com";         // ASKSENSORS API host name

ESP8266WiFiMulti WiFiMulti;

int moisture_Pin = 0; // Soil Moisture Sensor input at Analog PIN A0
int moisture_value = 0, moisture_state = 0xFF;


// flag to change some debug params
bool debugMode = false;

void setup() {
  Serial.begin(115200);
  Serial.println("********** Program Start : Soil Moisture monitoring using ESP8266 and AskSensors IoT cloud");
  Serial.println("Wait for WiFi... ");

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW); // turn the LED on

  Serial.print("********** connecting to WIFI : ");
  Serial.println(wifi_ssid);
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("-> WiFi connected");
  Serial.println("-> IP address: ");
  Serial.println(WiFi.localIP());

  readAndPostSensorValue();
  if (debugMode) {
    // 20 seconds
    Serial.println("Short sleep");
    digitalWrite(LED, LOW); // turn the LED off
    ESP.deepSleep(2e7);
  }
  else {
    // 30 minute sleep
    Serial.println("Long sleep");
    digitalWrite(LED, HIGH); // turn the LED off
    ESP.deepSleep(1.8e9);
  }
}


void readAndPostSensorValue() {
  Serial.print("MOISTURE LEVEL : ");
  moisture_value = analogRead(moisture_Pin);
  Serial.println(moisture_value);

  // wait for WiFi connection
  if (WiFi.status() == WL_CONNECTED) {

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");

    // Create a URL for the request
    String url = "";
    url += host;
    url += "/write/";
    url += apiKeyIn;
    url += "?module1=";
    url += moisture_value;

    Serial.print("********** requesting URL: ");
    Serial.println(url);
    http.begin(client, url); //HTTP

    Serial.println("> Soil moisture level and state were sent to ASKSENSORS");

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        blink();
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

    Serial.println("********** End");
  }
}

void blink() {
  digitalWrite(LED, HIGH); // LED off
  delay(500);
  digitalWrite(LED, LOW); // LED on
  delay(500);
  digitalWrite(LED, HIGH); // LED off
  delay(500);
  digitalWrite(LED, LOW); // LED on
  delay(500);
  digitalWrite(LED, HIGH); // LED off
  delay(500);
  digitalWrite(LED, LOW); // LED on
}

void loop () {}
