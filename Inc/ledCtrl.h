#ifndef __LEDCTRL_
#define __LEDCTRL_

#include "stm32f1xx_hal.h"
#include "main.h"

extern uint8_t blinkFlag;
void ledBlink(uint8_t color);
void ledSwitch(uint8_t color, uint8_t statu);
void enableGreenLedBreathe(uint8_t enable);
void greenLedBreatheScan();
#endif // !__LEDCTRL_
