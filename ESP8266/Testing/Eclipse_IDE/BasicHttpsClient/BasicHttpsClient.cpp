#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
String GoogleFingerprint = "EA E0 8E 6A AC 0F BA 1C 8C F1 16 E7 41 89 BD 4B 8E 18 27 43";
String base_Url = "https://script.google.com/macros/s/AKfycbxHYNfpfeJ4k4rW5PuDVML5FzkZOzy5PodMi9lRJ0RT7RQyJsE/exec?";
String Parameters = "tag=Temp&value=";
int TempValue = 0;

#define SSID        "---"
#define PASSWORD    "---"

void setup() {
    USE_SERIAL.begin(115200);
    USE_SERIAL.println();

    pinMode(0, OUTPUT);

    for(uint8_t t = 7; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
    WiFiMulti.addAP(SSID, PASSWORD);
}

void loop() {
    TempValue++;
    Parameters = String("tag=Temp&value=") + String(TempValue);
    String URL = base_Url + Parameters;

    if(TempValue%2 == 0) {
    	digitalWrite(0, HIGH);
    }
    else {
    	digitalWrite(0, LOW);
    }

    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        http.begin(URL, GoogleFingerprint);

        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET status: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.printf("[HTTP] GET data: %s\n", payload.c_str());
            }
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                USE_SERIAL.printf("[HTTP] GET data: %s\n", payload.c_str());
            }

        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(4000);
}
