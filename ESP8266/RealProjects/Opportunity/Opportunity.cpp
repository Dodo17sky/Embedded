#include <Arduino.h>
#include "Serial.h"
#include "ComManager.h"
#include "Controller.h"

void setup() {
	Serial_InitRunnable();
	ComM_InitRunnable();
	Controller_InitRunnable();
}

void loop() {
	Serial_MainRunnable();
	ComM_MainRunnable();
	Controller_MainRunnable();
}
