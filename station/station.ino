#include <SoftwareSerial.h>
#include "ldr.h"
#define Hall A1
#include "rtc.h"
#include "flash.h"
#include "storage.h"


SoftwareSerial BTSerial(4, 3); // RX, TX pins of HC-05

float wind = 0.0; //Sensor values to send
unsigned long timer, ctimer = 0;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  delay(100);
  initLDR();
  RTCinit();
  //RTCunhalt();
  flashEnableWrites();
  RTCwrite(RTCwp,       0b10000000);
}

void Measure_WindSpeed(){
  float data,speed,distance = 0.00;
  int rotation, lock, print = 0;
  int radius = 0.075; // !!! Remember to edit radius !!!
  float circumference = 2 * PI * radius;
  timer = millis();
  ctimer = timer;
  while((ctimer-timer) <= 10000){
  data = analogRead(Hall);
  if(data <=560 ){
    lock=0;
  }
  if(data >= 570 && lock <= 0){
    rotation++;
    lock = 1;
    //Serial.print(rotation);
  }
  //Serial.println(data);
  delay(5);
  ctimer = millis();
  }
  distance = rotation * circumference;
  speed = distance / 10;
}

void sendSomething()
{
  int a = analogRead(A0);
  float voltage = a * (5.0 / 1023.0); // Convert analog value to voltage
  float R2 = 10000.0 * (5.0 / voltage - 1.0); // Calculate resistance of thermistor
  
  float logR2R25 = log(R2/10000.0);
  float temperature = 1.0 / (1.0/298.15 + logR2R25 / 3950.0);
  temperature -= 273.15; // Convert temperature to Celsius
  char myNumber[10];
  dtostrf(temperature, 4, 1, myNumber);

 // Measure_WindSpeed();
  char windd[10];
  dtostrf(wind, 4, 1, windd);
  
  char sun[15];
  dtostrf((float)readLDR(), 9, 1, sun);

  sendToEeprom(temperature, wind, readLDR());
      
  char message[35];
  sprintf(message, "%s\n", myNumber);
  BTSerial.write(message);
  sprintf(message, "%s\n", windd);
  BTSerial.write(message);
  sprintf(message, "%s\n", sun);
  BTSerial.write(message);

  
  //char message[20] = "134.7\n23.5\n35.5\n";
  //BTSerial.write(message);
  Serial.println("Sending value: ");
  Serial.println(message);
  Serial.println(temperature);
  Serial.println(wind);
  Serial.println(readLDR());
}



void updateClock()
{
    String data = BTSerial.readStringUntil('\n');
  
    RTCwrite(RTCwp,       0b00000000);
    RTCwrite(RTCyear,     ((data[2]-48) << 4) || (data[3]-48));
    RTCwrite(RTCday,      data[14]-48);
    RTCwrite(RTCmonth,    ((data[6]-48) << 4) || (data[7]-48));
    RTCwrite(RTCdate,     ((data[4]-48) << 4) || (data[5]-48));
    RTCwrite(RTChours,    ((data[8]-48) << 4) || (data[9]-48));
    RTCwrite(RTCminutes,  ((data[10]-48) << 4) || (data[11]-48));
    RTCwrite(RTCseconds,  ((data[12]-48) << 4) || (data[13]-48));
    RTCwrite(RTCwp,       0b10000000);

    
}

void loop() {

  String data;

  if (BTSerial.available()) {
    //data = BTSerial.parseInt();
    data = BTSerial.readStringUntil('\n');
    Serial.print("Received value: ");
    Serial.println(data);   
  }

    if (data == "1")
    {
      sendSomething();
    }
    if (data == "2")
    {
      updateClock();
    }
}
