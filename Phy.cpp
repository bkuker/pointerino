#include "Phy.h"

#define AZ_DIR 8
#define AZ_STEP 9
#define AZ_ZERO 12
#define AZ_DELAY 1000

#define ALT_DIR 10
#define ALT_STEP 11
#define ALT_ZERO 13
#define ALT_DELAY 500

#define STEP(PIN, DIR) {\
		digitalWrite(PIN##_DIR, DIR); \
		digitalWrite(PIN##_STEP, HIGH); \
		delayMicroseconds(PIN##_DELAY); \
		digitalWrite(PIN##_STEP, LOW); \
		delayMicroseconds(PIN##_DELAY); \
}

#define ZERO(PIN) !digitalRead(PIN##_ZERO)

#define ALT_MIN -3500
#define ALT_CIR 7000
#define ALT_MAX 3500

#define AZ_MIN -3200
#define AZ_CIR 6400
#define AZ_MAX 3200

Phy::Phy() :
		alt_cur(0), az_cur(0) {
	pinMode(ALT_DIR, OUTPUT);
	pinMode(ALT_STEP, OUTPUT);
	digitalWrite(ALT_STEP, LOW);
	digitalWrite(ALT_DIR, LOW);
	pinMode(ALT_ZERO, INPUT);
	digitalWrite(ALT_ZERO, HIGH);

	pinMode(AZ_DIR, OUTPUT);
	pinMode(AZ_STEP, OUTPUT);
	digitalWrite(AZ_STEP, LOW);
	digitalWrite(AZ_DIR, LOW);
	pinMode(AZ_ZERO, INPUT);
	digitalWrite(AZ_ZERO, HIGH);
}

void Phy::moveTo(float altD, float azD) {
	int alt = ALT_CIR * (altD / 360.0);
	int az = AZ_CIR * (azD / 360.0);
	Serial.println(alt);
	stepTo(alt, az);
}

void Phy::stepTo(int alt, int az) {
	if (alt > ALT_MAX || alt < ALT_MIN || az > AZ_MAX || az < AZ_MIN) {
		Serial.println("LIMIT, HALTED");
		while (true) {
		}
	}
	while (alt_cur != alt || az_cur != az) {
		if (alt_cur != alt) {
			STEP(ALT, alt_cur < alt ? 0 : 1)
			alt_cur = alt_cur + (alt_cur < alt ? 1 : -1);
		}
		if (az_cur != az) {
			STEP(AZ, az_cur < az ? 0 : 1);
			az_cur = az_cur + (az_cur < az ? 1 : -1);
		}
	}
}

void Phy::zero() {
	Serial.println("Zeroing ALT...");
	for (int i = 0; i < 1000; i++)
		STEP(ALT, 0);
	for (int i = 0; i < 2000 && !ZERO(ALT); i++)
		STEP(ALT, 1);

	Serial.println("Zero AZ...");
	for (int i = 0; i < 500; i++)
		STEP(AZ, 0);
	for (int i = 0; i < 1000 && !ZERO(AZ); i++)
		STEP(AZ, 1);

	if (!ZERO(ALT)) {
		Serial.println("Zero ALT failed.");
	}
	if (!ZERO(AZ)) {
		Serial.println("Zero AZ failed.");
	}

	if (!ZERO(ALT) || !ZERO(AZ))
		while (true) {
		}

	Serial.println("Zero OK...");
}
