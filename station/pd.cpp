#include "pd.h"
#include "Arduino.h"

#define pdPin	 A0
#define pdResist 470

void pdInit(){
	pinMode(sensorPin, INPUT);
}

void pdRead(){
	double sr = analogRead(pdPin)/1023.0*5.0;
	return sr*sr*psResist*3536.77651315322968377;
}
