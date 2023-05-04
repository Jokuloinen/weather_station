#include "ldr.h"
#include "Arduino.h"

#define LDR1  5000.0//7500.0
#define R1    470.0//5000
#define L1Pin A2
#define L1x   12518931
#define L1y   -1.405

#define LDR2  10000.0//15000.0
#define R2    470.0//10000.0
#define L2Pin A3
#define L2x   12518931
#define L2y   -1.405

#define LDR3  200000.0//35000.0
#define R3    470.0//20000.0
#define L3Pin A4
#define L3x   12518931
#define L3y   -1.405

#define LDRA  6000    //(5×2+2.5×5+1.25×6)×(1/5)×1000

void initLDR(){
  pinMode(L1Pin, INPUT);
  pinMode(L2Pin, INPUT);
  pinMode(L3Pin, INPUT);
}

double readLDR(){
  //ldr data
  double LDRdata[3][6]={
    {L1Pin, LDR1, LDRA, R1, L1x, L1y},
    {L2Pin, LDR2, LDRA, R2, L2x, L2y},
    {L3Pin, LDR3, LDRA, R3, L3x, L3y}
  };
  
  //calculation results
  double lux[3];

  //calculate lux for LDRs
  for(char i = 0; i<3; i++){
    double ar   = analogRead(LDRdata[i][0]);
//    Serial.println(ar);
    double Rv   = ar/1023*5;
 //   Serial.println(Rv);
    double ldrV = 5-Rv;
//    Serial.println(ldrV);
    double ldrR = ldrV/Rv*LDRdata[i][1];
//    Serial.println(ldrR);
    double luks = LDRdata[i][4] * pow(ldrR, LDRdata[i][5]);
//    Serial.println(luks);    
    lux[i] = luks;
    //wattm[i] = 0.9290304*lux*LDRdata[i][2]/93;
//    Serial.println();
  }

  //get a average or smth
//  double tVAL = (lux[0]+lux[1]+lux[2])/3;               //average
  double tVAL = pow(lux[0]*lux[1]*lux[2], 1.0/3.0);     //geometric mean
//  Serial.println(tVAL, 10);

  //test print
//  Serial.print(lux[0]);
//  Serial.print(", ");
//  Serial.print(lux[1]);
//  Serial.print(", ");
//  Serial.print(lux[2]);
//  Serial.println();

  return 0.9290304*tVAL*LDRA/93;
}
