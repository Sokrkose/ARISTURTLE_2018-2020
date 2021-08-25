#include <Arduino.h>
#include <SPI.h>
#include "ADCCONVERT.h"

uint16_t ADCCONVERT::adcConvert(byte adChannel, int CS){   // 8 bit version

  byte adDin;
  if(adChannel == 1)           //select adc channel
     adDin = 0x00;             //convert command channel 1
  else if(adChannel == 2)
     adDin = 0x08;             //convert command channel 2  

  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE3));
  
  digitalWrite(CS, LOW);                     //adc enable

  int adDataHigh = SPI.transfer(adDin);      //high byte data transfer 
  int adDataLow = SPI.transfer(0x00);        //low byte data transfer
  
  digitalWrite(CS, HIGH);                    //adc disable
  
  SPI.endTransaction();
  
  uint16_t adData = (adDataHigh << 8) + adDataLow;

  return adData;      

}
