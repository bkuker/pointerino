// Do not remove the include below
#include "Pointer.h"
#include "Clock.h"
#include "Phy.h"
#include "AstroClock.h"

Clock clock = Clock();
Phy phy = Phy();
AstroClock as = AstroClock(43.554736, -73.249809);

String go = "";         // a string to hold incoming data
//r480642D7,F41BECFA
//R34AB,12CE

//r34AB0000,12CE0000



void dms(float a) {
	int d = floor(a);
	int m = floor((a - d) * 60);
	int s = floor((((a - d) * 60) - m) * 60);
	Serial.print(d);
	Serial.print("°");
	Serial.print(m);
	Serial.print("'");
	Serial.print(s);
	Serial.print("\"");
}

void goTo(String s) {
	String ra = s.substring(0,4);
	String dec = s.substring(9,13);
//	Serial.print("RA=");
//	Serial.println(ra);
//	Serial.print("DEC=");
//	Serial.println(dec);
	long ral = strtol(ra.c_str(),NULL,16);
	long decl = strtol(dec.c_str(),NULL,16);
//	Serial.print("RAl=");
//	Serial.println(ral);
//	Serial.print("DECl=");
//	Serial.println(decl);

	float raf;
	float decf;

	raf = ((float)ral / 65535.0) * 360;
	decf = ((float)decl / 65535.0) * 360;

//	Serial.print("RAf=");
//	Serial.println(raf);
//	dms(raf);
//	Serial.print("DECf=");
//	Serial.println(decf);
//	dms(decf);

	float alt, az;
	as.convert(clock.getTime(), raf,decf, &alt, &az);
	phy.moveTo(alt,az);
//
//	Serial.print(clock.getTime());
//		Serial.print("\nALT,AZ: ");
//		dms(alt);
//		Serial.print(",");
//		dms(az);
//		Serial.println("");
}

typedef enum {
	WAITING, GET, GOTO
} state_t;
state_t state = WAITING;
void process(char c) {
	if (state == WAITING) {
		if (c == 'r') {
			state = GOTO;
			go = "";
		} else if (c == 'e') {
			if (go.length() == 17) {
				Serial.print(go);
				Serial.print("#");
			} else
				Serial.print("00000000,00000000#");
		}
		return;
	}
	if (state == GOTO) {
		go += c;
		if (go.length() == 17) {
			Serial.print("#");
			goTo(go);
			state = WAITING;
		}
	}
}

void serialEvent() {
	while (Serial.available()) {
		process((char) Serial.read());
	}
}

void setup() {
	Serial.begin(9600);
	go.reserve(20);

	Wire.begin();
	//clock.setTime(0, 41, 23, 1, 31, 5, 15);
	Serial.print("00000000,00000000#");

	phy.zero();
}

void loop() {
	phy.tick();
	return;
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
	phy.moveTo(0, 0);

	while (1) {
	}
}
