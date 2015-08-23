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

#define ALT_CIR 7070
#define ALT_MAX (ALT_CIR/2)
#define ALT_MIN (-ALT_MAX)


#define AZ_CIR 6405
#define AZ_MAX (AZ_CIR/2)
#define AZ_MIN (-AZ_MAX)

Phy::Phy() :
		alt_cur(0), az_cur(0), alt_target(0), az_target(0) {
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

float Phy::getAlt() {
	float ret = alt_cur * 360.0 / (float)ALT_CIR;
	if ( ret > 90 )
		ret = 180 - ret;
	return ret;
}

float Phy::getAz() {
	float alt = alt_cur * 360.0 / (float)ALT_CIR;
	float ret = az_cur * 360.0 / (float)AZ_CIR;
	if ( alt > 90 )
		ret = ret + 180;
	return ret;
}

void Phy::setAltAz(float altD, float azD) {
	if (azD > 180) {
		azD = azD - 180;
		altD = 90 + 90 - altD;
	}
	alt_target = ALT_CIR * (altD / 360.0);
	az_target = AZ_CIR * (azD / 360.0);
}

void Phy::tick() {
	if (alt_cur > ALT_MAX || alt_cur < ALT_MIN || az_cur > AZ_MAX
			|| az_cur < AZ_MIN) {
		return;
	}

	if (alt_cur != alt_target) {
		digitalWrite(ALT_DIR, alt_cur < alt_target ? 0 : 1);
		digitalWrite(ALT_STEP, HIGH);
		alt_cur = alt_cur + (alt_cur < alt_target ? 1 : -1);
	}
	if (az_cur != az_target) {
		digitalWrite(AZ_DIR, az_cur < az_target ? 0 : 1);
		digitalWrite(AZ_STEP, HIGH);
		az_cur = az_cur + (az_cur < az_target ? 1 : -1);
	}

	delayMicroseconds(1000);

	digitalWrite(ALT_STEP, LOW);
	digitalWrite(AZ_STEP, LOW);
}

void Phy::azCalCircle(){
	delay(3000);
	for (int i = 0; i < AZ_CIR; i++)
		STEP(AZ, 0);
	delay(3000);
	for (int i = 0; i < AZ_CIR; i++)
		STEP(AZ, 1);
}

void Phy::altCalCircle(){
	delay(3000);
	for (int i = 0; i < ALT_CIR; i++)
		STEP(ALT, 0);
	delay(3000);
	for (int i = 0; i < ALT_CIR; i++)
		STEP(ALT, 1);
}

void Phy::zero() {
	//Serial.println("Zeroing ALT...");
	for (int i = 0; i < 1000; i++)
		STEP(ALT, 0);
	for (int i = 0; i < 2000 && !ZERO(ALT); i++)
		STEP(ALT, 1);

	//Serial.println("Zero AZ...");
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

	if (!ZERO(ALT) || !ZERO(AZ)) {
		while (true) {
		}
	}

	//Serial.println("Zero OK...");
}
