#include "storage.h"
#include "rtc.h"
#include "flash.h"

void sendToEeprom(float temperature, float wind, float sun)
{
  //4+ 1
  //konversio
  short tempi =(short)temperature;//6
  short tempf =(short)(fmod(temperature, 1)*100);//4

  short windi =(short)wind; //6
  short windf =(short)(fmod(wind, 1)*100);//4

  short suni =(short)sun;//7
  short sunf =(short)(fmod(sun, 1)*100);//4
  //pakkaus
  
  //tallennus
  flashWrite(uint16_t addr, uint8_t data);
}
