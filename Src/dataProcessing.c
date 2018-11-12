#include "dataProcessing.h"

#define readS2Pin1			HAL_GPIO_ReadPin(s2_pin1_GPIO_Port, s2_pin1_Pin)
#define readS2Pin2			HAL_GPIO_ReadPin(s2_pin2_GPIO_Port, s2_pin2_Pin)
#define readS2Pin3			HAL_GPIO_ReadPin(s2_pin3_GPIO_Port, s2_pin3_Pin)
#define readS2Pin4			HAL_GPIO_ReadPin(s2_pin4_GPIO_Port, s2_pin4_Pin)
#define readS2Pin5			HAL_GPIO_ReadPin(s2_pin5_GPIO_Port, s2_pin5_Pin)
#define readS2Pin6			HAL_GPIO_ReadPin(s2_pin6_GPIO_Port, s2_pin6_Pin)

#define readS1Pin1			HAL_GPIO_ReadPin(s1_pin1_GPIO_Port, s1_pin1_Pin)
#define readS1Pin2			HAL_GPIO_ReadPin(s1_pin2_GPIO_Port, s1_pin2_Pin)
#define readS1Pin3			HAL_GPIO_ReadPin(s1_pin3_GPIO_Port, s1_pin3_Pin)
#define readS1Pin4			HAL_GPIO_ReadPin(s1_pin4_GPIO_Port, s1_pin4_Pin)
#define readS1Pin5			HAL_GPIO_ReadPin(s1_pin5_GPIO_Port, s1_pin5_Pin)
#define readS1Pin6			HAL_GPIO_ReadPin(s1_pin6_GPIO_Port, s1_pin6_Pin)

uint8_t switchSetFlag;						//���뿪�س�ʼ����־��δ��ʼ����0���ѳ�ʼ����1
uint8_t switchSetFlagV;						//���뿪�س�ʼ����־:��ѹ�������
uint8_t switchSetFlagI;						//���뿪�س�ʼ����־:�����������
uint8_t humiMode;
uint16_t alarmCode;
uint16_t humiCurrent;
uint16_t humiOpening;
uint16_t humiOpeningV;
uint16_t humiOpeningI;
uint16_t humiOpeningFromPLC;
uint16_t humiCurrentUpperLimit;
uint16_t humiVoltage = 380;
uint16_t powerProportion;

uint16_t extraDrainWaterTime;		//������ˮʱ��
uint16_t autoDrainWaterTime;		//�Զ���ˮʱ��
uint16_t cleanDrainWaterTime;		//ϴͰʱ��

uint16_t ctrlToDisplayTemp[255];
uint16_t ctrlToPLCTemp[255];

volatile uint16_t ADC_ConvertedValue[4];
uint32_t ADC_Average[4];

static void adcProcesdsing() {

	uint32_t a=0, b=0, c=0;

	for (uint8_t i = 0; i < 100; i++)
	{
		for (uint8_t j = 0; j < 4; j++) {
			ADC_Average[j] += ADC_ConvertedValue[j];
		}
	}

	humiOpeningV = ADC_Average[0] *10/ 4096;

	humiCurrent = ADC_Average[1] *6/4096;//ѡ��39R���衣0-60A-->0-60ma-->0-0.06*39*1.414V-->0-3.3V-->0-4095
										//0-600 ��Ӧ 0-4095.

	if (ADC_Average[2]<84000)
	{
		ADC_Average[2] = 84000;
	}
	humiOpeningI = (ADC_Average[2] / 100 - 840) * 999 / 3255;

	powerProportion = ADC_Average[3] * (1001 - 250) / 4096 / 100 + 250;


	if (humiOpeningV > humiOpeningI)
	{
		humiOpening = humiOpeningV;
	}
	else {
		humiOpening = humiOpeningI;
	}

	for (uint8_t i = 0; i < 4; i++)
	{
		ADC_Average[i] = 0;
	}
}

/*************************************�ɹ��ʼ���������************************************/
static uint16_t getIFromP(uint16_t p) {

	if (380 == humiVoltage || 400 == humiVoltage)
	{
		if (p < 17500)
		{
			return p*1.732 / 3 / humiVoltage * 10;
		}
		else
		{
			return p*1.732 / 3 / humiVoltage * 10 / 2;		//�����ʴ��ڵ���17500ʱ����������3����Ϊ6����������������
		}
	}

	if (220 == humiVoltage)
	{
		return p / humiVoltage * 10;
	}
}

void dialSwitchInit() {


	/***********************************  ����ģʽѡ��  **********************************/

	if (readS2Pin1 == 1)
	{
		humiMode = PROPORTIONMODE;
	}

	if (readS2Pin1 == 0)
	{
		humiMode = SWITCHMODE;
	}

	/***********************************  ��ʪ��ѹѡ��  *************************************/
	if ((readS1Pin3 == 0) && (readS1Pin4 == 0)&& (readS1Pin5 == 0) && (readS1Pin6 == 0))//0000>>120
	{
		humiVoltage = 120;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 1) && (readS1Pin4 == 0) && (readS1Pin5 == 0) && (readS1Pin6 == 0))//1000>>200
	{
		humiVoltage = 200;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 0) && (readS1Pin4 == 1) && (readS1Pin5 == 0) && (readS1Pin6 == 0))//0100>>208
	{
		humiVoltage = 208;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 0) && (readS1Pin6 == 0))//1100>>220
	{
		humiVoltage = 220;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 0) && (readS1Pin4 == 0) && (readS1Pin5 == 1) && (readS1Pin6 == 0))//0010>>230
	{
		humiVoltage = 230;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 1) && (readS1Pin4 == 0) && (readS1Pin5 == 1) && (readS1Pin6 == 0))//1010>>240
	{
		humiVoltage = 240;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 0) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 0))//0110>>276
	{
		humiVoltage = 276;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 1) && (readS1Pin6 == 0))//1110>>346
	{
		humiVoltage = 346;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 0) && (readS1Pin4 == 0) && (readS1Pin5 == 0) && (readS1Pin6 == 1))//0001>>380
	{
		humiVoltage = 380;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 1) && (readS1Pin4 == 0) && (readS1Pin5 == 0) && (readS1Pin6 == 1))//1001>>400
	{
		humiVoltage = 400;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 0) && (readS1Pin4 == 1) && (readS1Pin5 == 0) && (readS1Pin6 == 1))//0101>>415
	{
		humiVoltage = 415;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 1) && (readS1Pin4 == 1) && (readS1Pin5 == 0) && (readS1Pin6 == 1))//1101>>440
	{
		humiVoltage = 440;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 0) && (readS1Pin4 == 0) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//0011>>460
	{
		humiVoltage = 460;
		switchSetFlagV = 1;
	}
	else if ((readS1Pin3 == 1) && (readS1Pin4 == 0) && (readS1Pin5 == 1) && (readS1Pin6 == 1))//1011>>500
	{
		humiVoltage = 500;
		switchSetFlagV = 1;
	}
	else
	{
		switchSetFlagV = 0;
	}

	/***********************************  ��ʪ����ѡ��  *************************************/
	/*
	*	humiCurrentUpperLimitΪʵ�ʶ����*10������������ݴ���
	*/
	if ((readS2Pin2 == 1) && (readS2Pin3 == 1) && (readS2Pin4 == 0) && (readS2Pin5 == 0))			//2����
	{
		humiCurrentUpperLimit = getIFromP(1500);
		autoDrainWaterTime = 2;
		cleanDrainWaterTime = 90;
		switchSetFlagI = 1;
	}

	else if ((readS2Pin2 == 0) && (readS2Pin3 == 1) && (readS2Pin4 == 0) && (readS2Pin5 == 0))		//3����
	{
		humiCurrentUpperLimit = getIFromP(2300);
		autoDrainWaterTime = 2;
		cleanDrainWaterTime = 90;
		switchSetFlagI = 1;
	}

	else if ((readS2Pin2 == 1) && (readS2Pin3 == 0) && (readS2Pin4 == 0) && (readS2Pin5 == 0))		//4����
	{
		humiCurrentUpperLimit = getIFromP(3000);
		autoDrainWaterTime = 8;
		cleanDrainWaterTime = 90;
		switchSetFlagI = 1;
	}

	else if ((readS2Pin2 == 1) && (readS2Pin3 == 0) && (readS2Pin4 == 1) && (readS2Pin5 == 0))		//8����
	{
		humiCurrentUpperLimit = getIFromP(6100);
		autoDrainWaterTime = 8;
		cleanDrainWaterTime = 120;
		switchSetFlagI = 1;
	}

	else if ((readS2Pin2 == 1) && (readS2Pin3 == 1) && (readS2Pin4 == 1) && (readS2Pin5 == 0))		//13����
	{
		humiCurrentUpperLimit = getIFromP(9800);
		autoDrainWaterTime = 10;
		cleanDrainWaterTime = 180;
		switchSetFlagI = 1;
	}

	else if ((readS2Pin2 == 1) && (readS2Pin3 == 0) && (readS2Pin4 == 0) && (readS2Pin5 == 1))		//15����
	{
		humiCurrentUpperLimit = getIFromP(11400);
		autoDrainWaterTime = 10;
		cleanDrainWaterTime = 180;
		switchSetFlagI = 1;
	}

	else if ((readS2Pin2 == 1) && (readS2Pin3 == 1) && (readS2Pin4 == 0) && (readS2Pin5 == 1))		//23����
	{
		humiCurrentUpperLimit = getIFromP(17500);
		autoDrainWaterTime = 10;
		cleanDrainWaterTime = 180;
		switchSetFlagI = 1;
	}

	else if ((readS2Pin2 == 1) && (readS2Pin3 == 0) && (readS2Pin4 == 1) && (readS2Pin5 == 1))		//32����
	{
		humiCurrentUpperLimit = getIFromP(24300);
		autoDrainWaterTime = 15;
		cleanDrainWaterTime = 300;
		switchSetFlagI = 1;
	}

	else if ((readS2Pin2 == 1) && (readS2Pin3 == 1) && (readS2Pin4 == 1) && (readS2Pin5 == 1))		//45����
	{
		humiCurrentUpperLimit = getIFromP(34200);
		autoDrainWaterTime = 15;
		cleanDrainWaterTime = 300;
		switchSetFlagI = 1;
	}

	else if ((readS2Pin2 == 0) && (readS2Pin3 == 1) && (readS2Pin4 == 1) && (readS2Pin5 == 1))		//65����
	{
		humiCurrentUpperLimit = getIFromP(49400);
		autoDrainWaterTime = 20;
		cleanDrainWaterTime = 360;
		switchSetFlagI = 1;
	}
	else
	{
		switchSetFlagI = 0;
	}
	
	//�жϲ��뿪���Ƿ��ʼ����δ��ʼ����0���ѳ�ʼ����1


	if ((0 == switchSetFlagV) || (0 == switchSetFlagI))
	{
		switchSetFlag = 0;
	}
	else {
		switchSetFlag = 1;
	}

/******************************************  ������ˮʱ��  ****************************************/
	if ((readS1Pin1 == 1) && (readS1Pin2 == 1))
	{
		extraDrainWaterTime = 80;
	}

	if ((readS1Pin1 == 0) && (readS1Pin2 == 1))
	{
		extraDrainWaterTime = 30;
	}

	if ((readS1Pin1 == 1) && (readS1Pin2 == 0))
	{
		extraDrainWaterTime = 10;
	}

	if ((readS1Pin1 == 0) && (readS1Pin2 == 0))
	{
		extraDrainWaterTime = 0;
	}
}


void dataProcessing() {

	adcProcesdsing();

	ctrlToDisplayTemp[5] = humiCurrent;
	ctrlToDisplayTemp[6] = humiOpening;
	ctrlToDisplayTemp[7] = powerProportion;

	ctrlToPLCTemp[0] = humiCurrent;
	humiOpeningFromPLC = ctrlToPLCTemp[5];
}