#pragma once
#include "stm32f1xx_hal.h"
#include "main.h"

#define PROPORTIONMODE	0
#define SWITCHMODE		1
#define COMMUNICATION	2

extern uint8_t switchSetFlag;			
extern uint8_t humiMode;
extern uint16_t humiCurrent;			//��ǰ����
extern uint16_t humiOpening;			//��ʪ����
extern uint16_t humiOpeningFromPLC;
extern uint16_t powerProportion;		//��������
extern uint16_t humiCurrentUpperLimit;
extern uint16_t ctrlToDisplayTemp[];
extern uint16_t ctrlToPLCTemp[];
//extern uint8_t humiMode;
extern uint16_t extraDrainWaterTime;
extern uint16_t autoDrainWaterTime;
extern uint16_t cleanDrainWaterTime;

extern volatile uint16_t ADC_ConvertedValue[];
extern uint32_t ADC_Average[];


void dialSwitchInit();
void dataProcessing(void);