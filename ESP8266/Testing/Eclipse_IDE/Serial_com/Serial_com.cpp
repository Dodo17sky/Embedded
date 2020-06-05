#include <Arduino.h>

#define USER_LED		D8

char c;

void setup() {
	Serial.begin(115200);
	pinMode(USER_LED, OUTPUT);
}

void loop() {

	if(Serial.available() ) {
		c = Serial.read() ;

		if(c == '0')
		{
			Serial.println("ESP8266 led = 0");
			digitalWrite(USER_LED, LOW);
		}
		if(c == '1')
		{
			Serial.println("ESP8266 led = 1");
			digitalWrite(USER_LED, HIGH);
		}
	}

}
