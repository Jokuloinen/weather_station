#ifndef STORAGE_H
#define STORAGE_H
#include <stdint.h>
void sendToEeprom(float temperature, float wind, float sun);
int getFromEeprom(float* results, uint8_t* tim);

#endif
