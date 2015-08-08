#ifndef PHY_H_
#define PHY_H_
#include "Arduino.h"
#include "Wire.h"

class Phy {
private:
	int alt_cur;
	int az_cur;
	int alt_target;
	int az_target;
public:
	Phy();
	void setAltAz(float altD, float azD);
	float getAlt();
	float getAz();
	void tick();
	void zero();
};

#endif
