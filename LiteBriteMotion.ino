#include <avr/sleep.h>

#define _Debug 1
#define DebugBaudRate 115200
#define ReadyWarmupTime 10000
#define MotionRearmDelay 60000 * 1
#define RelayPulseTime 500

#define PinRelayOut 7
#define PinMotionIn 19

#define PinLampWarmup 3
#define PinLampMotion 4

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// rename these to EnableWhatever to enable them.
// rename these to DisableWhatever to disable them.

#define EnableSleepMode 1
#define DisableSoftwareMotionRearm 1

/*******************************************************************************
**** SLEEP NOTES ***************************************************************

the mega 2560 while having a good number of interupt pins...
- 2, 3, 18, 19, 20, 21

pins 2 and 3 do not seem to respond to RISING/FALLING/CHANGE states.

https://forum.arduino.cc/t/329291
https://forum.arduino.cc/t/862773

additionally, the function called during wake interupt seems to be turbo limited
in what it can do, maybe because its getting called before the board fully wakes
up. but the only way it was working how i wanted it to work was to put all the
things i wanted in awaken() to just be after the sleep call where it continues
where it left off.

sleepmode will pause the clock which makes the software timer pointless
so sleep mode will be disabled when software timer is enabled. the fix for that
would be to add the rtc module that has it own little watch battery lol. im sure
at some point that will happen because if i'm going to make the worlds most
expensive lite-brite might as well go all in.

*******************************************************************************/

void setup(void);
void loop(void);
void awaken(void);
void sleepytime(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

unsigned long Now;
bool Ready;

unsigned short WasMotionOn;
unsigned short IsMotionOn;
unsigned long WhenMotionOn;
bool HasMotionedOnce;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void setup() {

	WasMotionOn = LOW;
	IsMotionOn = LOW;
	WhenMotionOn = 0;
	HasMotionedOnce = false;

	Now = 0;
	Ready = false;

	////////

	// output pins
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(PinRelayOut, OUTPUT);
	pinMode(PinLampWarmup, OUTPUT);
	pinMode(PinLampMotion, OUTPUT);

	// input pins
	pinMode(PinMotionIn, INPUT);

	// turn the warmup and status indicators on.
	digitalWrite(LED_BUILTIN, HIGH);
	digitalWrite(PinLampWarmup, HIGH);

	////////

	#ifdef Debug
	Serial.begin(DebugBaudRate);
	Serial.println("hello.");
	Serial.print("Warmup Delay: ");
	Serial.print(ReadyWarmupTime);
	Serial.print(", Rearm Delay: ");
	Serial.print(MotionRearmDelay);
	Serial.print("\n");
	#endif

	return;
};

void loop() {

	Now = millis();

	// the PIR sensor actually needs some time to warm up before
	// it can spit out half way reasonable results. so while we
	// wait for that we will bail early.

	if(!Ready) {
		if(Now > ReadyWarmupTime) {
			#ifdef Debug
			Serial.println("sensor warmup complete.");
			#endif

			digitalWrite(PinLampWarmup, LOW);
			Ready = true;
		}

		return;
	}

	// check the motion sensor.

	IsMotionOn = digitalRead(PinMotionIn);

	// if we were not motioned before but we are now then we have
	// tripped and detected movement.

	if(WasMotionOn == LOW && IsMotionOn == HIGH) {

		#ifdef Debug
		Serial.println("motion sensor tripped.");
		#endif

		// handle if we managed to hit the 50 days of uptime

		if(Now < WhenMotionOn)
		WhenMotionOn = UINT32_MAX - WhenMotionOn;

		// handle not retriggering if the toy is likely still on.
		// multiple button presses put the toy in different modes.

		#ifdef EnableSoftwareMotionRearm
		if(HasMotionedOnce && ((Now - WhenMotionOn) <= MotionRearmDelay)) {

			#ifdef Debug
			Serial.print("motion too soon: ");
			Serial.println(Now - WhenMotionOn);
			#endif

			digitalWrite(PinLampMotion, HIGH);
			WasMotionOn = HIGH;
			return;
		}
		#endif

		digitalWrite(PinLampMotion, HIGH);
		digitalWrite(PinRelayOut, HIGH);
		delay(RelayPulseTime);
		digitalWrite(PinRelayOut, LOW);

		WhenMotionOn = Now;
		WasMotionOn = HIGH;
		HasMotionedOnce = true;
		return;
	}

	// if we were motioned before but we are no longer then the
	// sensor has gone back into idle.

	if(WasMotionOn == HIGH && IsMotionOn == LOW) {

		#ifdef Debug
		Serial.print("motion sensor idle after ");
		Serial.println( (((double)(Now - WhenMotionOn) / 1000.0) / 60.0) );
		#endif

		digitalWrite(PinLampMotion, LOW);

		WasMotionOn = LOW;
		return;
	}

	// if nothing has happened then go to sleep.

	#ifdef EnableSleepMode
	#ifndef EnableSoftwareMotionRearm
	sleepytime();
	#endif
	#endif

	return;
};

void sleepytime() {

	#ifdef Debug
	// the delay prevents debug output from getting turboborked seemingly
	// due to it going to sleep partway through the serial transmission.
	// guess that is asyncish.
	Serial.println("sleepytime.");
	delay(250);
	#endif

	digitalWrite(LED_BUILTIN, LOW);
	sleep_enable();
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	attachInterrupt(
		digitalPinToInterrupt(PinMotionIn),
		awaken,
		(WasMotionOn ? FALLING : RISING)
	);

	sleep_mode();

	detachInterrupt(digitalPinToInterrupt(PinMotionIn));

	sleep_disable();
	digitalWrite(LED_BUILTIN, HIGH);

	return;
};

void awaken() {

	return;
};

