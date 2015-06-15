/*
 * AstroClock.h
 *
 *  Created on: Jun 14, 2015
 *      Author: bkuker
 */

#ifndef ASTROCLOCK_H_
#define ASTROCLOCK_H_

#include "Clock.h"

class AstroClock {
private:
	float lat;
	float lon;
	float convertJ2000(uint32_t timestamp);
	float getLSTd(float j2k);
public:
	AstroClock(float _lat, float _lon);
	virtual ~AstroClock();

	void convert(uint32_t timestamp, float ra, float dec, float* alt,
			float* az);
};

#endif /* ASTROCLOCK_H_ */
