#include <avr/sleep.h>

#define PinRelayOut 7
#define PinLampWarmup 3
#define PinLampMotion 4
#define PinMotionIn 2

#define ReadyWarmupTime 10000

unsigned short WasMotionOn;
unsigned short IsMotionOn;
unsigned long Now;
bool Ready;

void setup() {

	WasMotionOn = LOW;
	IsMotionOn = LOW;

	Now = 0;
	Ready = false;

	////////

	// output pins
	pinMode(PinRelayOut, OUTPUT);
	pinMode(PinLampWarmup, OUTPUT);

	// input pins
	pinMode(PinMotionIn, INPUT);

	// turn the warmup lamp on.
	digitalWrite(PinLampWarmup, HIGH);

	////////

	Serial.begin(115200);
	Serial.println("hello");

	return;
};

void loop() {

	Now = millis();
	IsMotionOn = digitalRead(PinMotionIn);

	// the PIR sensor actually needs some time to warm up before
	// it can spit out half way reasonable results. so while we
	// wait for that we will bail early.

	if(!Ready) {
		if(Now > ReadyWarmupTime) {
			digitalWrite(PinLampWarmup, LOW);
			Ready = true;
		}

		return;
	}

	// if we were not motioned before but we are now then we have
	// tripped and detected movement.

	if(WasMotionOn == LOW && IsMotionOn == HIGH) {
		Serial.println("motion sensor tripped");

		digitalWrite(PinLampMotion, HIGH);
		digitalWrite(PinRelayOut, HIGH);
		delay(500);
		digitalWrite(PinRelayOut, LOW);

		WasMotionOn = HIGH;
		return;
	}

	// if we were motioned before but we are no longer then the
	// sensor has gone back into idle.

	if(WasMotionOn == HIGH && IsMotionOn == LOW) {
		Serial.println("motion sensor idle");

		digitalWrite(PinLampMotion, LOW);

		WasMotionOn = LOW;
		return;
	}

	// if nothing has happened then go to sleep.

	//sleepytime();

	return;
};

/*
void sleepytime() {

	Serial.println("sleepytime.");

	sleep_enable();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	attachInterrupt(digitalPinToInterrupt(PinMotionIn) , awaken, (IsMotionOn ? LOW : HIGH));
	digitalWrite(LED_BUILTIN, LOW);

	delay(500);
	sleep_cpu();
	return;
};

void awaken() {

	Serial.println("awaken.");
	digitalWrite(LED_BUILTIN, HIGH);
	sleep_disable();
	detachInterrupt(digitalPinToInterrupt(PinMotionIn));

	return;
};
*/
