#ifndef FLASH_H
#define FLASH_H
#include <stdint.h>
//so, si, sck are from arduino spi
#define FLASHclock  100
#define FLASHselect 4 //this is slave select

uint8_t flashReadStatus();
void flashDisableWrites();
void flashEnableWrites();
void flashSetStatus(bool wpen, bool bp1, bool bp0);
uint8_t flashRead(uint16_t addr);
void flashWrite(uint16_t addr, uint8_t data);
void initFlash();

#endif//FLASH_H
