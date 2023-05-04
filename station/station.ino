#include <SoftwareSerial.h>
#include "ldr.h"
#define Hall A2
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
  initFlash();
  RTCinit();
  RTCunhalt();
  flashEnableWrites();
  RTCwrite(RTCwp,       0b10000000);
}

void Measure_WindSpeed(){
  float data,distance = 0.00;
  int rotation = 0;
  int lock = 1;
  float radius = 0.075; // !!! Remember to edit radius !!!
  float circumference = 2 * PI * radius;
  timer = millis();
  ctimer = timer;
  while((ctimer-timer) <= 10000){
  data = analogRead(Hall);
  //Serial.println(data);
  if(data <=530 ){
    lock=0;
  }
  if(data >= 540 && lock <= 0){
    rotation++;
    lock = 1;
    Serial.print(rotation);
  }
  //Serial.println(data);
  delay(5);
  ctimer = millis();
  }
  distance = rotation * circumference;
  wind = distance / 10;
  rotation = 0;
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

  Measure_WindSpeed();
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

void historyData()
{
  float results[3];
  uint8_t tim[4];
  
  while (getFromEeprom(results, tim))
  {
    char rand1[10]; 
    char rand2[10]; 
    char rand3[10]; 
    char rand4[10]; 
 
    char message[20];
   // for(int i = 0; i < 10; i++){
  
      dtostrf(results[0], 4, 1, rand1);
      dtostrf(results[1], 4, 1, rand2);
      dtostrf(results[2], 4, 1, rand3);


      uint64_t pvm = 0;
      pvm = (pvm + bed2b(tim[0]))*100;
      pvm = (pvm + bed2b(tim[1]))*100;
      pvm = (pvm + bed2b(tim[2]))*1000;
      pvm = (pvm + bed2b(tim[3]));
      
      dtostrf(pvm, 4, 1, rand4);
      
      sprintf(message, "Date: %d, ", rand4);
      BTSerial.write(message);
      Serial.println(message);
      
      sprintf(message, "Temperature: %s C ", rand1);
      BTSerial.write(message);
      Serial.println(message);
      
      sprintf(message, "Wind %s m/s, ", rand2);
      BTSerial.write(message);
      Serial.println(message);
      
      sprintf(message, "Sun: %s W/m^2\n\n", rand3);
      BTSerial.write(message);
      Serial.println(message);
      
  }
/*
char rand1[10]; 
char rand2[10]; 
char rand3[10]; 
char rand4[10]; 


  char message[20];
  for(int i = 0; i < 10; i++){

    dtostrf(rand(), 4, 1, rand1);
    dtostrf(rand(), 4, 1, rand2);
    dtostrf(rand(), 4, 1, rand3);
    dtostrf(rand(), 4, 1, rand4);
    
    sprintf(message, "Date: %d, ", rand4);
    BTSerial.write(message);
    Serial.println(message);
    
    sprintf(message, "Sun: %s W/m^2 ", rand1);
    BTSerial.write(message);
    Serial.println(message);
    
    sprintf(message, "Temp: %s C, ", rand2);
    BTSerial.write(message);
    Serial.println(message);
    
    sprintf(message, "Wind: %s m/s\n\n", rand3);
    BTSerial.write(message);
    Serial.println(message);
    }*/
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
    if (data == "3")
    {
      historyData();
    }
}
