#ifndef CLOCK_H_
#define CLOCK_H_
#include "Arduino.h"
#include "Wire.h"

class Clock {
private:
	byte bcdToDec(byte val);
	byte decToBcd(byte val);
public:
	Clock();
	void setTime(byte second, byte minute, byte hour, byte dayOfWeek,
			byte dayOfMonth, byte month, byte year);
	void readTime(byte *second, byte *minute, byte *hour, byte *dayOfWeek,
			byte *dayOfMonth, byte *month, byte *year);
	uint32_t getTime();
	void displayTime();
};

#endif
