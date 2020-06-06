#include <Arduino.h>
#include "Serial.h"
#include "ComManager.h"

void setup() {
	Serial_InitRunnable();
	ComM_InitRunnable();
}

void loop() {
	Serial_MainRunnable();
	ComM_MainRunnable();
}
