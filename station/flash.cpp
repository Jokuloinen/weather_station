#include "flash.h"
#include <SPI.h>

uint8_t FLASHwren   =0b00000110;  //enable write
uint8_t FLASHwrdi   =0b00000100;  //disabel writes
uint8_t FLASHrdsr   =0b00000101;  //read status register
uint8_t FLASHwrsr   =0b00000001;  //write status register
uint8_t FLASHread   =0b00000011;  //read from flash
uint8_t FLASHwrite  =0b00000010;  //write to flash

uint8_t flashReadStatus(){
  SPI.beginTransaction(SPISettings(FLASHclock, MSBFIRST, SPI_MODE0));
  digitalWrite(FLASHselect, LOW);
  uint8_t reg = SPI.transfer(FLASHrdsr);
  reg = SPI.transfer(0);
  SPI.endTransaction();
  digitalWrite(FLASHselect, HIGH);
  return reg;
}

void flashDisableWrites(){
  SPI.beginTransaction(SPISettings(FLASHclock, MSBFIRST, SPI_MODE0));
  digitalWrite(FLASHselect, LOW);
  uint8_t reg = FLASHwrdi;
  SPI.transfer(&reg, 1);
  SPI.endTransaction();
  digitalWrite(FLASHselect, HIGH);
}

void flashEnableWrites(){
  SPI.beginTransaction(SPISettings(FLASHclock, MSBFIRST, SPI_MODE0));
  digitalWrite(FLASHselect, LOW);
  //uint8_t reg = 
  SPI.transfer(FLASHwren);
  SPI.endTransaction();
  digitalWrite(FLASHselect, HIGH);
}

void flashSetStatus(bool wpen, bool bp1, bool bp0){
  uint8_t reg = 0;
  if(wpen)//enable write protect
    reg |= 0b10000000;
  if(bp1)//enable memory protect
    reg |= 0b00001000;
  if(bp0)//enable memory protect
    reg |= 0b00000100;
  SPI.beginTransaction(SPISettings(FLASHclock, MSBFIRST, SPI_MODE0));
  digitalWrite(FLASHselect, LOW);
  SPI.transfer(FLASHwrsr<<8 | reg);
  SPI.endTransaction();
  digitalWrite(FLASHselect, HIGH);
}

uint8_t flashRead(uint16_t addr){
  flashEnableWrites();
  SPI.beginTransaction(SPISettings(FLASHclock, MSBFIRST, SPI_MODE0));
  digitalWrite(FLASHselect, LOW);
  SPI.transfer(FLASHread); 
  SPI.transfer16(addr);
  uint8_t data = SPI.transfer(0);
  SPI.endTransaction();
  digitalWrite(FLASHselect, HIGH);
  flashEnableWrites();
  return data;
}

void flashWrite(uint16_t addr, uint8_t data){
  flashEnableWrites();
  SPI.beginTransaction(SPISettings(FLASHclock, MSBFIRST, SPI_MODE0));
  digitalWrite(FLASHselect, LOW);
  SPI.transfer(FLASHwrite);
  SPI.transfer16(addr);
  SPI.transfer(data);
  SPI.endTransaction();
  digitalWrite(FLASHselect, HIGH);
  flashEnableWrites();
}

void initFlash(){
  SPI.begin();
  pinMode(FLASHselect, OUTPUT);
  digitalWrite(FLASHselect, HIGH);
  delay(25);
  //set statusregister
  uint8_t reg = flashReadStatus();
//   Serial.println(reg, BIN);
  //if a write is in progress
  while(reg & 0b00000001){
    delay(1);
    reg = flashReadStatus();
  }
//  Serial.println("init");
  if(reg != 0b00000000){
    delay(25);
    flashEnableWrites();
    delay(40);
    flashSetStatus(false, false, false);
  }
}
