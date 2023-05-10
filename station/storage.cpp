#include "storage.h"
#include "rtc.h"
#include "flash.h"
#include <Arduino.h>

uint8_t readFromBuf(uint8_t* loc){
    uint8_t waddr = RTCmRead(10);
    uint8_t raddr = RTCmRead(0);
    uint8_t filld = RTCmRead(5);
    Serial.println("read:: fill:"+String(filld)+" wl:"+String(waddr)+" rl:"+String(raddr));
    if(filld == 0){
      return 0;//no data available
    }
    if(waddr == raddr){
      return 0;
    }
    filld -= 1;
    //get data
    for(uint8_t i = 0; i < 8; i++){
      loc[i]=flashRead((raddr*8)+i);
      Serial.print((raddr*8)+i);
      Serial.print(" ");
      Serial.print(flashRead((raddr*8)+i), BIN);
      Serial.print(" ");
    }
    Serial.println();
    raddr = raddr + 1 % 256;
    //update circ buff
    RTCmWrite(0, raddr);
    delay(3);
    RTCmWrite(5, filld);
    delay(3);
    RTCmWrite(10, waddr);
    return 1;
    
}
//void writeToBuf(uint8_t* data){
void writeToBuf(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, uint8_t d8){
  //flashEnableWrites();
  delay(5);
    uint8_t waddr = RTCmRead(10);
    uint8_t raddr = RTCmRead(0);
    uint8_t filld = RTCmRead(5);

    Serial.println(flashReadStatus(), BIN);
    flashWrite(waddr*8+0, d1);Serial.print(d1);delay(5);Serial.print(flashRead((waddr*8)+0));delay(5);
    flashWrite(waddr*8+1, d2);Serial.print(d2);delay(5);Serial.print(flashRead((waddr*8)+1));delay(5);
    flashWrite(waddr*8+2, d3);Serial.print(d3);delay(5);Serial.print(flashRead((waddr*8)+2));delay(5);
    flashWrite(waddr*8+3, d4);Serial.print(d4);delay(5);Serial.print(flashRead((waddr*8)+3));delay(5);
    flashWrite(waddr*8+4, d5);Serial.print(d5);delay(5);Serial.print(flashRead((waddr*8)+4));delay(5);
    flashWrite(waddr*8+5, d6);Serial.print(d6);delay(5);Serial.print(flashRead((waddr*8)+5));delay(5);
    flashWrite(waddr*8+6, d7);Serial.print(d7);delay(5);Serial.print(flashRead((waddr*8)+6));delay(5);
    flashWrite(waddr*8+7, d8);Serial.print(d8);delay(5);Serial.println(flashRead((waddr*8)+7));delay(5);
    Serial.println(flashReadStatus(), BIN);

    filld = filld + 1;
    if(filld > 255){
      filld = 255;
      raddr = (raddr + 1) % 255;
    }
    waddr = (waddr + 1) % 255;
    RTCmWrite(0, raddr);
    RTCmWrite(5, filld);
    RTCmWrite(10, waddr);
    Serial.println("write:: fill:"+String(filld)+" wl:"+String(waddr)+" rl:"+String(raddr));
}

void sendToEeprom(float temperature, float wind, float sun)
{

  uint8_t writeBuffer[8];// = {0, 0, 0, 0, 0, 0, 0, 0};


  //compress clock
  uint8_t hour   = RTCHourRead();
  uint8_t date   = RTCDateRead();
  uint8_t month  = RTCMonthRead();
  uint8_t year   = RTCYearRead();
  writeBuffer[0] = hour;              //MMmmmmHH
  writeBuffer[1] = date;              //hhhhDDdd
  writeBuffer[2] = month; //ddMmmmmY
  writeBuffer[3] = year;
  
  //pull values to wanted ranges
  uint8_t temps = (temperature < 0) ? 0b11111111 : 0;
  uint8_t tempi =(abs(temperature)/0.13725490196078431373); //8   40
  uint8_t windi =abs(wind)/0.11764705882352941176;  //7   64
  uint8_t suni =abs(sun)/4.31372549019607843137;   //10  1100
  


  //append compressed numbers to write buffer
  writeBuffer[4] = temps;
  writeBuffer[5] = tempi;
  writeBuffer[6] = windi;
  writeBuffer[7] = suni;
  Serial.println(String(suni) +" "+ String(temps) +" "+ String(tempi) +" "+ String(windi));
Serial.println(String(hour) +" "+ String(date) +" "+ String(month) +" "+ String(year));
Serial.println(String(writeBuffer[0]) +" "+ String(writeBuffer[1]) +" "+ String(writeBuffer[2]) +" "+ String(writeBuffer[3]) +" "+String(writeBuffer[4]) +" "+ String(writeBuffer[5]) +" "+ String(writeBuffer[6]) +" "+ String(writeBuffer[7]));
  
  writeToBuf(hour, date, month, year, temps, tempi, windi, suni);
}

int getFromEeprom(float* results, uint8_t* tim){
  int retVal = 0;

  uint8_t measurments[8];

  if(readFromBuf(measurments)){
    retVal = 1;
    uint8_t hour = measurments[0];
    uint8_t day = measurments[1];
    uint8_t month = measurments[2];
    uint8_t year = measurments[3];

  Serial.print("hou:");
  Serial.println(hour);
  Serial.print("dat:");
  Serial.println(day);
  Serial.print("mon:");
  Serial.println(month);
  Serial.print("yea:");
  Serial.println(year);

    uint8_t rtmp = measurments[5];
    uint8_t rwnd = measurments[6];
    uint16_t rsol = measurments[7];
    
    float tmp=0.13725490196078431373*rtmp;
    tmp *= (measurments[4] != 0) ? -1: 1;
    float wnd= 0.11764705882352941176*rwnd;
    float sol=4.31372549019607843137*rsol;
    
  Serial.print("tmp:");
  Serial.println(tmp);
  Serial.print("wnd:");
  Serial.println(wnd);
  Serial.print("sol:");
  Serial.println(sol);
    
    //store numbers to results
    results[0] = tmp;
    results[1] = wnd;
    results[2] = sol;
  }
  //return an 1 if we had no historical data
  return retVal;
}
