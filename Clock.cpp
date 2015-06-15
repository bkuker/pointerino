/*
 * Clock.cpp
 *
 *  Created on: May 25, 2015
 *      Author: http://tronixstuff.com/2014/12/01/tutorial-using-ds1307-and-Clock-real-time-clock-modules-with-arduino/
 */
#include "Clock.h"

#define Clock_I2C_ADDRESS 0x68

const uint8_t days_in_month[12] PROGMEM = { 31, 28, 31, 30, 31, 30, 31, 31, 30,
		31, 30, 31 };

#define SECONDS_FROM_1970_TO_2000 946684800

uint32_t Clock::getTime() {
	byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
	uint8_t i;
	uint16_t d;
	int16_t y;
	uint32_t rv;

	this->readTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
			&year);

	y = year;

	d = dayOfMonth - 1;
	for (i = 1; i < month; i++) {
		d += pgm_read_byte(days_in_month + i - 1);
	}
	if (month > 2 && y % 4 == 0) {
		d++;
	}
	// count leap days
	d += (365 * y + (y + 3) / 4);
	rv = ((d * 24UL + hour) * 60 + minute)
			* 60+ second + SECONDS_FROM_1970_TO_2000;
	return rv;
}
byte Clock::bcdToDec(byte val) {
	return ((val / 16 * 10) + (val % 16));
}

byte Clock::decToBcd(byte val) {
	return ((val / 10 * 16) + (val % 10));
}

void Clock::setTime(byte second, byte minute, byte hour, byte dayOfWeek,
		byte dayOfMonth, byte month, byte year) {
	// sets time and date data to Clock
	Wire.beginTransmission(Clock_I2C_ADDRESS);
	Wire.write(0); // set next input to start at the seconds register
	Wire.write(decToBcd(second)); // set seconds
	Wire.write(decToBcd(minute)); // set minutes
	Wire.write(decToBcd(hour)); // set hours
	Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
	Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
	Wire.write(decToBcd(month)); // set month
	Wire.write(decToBcd(year)); // set year (0 to 99)
	Wire.endTransmission();
}

Clock::Clock() {

}

void Clock::readTime(byte *second, byte *minute, byte *hour, byte *dayOfWeek,
		byte *dayOfMonth, byte *month, byte *year) {
	Wire.beginTransmission(Clock_I2C_ADDRESS);
	Wire.write(0); // set Clock register pointer to 00h
	Wire.endTransmission();
	Wire.requestFrom(Clock_I2C_ADDRESS, 7);
	// request seven bytes of data from Clock starting from register 00h
	*second = bcdToDec(Wire.read() & 0x7f);
	*minute = bcdToDec(Wire.read());
	*hour = bcdToDec(Wire.read() & 0x3f);
	*dayOfWeek = bcdToDec(Wire.read());
	*dayOfMonth = bcdToDec(Wire.read());
	*month = bcdToDec(Wire.read());
	*year = bcdToDec(Wire.read());
}

void Clock::displayTime() {
	byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
	// retrieve data from Clock
	this->readTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
			&year);
	// send it to the serial monitor
	Serial.print(hour, DEC);
	// convert the byte variable to a decimal number when displayed
	Serial.print(":");
	if (minute < 10) {
		Serial.print("0");
	}
	Serial.print(minute, DEC);
	Serial.print(":");
	if (second < 10) {
		Serial.print("0");
	}
	Serial.print(second, DEC);

	Serial.print(" ");
	switch (dayOfWeek) {
	case 1:
		Serial.print("Sunday");
		break;
	case 2:
		Serial.print("Monday");
		break;
	case 3:
		Serial.print("Tuesday");
		break;
	case 4:
		Serial.print("Wednesday");
		break;
	case 5:
		Serial.print("Thursday");
		break;
	case 6:
		Serial.print("Friday");
		break;
	case 7:
		Serial.print("Saturday");
		break;
	}

	Serial.print(" ");
	Serial.print(dayOfMonth, DEC);
	Serial.print("/");
	Serial.print(month, DEC);
	Serial.print("/");
	Serial.println(year, DEC);

}
