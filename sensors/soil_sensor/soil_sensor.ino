/**

  Soil sensor

*/

#include <Arduino.h>
#include "config.h"

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

int WiFiStrength = 0;

const char* ssid = SECERT_WIFI_SSID;
const char* password = SECERT_WIFI_PASSWORD;
const char* serverIp =  SERVER_IP;

// big flag to jiggle some debug params
bool debugMode = false;

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(debugMode);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

}

int analogValue = 0.0;
double analogVolts = 0.0;

void loop() {
    Serial.println("Looping");

    if ((WiFi.status() == WL_CONNECTED)) {
      analogValue = analogRead(A0); // read the analog signal
      Serial.printf("Raw Analog is: %d\n", analogValue);
  
      WiFiClient client;
      HTTPClient http;
  
      Serial.print("[HTTP] begin...\n");
      // configure traged server and url
      String route = (String)"http://" + serverIp  + "/api/mqtt/soil";
      http.begin(client, route); //HTTP
      http.addHeader("Content-Type", "application/json");
  
      Serial.print("[HTTP] POST...\n");
      // start connection and send HTTP header and body
      String payload = (String)"{\"soilAnalog\":" + analogValue  + "}";
      int httpCode = http.POST(payload);
  
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  
        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          const String& payload = http.getString();
          Serial.println("received payload:\n<<");
          Serial.println(payload);
          Serial.println(">>");
        }
      } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
  
      http.end();
  }

  if (debugMode) {
    delay(1000);
  }
  else {
    delay(300000); // TODO: vastly increase this in production
  }
}
