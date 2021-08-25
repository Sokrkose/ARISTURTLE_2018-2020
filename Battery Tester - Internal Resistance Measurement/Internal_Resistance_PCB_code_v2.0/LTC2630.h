#ifndef __LTC2630_h__
#define __LTC2630_h__

#include <Arduino.h>

// Backlight full on at DAC output = 1.5V
// Backlight off at DAC output < 50mV
#define BACKLIGHT_FULL_ON 255
#define BACKLIGHT_FULL_OFF 0

// LTC2630-8 DAC Commands
typedef enum {
  DAC_REG_WRITE = 0b00000000,
  DAC_REG_UPDATE = 0b00010000,
  DAC_REG_WRITE_UPDATE = 0b00110000,
  DAC_POWER_DOWN = 0b01000000,
  DAC_INTERNAL_REF = 0b01100000,
  DAC_VCC_REF = 0b01110000,
} dac_cmd_t;

class LTC2630
{
public:
  void init(void);
  void backlight_cycle(void);
  void backlight_set(uint8_t light);
  void cmd_send(dac_cmd_t cmd, uint8_t data);
};

#endif //__LTC2630_h__
