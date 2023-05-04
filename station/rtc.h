#ifndef RTC_H
#define RTC_H



#include <stdint.h>

#define READ  0b1000001
#define WRIRE 0b1000000

//addresses in rtc memory space
#define RTCseconds  0x80
#define RTCminutes  0x82
#define RTChours    0x84
#define RTCdate     0x86
#define RTCmonth    0x88
#define RTCday      0x8A
#define RTCyear     0x8C
#define RTCwp       0x8E
#define RTCctrl     0x90
#define RTCmem      0xC0

//RTC sram is used by storagelike this
//address 0 is high byte of read location for eeprom
//address 1 is coresponding lowbyte
//address 2 if high byte of write location
//address 3 lowbyte of write location
//address 4 highbyte of how many entries buffer holds
//address 5 lowbyte of the above


//pins for RTC
#define RTCenable 5
#define RTCio     7
#define RTCclock  8


void RTCinit();
uint8_t RTCread(uint8_t operation);
uint8_t RTCSecondRead();
uint8_t RTCMinuteRead();
uint8_t RTCHourRead();
uint8_t RTCDateRead();
uint8_t RTCMonthRead();
uint8_t RTCDayRead();
uint8_t RTCYearRead();
uint8_t RTCread(uint8_t operation);
void RTCwrite(uint8_t operation, uint8_t data);
void RTCunhalt();
void RTCmWrite(uint8_t addr, uint8_t data);
uint8_t RTCmRead(uint8_t addr);
uint8_t bed2b(uint8_t number);

#endif//RTC_H
