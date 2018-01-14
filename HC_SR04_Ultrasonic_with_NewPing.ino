#include "Arduino.h"
#include <NewPing.h>

// Pins for the Ultrasonic (ping) Tx and Rx
#define ECHO_PIN	9
#define TRIGGER_PIN 8
#define MAX_DISTANCE 30 //cm

// Start the NewPing
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
	Serial.begin(9600);
}

void loop() {

	// Detect any obstacle
	int distance = sonar.ping_cm();
	if (distance > 0) {
		Serial.println(distance);
	}
	delay(250);
}
