#include "rtc.h"

#include <stdint.h>
#include <Arduino.h>


void advanceClock(){
  digitalWrite(RTCclock, !digitalRead(RTCclock)); //advance clock
  delayMicroseconds(1);
  digitalWrite(RTCclock, !digitalRead(RTCclock)); //advance clock
  delayMicroseconds(1);
}

void ioWrite(uint8_t operation){
  //Serial.print("\tW ");
  for(uint8_t i = 0; i < 8; i++){
    digitalWrite(RTCio, (operation >> i) & 1); //set send bit
    advanceClock();
    //Serial.print((operation >> i) & 1);
  }
  //Serial.println();
}

uint8_t ioRead(){
  //Serial.print("\tR ");
  uint8_t reg = 0;
  for(uint8_t i = 0; i < 8; i++){
    //reg = reg << 1;//shift reg value
    reg = reg | (digitalRead(RTCio) << i);//add bit to reg
    advanceClock();
    //Serial.print(digitalRead(RTCio));
  }
  //Serial.println();
  return reg;
}

uint8_t RTCread(uint8_t operation){
  //Serial.println("READ");
  pinMode(RTCio, OUTPUT);
  digitalWrite(RTCenable, HIGH);
  delay(1);
  //output operation to io
  ioWrite(operation | 0b10000001);    //set command and read bit
  pinMode(RTCio, INPUT);
  //read from io
  uint8_t reg = ioRead();
  digitalWrite(RTCenable, LOW);
  pinMode(RTCio, OUTPUT);
  return reg;
}

void RTCwrite(uint8_t operation, uint8_t data){
  //Serial.println("WRITE");
  pinMode(RTCio, OUTPUT);
  digitalWrite(RTCenable, HIGH);
  delay(5);
  //output operation to io
  ioWrite(operation | 0b10000000);  //set command and read bits
  //write to register
  ioWrite(data);
  digitalWrite(RTCenable, LOW);
  pinMode(RTCio, OUTPUT);
}

void RTCunhalt(){
  if(RTCread(RTCseconds & 0b10000000)){
    //Serial.println("resetting clock");
    //Serial.println(RTCread(RTCwp));
    RTCwrite(RTCwp, 0b00000000);
    //Serial.println(RTCread(RTCwp));
      
    RTCwrite(RTCyear,     0b00100011);
    RTCwrite(RTCday,      0b00000010);
    RTCwrite(RTCmonth,    0b00000011);
    RTCwrite(RTCdate,     0b00101000);
    RTCwrite(RTChours,    0b00000000);
    RTCwrite(RTCminutes,  0b00000000);
    RTCwrite(RTCseconds,  0b00000000);
    RTCwrite(RTCwp,       0b10000000);
    //Serial.println(RTCread(RTCwp));
  }
}

uint8_t bed2b(uint8_t number){
  return ((number & 0b11110000)>>4)*10 + (number&0b00001111);
}
uint8_t b2bed(uint8_t number){
  return ((number-(number%10)) << 4) | (number%10);
}

uint8_t RTCSecondRead(){
  uint8_t tmp = RTCread(RTCseconds) & 0b01111111;
  return bed2b(tmp);
}
uint8_t RTCMinuteRead(){
  uint8_t tmp = RTCread(RTCminutes) & 0b01111111;
  return bed2b(tmp);
}
uint8_t RTCHourRead(){
  uint8_t tmp = RTCread(RTChours) & 0b00111111;
  return bed2b(tmp);
}
uint8_t RTCDateRead(){
  uint8_t tmp = RTCread(RTCdate) & 0b00111111;
  return bed2b(tmp);
}
uint8_t RTCMonthRead(){
  uint8_t tmp = RTCread(RTCmonth) & 0b01111111;
  return bed2b(tmp);
}
uint8_t RTCDayRead(){
  uint8_t tmp = RTCread(RTCday) & 0b00000111;
  return bed2b(tmp);
}
uint8_t RTCYearRead(){
  uint8_t tmp = RTCread(RTCyear & 0b01111111);
  return bed2b(tmp);
}

void RTCinit(){
  pinMode(RTCenable,      OUTPUT);
  pinMode(RTCio,          INPUT);
  pinMode(RTCclock,       OUTPUT);
  digitalWrite(RTCio,     LOW);
  digitalWrite(RTCenable, LOW);
  digitalWrite(RTCclock,  LOW);
}
