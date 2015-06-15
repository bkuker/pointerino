#ifndef PHY_H_
#define PHY_H_
#include "Arduino.h"
#include "Wire.h"

class Phy {
private:
	int alt_cur;
	int az_cur;
	float alt_deg;
	float az_deg;
public:
	Phy();
	void moveTo(float altD, float azD);
	void stepTo(int alt, int az);
	void zero();
};

#endif
