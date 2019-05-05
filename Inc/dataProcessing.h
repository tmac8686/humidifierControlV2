#pragma once
#include "stm32f1xx_hal.h"
#include "main.h"

#define PROPORTIONMODE	0
#define SWITCHMODE		1
#define COMMUNICATION	2

extern uint8_t switchWashBucket;
extern uint8_t switchSetFlag;			
extern uint8_t humiMode;
extern uint16_t humiCurrent;			//当前电流
extern uint16_t humiOpening;			//加湿开度
extern uint16_t humiOpeningFromPLC;
extern uint16_t powerProportion;		//能量比例
extern uint16_t humiCurrentUpperLimit;
extern uint16_t extraDrainWaterTime;
extern uint16_t autoDrainWaterTime;
extern uint16_t cleanDrainWaterTime;
extern uint8_t humiVersion;

extern volatile uint16_t ADC_ConvertedValue[];
extern uint32_t ADC_Average[];


void dialSwitchInit();
void dataProcessing(void);