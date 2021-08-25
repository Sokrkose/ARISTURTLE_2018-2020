#include <Arduino.h>
#include <SPI.h>
#include "LTC2630.h"

#define NULL_DATA 0

//=============================================================================
void LTC2630::init(void)
{
  // Initialize the LTC2630-8 DAC. Set its reference to VCC
  // (+5VDC)
//  Serial.println(F("cmd1"));
  cmd_send(DAC_POWER_DOWN, NULL_DATA);
//  Serial.println(F("cmd2"));
  cmd_send(DAC_VCC_REF, NULL_DATA);
}
//=============================================================================
void LTC2630::backlight_cycle(void)
{
  uint8_t light_val;

  /* Decrease backlight from full brightness to almost off
      */
  for (light_val = BACKLIGHT_FULL_ON; light_val > BACKLIGHT_FULL_OFF; --light_val)
  {
    backlight_set(light_val);
    _delay_ms(40);
  }

  /* Leave backlight off for 1S
	  */
  delay(1000);

  /* Increase backlight from off to almost full brightness
      */
  for (light_val = BACKLIGHT_FULL_OFF; light_val < BACKLIGHT_FULL_ON; ++light_val)
  {
    backlight_set(light_val);
    _delay_ms(40);
  }
}
//=============================================================================
void LTC2630::backlight_set(uint8_t light)
{
  cmd_send(DAC_REG_WRITE_UPDATE, light);
}
//=============================================================================
void LTC2630::cmd_send(dac_cmd_t cmd, uint8_t data)
{
  //Initialize SPI. By default the clock is 4MHz.
  //SPI.begin();
  //Bump the clock to 8MHz. Appears to be the maximum.
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  
  // Set DAC_CS low. Send DAC command, data byte followed by don't
  //  care byte to DAC
  //CLR_DAC_CS;
  digitalWrite(53, LOW);
 
  SPI.transfer(cmd);
  delay(1);
  SPI.transfer(data);
  delay(1);
  SPI.transfer(NULL_DATA);
  delay(1);

  digitalWrite(53, HIGH);
  
  SPI.endTransaction();  
}
