// Do not remove the include below
#include "Pointer.h"
#include "Clock.h"
#include "Phy.h"
#include "AstroClock.h"

Clock clock = Clock();
Phy phy = Phy();
AstroClock as = AstroClock(43.554736, -73.249809);

float target_ra = 0;
float target_dec = 0;
bool move = false;

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

String raDecToNexstar(float ra, float dec){

	long ral = ((ra / 360.0f) * 65535.0f);
	long decl = ((dec / 360.0f) * 65535.0f);

	String ras = String(ral, HEX);
	String decs =  String(decl, HEX);

	while ( ras.length() > 4 )
		ras.remove(0,1);
	while ( ras.length() < 4 )
		ras = "0" + ras;

	while(decs.length() > 4)
		decs.remove(0,1);
	while(decs.length() < 4)
		decs = "0" + decs;

	ras.toUpperCase();
	decs.toUpperCase();
	return ras + "0000," + decs + "0000";
}

void nexstarToRaDec(String s, float* raf, float* decf) {
	String ra = s.substring(0, 4);
	String dec = s.substring(9, 13);
//	Serial.print("RA=");
//	Serial.println(ra);
//	Serial.print("DEC=");
//	Serial.println(dec);
	long ral = strtol(ra.c_str(), NULL, 16);
	long decl = strtol(dec.c_str(), NULL, 16);
//	Serial.print("RAl=");
//	Serial.println(ral);
//	Serial.print("DECl=");
//	Serial.println(decl);


	*raf = ((float) ral / 65535.0) * 360;
	*decf = ((float) decl / 65535.0) * 360;

//	Serial.print("RAf=");
//	Serial.println(raf);
//	dms(raf);
//	Serial.print("DECf=");
//	Serial.println(decf);
//	dms(decf);

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
//			if (go.length() == 17) {
//				Serial.print(go);
//				Serial.print("#");
//			} else {
				Serial.print(raDecToNexstar(target_ra, target_dec));
				Serial.print("#");
//			}
		}
		return;
	}
	if (state == GOTO) {
		go += c;
		if (go.length() == 17) {
			Serial.print("#");

			float ra, dec;
			nexstarToRaDec(go, &ra, &dec);

			target_ra = ra;
			target_dec = dec;
			move = true;

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
	go = "";

	Wire.begin();
	//clock.setTime(0, 41, 23, 1, 31, 5, 15);
	Serial.print("00000000,00000000#");

	//phy.zero();
}

uint32_t lastTime = 0;
void loop() {

#if 0
	float alt, az, ra, dec;
	ra=101.287;
	dec=-16.716;

	nexstarToRaDec("480642D7,F41BECFA", &ra, &dec);
	nexstarToRaDec("F4AB0000,F2CE0000", &ra, &dec);
	nexstarToRaDec("AFE50960,ED343393", &ra, &dec);

	Serial.print("RaDec: ");
	dms(ra);
	Serial.print(",");
	dms(dec);
	Serial.println("");

	as.convert(clock.getTime(), ra, dec, &alt, &az);
	Serial.print("AltAz: ");
	dms(alt);
	Serial.print(",");
	dms(az);
	Serial.println("");

	as.unconvert(clock.getTime(), alt,az,&ra,&dec);
	Serial.print("RaDec: ");
	dms(ra);
	Serial.print(",");
	dms(dec);
	Serial.println("");

	Serial.println(raDecToNexstar(ra,dec));

	while(1){};
#endif

#if 1
	float alt, az;
	if (move) {
		uint32_t time = clock.getTime();
		if (time != lastTime) {
			as.convert(clock.getTime(), target_ra, target_dec, &alt, &az);
			phy.setAltAz(alt, az);
		}
		lastTime = time;
		phy.tick();
	}
	return;
#endif

//	float alt, az;
//	as.convert(clock.getTime(), 101.287, -16.716, &alt, &az);
//
//	Serial.print(clock.getTime());
//	Serial.print(" ");
//	dms(alt);
//	Serial.print(",");
//	dms(az);
//	Serial.println("");
//
//	delay(500);
//	return;
//
//	phy.zero();
//
//	for (int alt = 0; alt <= 180; alt += 45) {
//		phy.moveTo(alt, 0);
//		delay(300);
//	}
//	for (int alt = 180; alt >= 0; alt -= 10) {
//		phy.moveTo(alt, 0);
//		delay(100);
//	}
//
//	phy.moveTo(0, 45);
//	delay(1000);
//	phy.moveTo(0, -45);
//	delay(1000);
//	phy.moveTo(0, 0);
//
//	while (1) {
//	}
}
