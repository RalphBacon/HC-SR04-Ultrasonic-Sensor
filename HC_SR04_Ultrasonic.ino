#include "Arduino.h"
#include <Wiring_private.h>

// HC-SR04 pins for trigger out and echo back in
#define trig 8
#define echo 9

// µSeconds before we abort the receive (divide by 1000 to get ms)
unsigned long pulseTimeout = 5000UL;

// Conversion factor - Centimetres or Inches
#define cm 28
#define inch 71
int conversionFactor;

void setup() {
	Serial.begin(9600);

	// define pin modes
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);

	// Set the conversion factor (here it is cm)
	conversionFactor = cm;
}

void loop() {
	//Send a pulse out
	digitalWrite(trig, LOW);
	delayMicroseconds(2);
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig, LOW);

	// Wait for a reply
	unsigned long distance = rsbPulseIn(echo, HIGH, pulseTimeout);

	// If we didn't 'timeout' waiting for a reply, display the converted distance
	if (distance != 0) {
		Serial.println(distance / conversionFactor / 2 );
	}

	// Just for the demo, a small delay
	delay(250);
}

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse.
 *
 * This function performs better with short pulses in noInterrupt() context
 */
unsigned long rsbPulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
	// cache the port and bit of the pin in order to speed up the
	// pulse width measuring loop and achieve finer resolution.  calling
	// digitalRead() instead yields much coarser resolution.
	uint8_t bit = digitalPinToBitMask(pin);
	uint8_t port = digitalPinToPort(pin);
	uint8_t stateMask = (state ? bit : 0);

	// convert the timeout from microseconds to a number of times through
	// the initial loop; it takes approximately 16 clock cycles per iteration
	unsigned long maxloops = microsecondsToClockCycles(timeout)/16;

	unsigned long width = countPulseASM(portInputRegister(port), bit, stateMask, maxloops);

	// prevent clockCyclesToMicroseconds to return bogus values if countPulseASM timed out
	if (width)
		return clockCyclesToMicroseconds(width * 16 + 16);
	else
		return 0;
}
