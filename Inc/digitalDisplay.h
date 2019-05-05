#ifndef __DIGITALDISPLAY_
#define __DIGITALDISPLAY_

#include "stm32f1xx_hal.h"
#include "main.h"

void disPlayData(uint8_t num, uint16_t data);
void disPlayString(uint8_t num, char* string);
#endif // !__DIGITALDISPLAY_
