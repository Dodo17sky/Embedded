#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
String GoogleFingerprint = "EE AA 58 6D 4F 1F 42 F4 18 5B 7F B0 F2 0A 4C DD 97 47 7D 99";
String Titan_Url = "http://api.openweathermap.org/data/2.5/weather?lat=44.42&lon=26.18&units=metric&APPID=6fe336bc9dcf6fa6f3e35f78551fa62e";

#define SSID        "---"
#define PASSWORD    "---"

String last_Desc;
String last_Temp;
String last_Wind_speed;
int last_Humi = 0;

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

    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        http.begin(Titan_Url);

        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {

            // file found at server
            if(httpCode == HTTP_CODE_FOUND || httpCode == HTTP_CODE_OK) {
            	String payload = http.getString();

            	DynamicJsonDocument doc(1024);
            	deserializeJson(doc, payload.c_str());

            	const char* desc = doc["weather"][0]["description"];
            	float temp = doc["main"]["temp"];
            	float wind_speed = doc["wind"]["speed"];
            	int humi = doc["main"]["humidity"];

            	String Desc = String(desc);
            	String Temp = String(temp);
            	String Wind_speed = String(wind_speed);

            	if (Desc != last_Desc)
            	{
            		USE_SERIAL.print("Description: ");
            		USE_SERIAL.println(Desc);
            		last_Desc = Desc;
            	}

            	if (Temp != last_Temp)
            	{
					USE_SERIAL.print("Temperature: ");
					USE_SERIAL.println(Temp);
					last_Temp = Temp;
            	}

            	if (Wind_speed != last_Wind_speed)
				{
					USE_SERIAL.print("Wind speed: ");
					USE_SERIAL.println(Wind_speed);
					last_Wind_speed = Wind_speed;
				}

            	if (abs(humi-last_Humi) > 0)
            	{
					USE_SERIAL.print("Humidity: ");
					USE_SERIAL.println(humi);
					last_Humi = humi;
            	}
            }

        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(20000);
}
