/*
 * AstroClock.cpp
 *
 *  Created on: Jun 14, 2015
 *      Author: bkuker
 */

#include "AstroClock.h"
#include <math.h>
#include <stdint.h>

#define J2K 946728000

#define RAD(D) (D / 57.2957795)
#define DEG(R) (R * 57.2957795)

#define SIN(D) sin(RAD(D))
#define COS(D) cos(RAD(D))
#define ASIN(X) DEG(asin(X))
#define ACOS(X) DEG(acos(X))

AstroClock::AstroClock(float _lat, float _lon) :
		lat(_lat), lon(_lon) {
}

AstroClock::~AstroClock() {
	// TODO Auto-generated destructor stub
}

float AstroClock::convertJ2000(uint32_t timestamp) {
	uint32_t seconds = timestamp - J2K;
	return seconds / (60.0 * 60.0 * 24.0);
}

float AstroClock::getLSTd(float j2k) {
	float ut = 12 + 24 * (j2k - floor(j2k));
	float lstd = 100.46 + 0.985647 * j2k + lon + 15 * ut;
	while (lstd - 360.0 > 0)
		lstd = lstd - 360.0;
	return lstd;
}

void AstroClock::convert(uint32_t timestamp, float ra, float dec, float* alt,
		float* az) {
	float lstd = getLSTd(convertJ2000(timestamp));
	float ha = lstd - ra;
	while (ha < 0)
		ha += 360.0;

	float sinAlt = SIN(dec) * SIN(lat) + COS(dec) * COS(lat) * COS(ha);
	*alt = ASIN(sinAlt);

	float cosA = //
			(SIN(dec) - SIN(*alt) * SIN(lat)) //
			/ (COS(*alt) * COS(lat));

	float a = ACOS(cosA);

	if (SIN(ha) < 0)
		*az = a;
	else
		*az = 360.0 - a;
}
