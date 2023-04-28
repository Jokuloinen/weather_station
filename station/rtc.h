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


#endif//RTC_H
