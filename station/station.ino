#include "rtc.h"
#include "ldr.h"
#include "pd.h"


void setup(void) {
	Serial.begin(9600);
	rtcInit();
	ldrInit();
	pdInit();
	delay(10);
}


void loop(void) {
	Serial.println(readLDR());
	Serial.println(RTCSecondRead());
	Serial.println(pdRead());
	Serial.println();
	delay(2000);
}
