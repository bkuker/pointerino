// Do not remove the include below
#include "Pointer.h"
#include "Clock.h"
#include "Phy.h"
#include "AstroClock.h"

Clock clock = Clock();
Phy phy = Phy();
AstroClock as = AstroClock(43.554736, -73.249809);

void setup() {
	Serial.begin(9600);
	Wire.begin();
	//clock.setTime(0, 41, 23, 1, 31, 5, 15);
	Serial.println("HELLO");
	clock.displayTime();

}

void dms(float a){
	int d = floor(a);
	int m = floor((a-d)*60);
	int s = floor( (((a-d)*60)-m)*60);
	Serial.print(d);
	Serial.print("°");
	Serial.print(m);
	Serial.print("'");
	Serial.print(s);
	Serial.print("\"");
}

void loop() {

	float alt, az;
	as.convert(clock.getTime(), 101.287, -16.716, &alt, &az);

	Serial.print(clock.getTime());
	Serial.print(" ");
	dms(alt);
	Serial.print(",");
	dms(az);
	Serial.println("");

	delay(500);
	return;

	phy.zero();

	for (int alt = 0; alt <= 180; alt += 45) {
		phy.moveTo(alt, 0);
		delay(300);
	}
	for (int alt = 180; alt >= 0; alt -= 10) {
		phy.moveTo(alt, 0);
		delay(100);
	}

	phy.moveTo(0, 45);
	delay(1000);
	phy.moveTo(0, -45);
	delay(1000);
	phy.moveTo(0,0);

	while (1) {
	}
}
