#include "humiCtrl.h"
#include "dataProcessing.h"
#include "cmsis_os.h"
#include "main.h"

#define contactorOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)		//�Ӵ�������
#define contactorClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)

#define drainValveOpen		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET)		//��ˮ
#define drainValveClose		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)

#define inletValveOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET)		//��ˮ
#define inletValveClose		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_RESET)

#define signalRelayOpen		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET)		//����źż̵���
#define signalRelayClose	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET)

#define waterLevelWarning	HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)						//��ˮλ����
#define switchSignal		HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15)						//�����ź�

const uint8_t KEY_TIME = 50;							//����������ʱʱ��
const uint8_t WATER_LEVEL_OFF_COUNT_CONST = 15;			//��ˮλ�����Ͽ�ÿ������趨ֵ
const uint8_t WATER_LEVEL_ON_COUNT_CONST = 5;			//��ˮλ������Чÿ������趨ֵ
const uint32_t NEED_WASH_BUCKET_COUNT_CONST = 259200;	//ϴͰ��������72*60*60 72Сʱ = 259200    ����ֵ��30��
const uint16_t CONTINUE_INLET_WATER = 1800;				//��ˮʱ���ʱ������30���ӣ��ж�Ϊ��ˮ�����ˮ����	
const uint16_t EXTRA_DRAIN_WATER_TIME = 1200;			//������ˮʱ�� 20*60 = 1200
const uint16_t MAUNAL_DRAIN_WATER_BACK_TIME = 1200;		//�ֶ���ˮ����Զ��ر�ʱ��
const uint16_t LOW_CURRENT_OFF_TIME = 600;				//�͵����ػ� ����Ϊ30�룬ʵ��Ϊ10*60��
const uint32_t WORK_TIME_ADDUP_CONST = 2160000;			//�ۼ�����ʱ�䱨������600Сʱ 600*3600 = 2160000

uint8_t nonstopWorkFlag;			//����������־
uint32_t nonstopWorkCount;			//������������

uint8_t ledBlinkFlagTemp4;			//���̵ƽ�����˸��־
uint8_t ledBlinkFlagTemp8;

uint8_t startLowerLimitCountFlag;	//�͵���������־
uint16_t lowerLimitCount;			//�͵�������

uint8_t extraDrainWaterFlag;		//�ⲿ��ˮ��־
uint16_t extraDrainWaterCount;		//�ⲿ��ˮ����

uint8_t manualDrainWaterFlag;		//�ֶ���ˮ��־
uint16_t manualDrainWaterCount;		//�ֶ���ˮ����

uint8_t drainWaterFlag;				//��ˮ��־
uint16_t drainWaterCount;			//��ˮ����

uint8_t overCurrentFlag;			//��������־
uint16_t overCurrentCount;			//����������

uint8_t blinkFlag;					//led��˸��־
uint8_t alarmFlag;					//������־

uint8_t waterValveFailureFlag;		//ˮ�����ϱ��	�޹���:1;�й���:1
uint8_t inletFlag;					//��ˮ��־		��ˮ����1����ˮ�أ�0
uint16_t inletTimeCount;			//��ˮʱ�����

uint8_t allowRunFlagWashBucket;		//���������źţ�ϴͰ���
uint8_t allowRunFlagDrainWater;		//���������źţ���ˮ���
uint8_t allowRunFlagProportion;		//���������źţ��������
uint8_t proportionLessThan5Flag;	//�����ź�С��5%��־
uint8_t proportionGreaterThan25Flag;//�����źŴ���25%��־

uint16_t shutOffCurrentTopLimit;	//�ضϵ�������
uint16_t shutOffCurrentLowerLimit;	//�ضϵ�������
uint16_t startInletCurrent;			//��ʼ��ˮ����
uint16_t startDrainCurrent;			//��ʼ��ˮ����
uint16_t stopInletCurrent;			//ֹͣ��ˮ����

uint8_t keyLock;					//���������������ı�����־
uint16_t keyTimeCount;				//����ȥ����ʾ������
uint8_t keyStatus;					//����״̬   ���£�1��̧��0

uint16_t waterLevelOffCount;					//��ˮλ�����Ͽ�ÿ�����
uint16_t waterLevelOnCount;						//��ˮλ������Чÿ�����
uint8_t waterLevelFlag;							//��ˮλ������־
uint8_t waterLevelWarningEffect;				//��ˮλ����ȷ����Ч

uint8_t needWashBucketFlag;							//��ҪϴͰ�жϱ�־λ
uint8_t startDrainWaterWashBucketFlag;				//ϴͰ��ʼ��ˮ��־λ
uint8_t stopDrainWaterWashBucketFlag;				//ϴͰֹͣ��ˮ��־λ
uint32_t needWashBucketCount;						//��ҪϴͰ�ж�ϴͰÿ�����
uint16_t startDrainWaterWashBucketCount;			//ϴͰ��ʼ��ˮ����
uint16_t stopDrainWaterWashBucketCount;				//ϴͰֹͣ��ˮ����

uint8_t washBucketStage;							//ϴͰ�����ڵĽ׶�
uint8_t nostopWorkTake;							
uint8_t beyond120Count;

uint8_t ledStopWorkFlag;				//ֹͣ����
uint8_t ledNormalWorkFlag;				//��������
uint8_t ledWaterUpperLevelFlag;			//��ˮλ
uint8_t ledDrainWaterHandFlag;			//�ֶ���ˮ
uint8_t ledReplaceBucketFlag;			//��Ͱ��ʾ
uint8_t ledCurrentUpperLimitFlag;		//������
uint8_t ledDialSwitchErrorFlag;			//�������
uint8_t ledCurrentLowLimitFlag;			//�͵���


static void osDelaySecond(int s);
static void drainWater(int s);
static void ledSwitch(uint8_t color, uint8_t statu);
static void ledBlink(uint8_t color);
static void greenLedDark();
static void humiSuspend();
static void inletValveOpenWithLimit();
static void manualDrainWaterScan(int s);
static void alarmLampHandle();
static void extraDrainWater();

/*
		���õ���90%			���õ���110%			��׼����120%			��׼����140%
---------------------------------------------------------------------------------------
			|		��ˮ			|					|					|
	��ˮ		|		��ˮ			|		ֹͣ��ˮ		|		��ˮ			|	15s��ֹͣ����
			|		������		|					|					|

*/

void humiCtrl() {

	inletFlag = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);		//��ȡ��ˮ��״̬
	
	if (inletTimeCount > CONTINUE_INLET_WATER)			//��ˮʱ���ʱ������30���ӣ��ж�Ϊ��ˮ�����ˮ����			
	{
		waterValveFailureFlag = 0;
	}

	if (1 == waterLevelWarning)			//��ˮλ����
	{
		waterLevelFlag = 1;
	}
	else
	{
		waterLevelFlag = 0;
	}

	if (waterLevelOnCount >= WATER_LEVEL_ON_COUNT_CONST)		//��ˮλ����5������
	{
		waterLevelWarningEffect = 1;
		inletValveClose;
		ledWaterUpperLevelFlag = 1;
	}
	
	if (waterLevelOffCount >= WATER_LEVEL_OFF_COUNT_CONST)		//��ˮλ�ָ�����15������
	{
		waterLevelWarningEffect = 0;
		ledWaterUpperLevelFlag = 0;
	}

	if (1 == switchSetFlag)				//���뿪��������ʼ��
	{
		ledDialSwitchErrorFlag = 0;

		if (humiMode == PROPORTIONMODE)
		{
			shutOffCurrentLowerLimit = humiCurrentUpperLimit*humiOpening / 1000.0 * powerProportion / 1000.0 * 0.3;
			startInletCurrent = humiCurrentUpperLimit*humiOpening / 1000.0 * powerProportion / 1000.0 * 0.9;
			stopInletCurrent = humiCurrentUpperLimit*humiOpening / 1000.0 * powerProportion / 1000.0 * 1.1;

			if (humiOpening < 50)						//�������źŵ���5%�Ǽ�¼��־λ
			{
				proportionLessThan5Flag = 1;
				proportionGreaterThan25Flag = 0;
				allowRunFlagProportion = 0;
				needWashBucketFlag = 0;					//ͣ��ϴͰ������־
			}

			if (humiOpening > 250)						//�������źŴ���25%ʱ��¼��־λ
			{
				proportionLessThan5Flag = 0;
				proportionGreaterThan25Flag = 1;
				allowRunFlagProportion = 1;
				needWashBucketFlag = 1;
			}

			if ((humiOpening >= 50)&&(humiOpening <= 250))	//�������źŴ���5%��25%֮��ʱ����������ı�־λ���жϿ���
			{
				if (1 == proportionLessThan5Flag)
				{
					allowRunFlagProportion = 0;
				}

				if (1 == proportionGreaterThan25Flag)
				{
					allowRunFlagProportion = 1;
					needWashBucketFlag = 1;
				}
			}
		}
		else if (humiMode == SWITCHMODE)
		{
			shutOffCurrentLowerLimit = humiCurrentUpperLimit * powerProportion / 1000 * 0.3;
			startInletCurrent = humiCurrentUpperLimit * powerProportion / 1000 * 0.9;
			stopInletCurrent = humiCurrentUpperLimit * powerProportion / 1000 * 1.1;
		}

		if ((needWashBucketCount > NEED_WASH_BUCKET_COUNT_CONST)&&(1 == allowRunFlagProportion))			//�Ӵ��������Ͽ�ʱ�����72Сʱ
		{
			allowRunFlagWashBucket = 0;
		}
	
		//�����������ĸ�������1.�����źűպϡ�2.����ˮ״̬��3.�Ǳ�����4.����ģʽʱ������ֵ����25%
		if ((1 == allowRunFlagDrainWater) && (0 == alarmFlag) && (1 == allowRunFlagProportion)&&(1 == waterValveFailureFlag)&&(1 == allowRunFlagWashBucket))
		{

			if (1 == switchSignal)
			{
				signalRelayClose;

				if (humiCurrent >= shutOffCurrentTopLimit)			//�����ضϵ������ػ�
				{
					inletValveClose;
					overCurrentFlag = 1;
					if (overCurrentCount > 15)
					{
						overCurrentFlag = 0;
						overCurrentCount = 0;
						if (humiCurrent >= shutOffCurrentTopLimit)
						{
							alarmFlag = 1;
							ledStopWorkFlag = 0;
							ledNormalWorkFlag = 0;
							ledCurrentUpperLimitFlag = 1;
						}
					}
					else
					{
						ledStopWorkFlag = 0;
						ledNormalWorkFlag = 1;
						ledCurrentUpperLimitFlag = 0;
					}
				}

				else if (humiCurrent >= startDrainCurrent)			//������ˮ��������ˮ
				{
					osDelaySecond(1);
					if (humiCurrent > startDrainCurrent) {

						ledStopWorkFlag = 0;
						ledNormalWorkFlag = 1;
						ledCurrentUpperLimitFlag = 0;
						ledCurrentLowLimitFlag = 0;

						drainWater(autoDrainWaterTime);				//�˴���ˮ��Ϊ����ʽ����Ϊ��ˮʱ�Ӵ�����Ͽ����޵����������������״̬
						beyond120Count++;
						if (beyond120Count >= 5 )					//��׼��������120%���Զ���ˮ��Σ������ߵ�������
						{
							beyond120Count = 0;
							alarmFlag = 1;
							ledStopWorkFlag = 0;
							ledNormalWorkFlag = 0;
							ledCurrentUpperLimitFlag = 1;
							ledCurrentLowLimitFlag = 0;
						}
					}
				}

				else if (humiCurrent >= stopInletCurrent)			//ֹͣ��ˮ
				{
					ledStopWorkFlag = 0;
					ledNormalWorkFlag = 1;

					extraDrainWater();//��������20���Ӻ�ʼ�Զ���ˮ

					//�ۼƹ���600Сʱ���̵���˸,�����̵�������
					if (1 == nonstopWorkFlag)
					{
						if (nonstopWorkCount > WORK_TIME_ADDUP_CONST) {
							nostopWorkTake = 1;
							signalRelayOpen;
						}
					}
					contactorOpen;
					inletValveClose;

					if (1 == nostopWorkTake)
					{
						ledReplaceBucketFlag = 1;	
					}
					else
					{
						ledReplaceBucketFlag = 0;
					}
				}

				else if ((humiCurrent >= startInletCurrent) && (humiCurrent <= startDrainCurrent))//����������������Χ��
				{
					contactorOpen;
					ledStopWorkFlag = 0;
					ledNormalWorkFlag = 1;
					ledCurrentUpperLimitFlag = 0;
					ledCurrentLowLimitFlag = 0;
					
					extraDrainWater();//��������20���Ӻ�ʼ�Զ���ˮ

					//�ۼƹ���600Сʱ���̵���˸,�����̵�������
					if (1 == nonstopWorkFlag)
					{
						if (nonstopWorkCount > WORK_TIME_ADDUP_CONST) {
							nostopWorkTake = 1;
							
							signalRelayOpen;
						}
					}

					if (1 == nostopWorkTake)
					{
						ledReplaceBucketFlag = 1;
					}
					else
					{
						ledReplaceBucketFlag = 0;
					}
				}

				else if (humiCurrent < startInletCurrent)			//�������㣬��ˮ
				{

					ledStopWorkFlag = 0;
					ledNormalWorkFlag = 1;
					ledCurrentUpperLimitFlag = 0;
					ledCurrentLowLimitFlag = 0;

					//�ۼƹ���600Сʱ���̵���˸,�����̵�������
					if (1 == nonstopWorkFlag)
					{
						if (nonstopWorkCount > WORK_TIME_ADDUP_CONST) {
							nostopWorkTake = 1;
							
							signalRelayOpen;
						}
					}

					if (1 == nostopWorkTake)
					{
						ledReplaceBucketFlag = 1;
					}
					else
					{
						ledReplaceBucketFlag = 0;
					}

					drainValveClose;
					inletValveOpenWithLimit();
					contactorOpen;
					if (humiCurrent <= shutOffCurrentLowerLimit)
					{
						startLowerLimitCountFlag = 1;
						if (lowerLimitCount >= LOW_CURRENT_OFF_TIME)	//�͵����ػ� ����Ϊ30�룬ʵ��Ϊ10*60��
						{
							lowerLimitCount = LOW_CURRENT_OFF_TIME;
							alarmFlag = 1;
							ledStopWorkFlag = 0;
							ledNormalWorkFlag = 0;
							ledCurrentUpperLimitFlag = 0;
							ledCurrentLowLimitFlag = 1;
						}
					}
				}
				
				nonstopWorkFlag = 1;
			}
			else
			{
				humiSuspend();
				nonstopWorkFlag = 0;
			}
		}
		else if (1 == alarmFlag)		//�б����ź�
		{
			humiSuspend();
			signalRelayOpen;
			nonstopWorkFlag = 0;
		}

		else if (0 == allowRunFlagWashBucket)	//��ҪϴͰ
		{

			if (1 == washBucketStage)
			{
				startDrainWaterWashBucketFlag = 1;
				if (startDrainWaterWashBucketCount < 30)
				{
					printf("����ͬʱ����ˮ�׶� \n");
					ledStopWorkFlag = 0;
					ledNormalWorkFlag = 1;

					inletValveOpenWithLimit();
					drainValveOpen;
				}
				else						//����30�룬�ر���ˮ��
				{
					printf("����ر���ˮ���׶� \n");
					drainValveClose;
					contactorOpen;

					if ((humiCurrent > humiCurrentUpperLimit*0.9) || (1 == waterLevelWarningEffect))	//����ʪ�����ﵽ90%�����߸�ˮλ����
					{
						stopDrainWaterWashBucketFlag = 1;		//������һ���׶Σ�������һ���׶εı�־λ
						startDrainWaterWashBucketFlag = 0;
						startDrainWaterWashBucketCount = 0;
						washBucketStage = 2;
					}
				}
			}
			else if(2 == washBucketStage)
			{
				if (1 == stopDrainWaterWashBucketFlag)
				{
					printf("����5������ˮ�׶� \n");
					
					if (stopDrainWaterWashBucketCount < cleanDrainWaterTime)	//����5����
					{
						contactorClose;							//�رսӴ���������ˮ�����رս�ˮ��
						drainValveOpen;
						inletValveClose;
					}
					else
					{
						printf("�������׶� \n");
						drainValveClose;						//5���Ӻ�ر���ˮ��,��ˮ��
						inletValveClose;
						stopDrainWaterWashBucketFlag = 0;
						startDrainWaterWashBucketFlag = 0;		//��λ�õ���״̬λ�ͼ�����
						allowRunFlagWashBucket = 1;
						stopDrainWaterWashBucketCount = 0;
						startDrainWaterWashBucketCount = 0;
						needWashBucketCount = 0;
						washBucketStage = 1;					//ϴͰ��ϣ����ص�һ�׶�
					}
				}
			}

			ledSwitch(1, 0);
			ledSwitch(0, 1);				//ϴͰ��ʱ�����̵�
		}

		else if (0 == allowRunFlagProportion)	//�ⲿ�����źŹ��ͣ�ֹͣ��ʪ
		{
			ledStopWorkFlag = 1;
			ledNormalWorkFlag = 0;
			ledCurrentUpperLimitFlag = 0;
			ledCurrentLowLimitFlag = 0;
			humiSuspend();

		}
		else if(0 == waterValveFailureFlag)		//ˮ����
		{
			contactorClose;
			inletValveClose;
			drainValveClose;
			ledStopWorkFlag = 0;
			ledNormalWorkFlag = 0;
			ledCurrentUpperLimitFlag = 0;
			ledCurrentLowLimitFlag = 1;
			signalRelayOpen;
		}

		if (1 == keyStatus)
		{
			keyStatus = 0;

			if (1 == allowRunFlagDrainWater)
			{
				allowRunFlagDrainWater = 0;
			}
			else {
				allowRunFlagDrainWater = 1;
				humiCtrlInit();
			}
		}

		if (0 == allowRunFlagDrainWater)		//�ֶ���ˮʱ�����̣����̽�����˸
		{
			ledStopWorkFlag = 0;
			ledNormalWorkFlag = 0;
			ledDrainWaterHandFlag = 1;
			ledCurrentUpperLimitFlag = 0;
			ledCurrentLowLimitFlag = 0;
		}
		else
		{
			ledDrainWaterHandFlag = 0;
		}

		manualDrainWaterScan(30);	//�ֶ���ˮ��ɨ�躯����ʱ��cleanDrainWaterTime�Զ��ر�
	}
	else
	{	
		ledDialSwitchErrorFlag = 1;
	}
	alarmLampHandle();
}


//������ˮ
static void extraDrainWater() {

	extraDrainWaterFlag = 1;								//��ʼ������ˮ��ʱ
	if (extraDrainWaterCount > EXTRA_DRAIN_WATER_TIME)		//��������20���Ӻ�ʼ�Զ���ˮ
	{
		extraDrainWaterCount = 0;
		extraDrainWaterFlag = 0;

		if (extraDrainWaterTime > 0)
		{
			contactorClose;
			inletValveClose;
			drainValveOpen;
			osDelaySecond(extraDrainWaterTime);
			drainValveClose;
			contactorOpen;
		}
	}

}

//�����Ƽ��д���
static void alarmLampHandle() {

	if (1 == ledDialSwitchErrorFlag)								//�������
	{
		switch (ledBlinkFlagTemp8)
		{
		case 0: ledSwitch(1, 1);
			ledSwitch(0, 0);
			break;
		case 1:	ledSwitch(1, 0);
			ledSwitch(0, 0);
			break;
		case 2: ledSwitch(1, 1);
			ledSwitch(0, 0);
			break;
		case 3:	ledSwitch(1, 0);
			ledSwitch(0, 0);
			break;
		case 4: ledSwitch(0, 0);
			ledSwitch(0, 1);
			break;
		case 5:	ledSwitch(0, 0);
			ledSwitch(0, 0);
			break;
		case 6:	ledSwitch(0, 0);
			ledSwitch(0, 1);
			break;
		case 7:	ledSwitch(0, 0);
			ledSwitch(0, 0);
			break;
		default:
			break;
		}
	}

	else if (1 == ledDrainWaterHandFlag)							//�ֶ���ˮ
	{
		switch (ledBlinkFlagTemp4)
		{
		case 0: ledSwitch(1, 1);
			ledSwitch(0, 0);
			break;
		case 1:	ledSwitch(1, 0);
			ledSwitch(0, 1);
			break;
		case 2:	ledSwitch(1, 1);
			ledSwitch(0, 0);
			break;
		case 3:	ledSwitch(1, 0);
			ledSwitch(0, 1);
			break;
		default:
			break;
		}
	}

	else if(1 == ledWaterUpperLevelFlag)							//���ˮλ
	{
		ledBlink(1);
		ledSwitch(0, 1);
	}

	else if(1 == ledCurrentUpperLimitFlag)							//������
	{
		ledSwitch(1, 1);
		ledSwitch(0, 0);
	}

	else if (1 == ledCurrentLowLimitFlag)							//������
	{
		ledBlink(1);
		ledSwitch(0, 0);
	}

	else if (1 == ledStopWorkFlag)									//ֹͣ����
	{
		ledSwitch(0, 0);
		ledSwitch(1, 0);
	}

	else if ((1 == ledNormalWorkFlag)&&(1 == ledReplaceBucketFlag))	//��Ҫ��Ͱ
	{
		ledBlink(0);
		ledSwitch(1, 0);
	}
	else															//��������
	{
		ledSwitch(0, 1);
		ledSwitch(1, 0);
	}
}


//����ɨ�躯��
void keyScan() {

	if (1 == HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15))		//IO �Ǹߵ�ƽ��˵������û�б����£���ʱҪ��ʱ����һЩ��־λ
	{
		keyLock = 0;			//����������־����
		keyTimeCount = 0;		//����ȥ������ʱ����������
	}
	else if(keyLock == 0)		//�а������£����ǵ�һ�α�����
	{
		keyTimeCount++;
		if (keyTimeCount > KEY_TIME)
		{
			keyTimeCount = 0;
			keyLock = 1;
			keyStatus = 1;
		}
	}
}


static void inletValveOpenWithLimit() {

	if (0 == waterLevelWarningEffect)
	{
		inletValveOpen;
	}
}


static void osDelaySecond(int s) {
	for (int i = 0; i < s; i++)
	{
		osDelay(1000);
	}
}

//��һ���ϵ�ϴͰ

void fristWashBucket(){

	allowRunFlagWashBucket = 0;
}


//��ʪ���ݳ�ʼ��
void humiCtrlInit() {

	contactorClose;
	drainValveClose;
	inletValveClose;
	signalRelayClose;

	needWashBucketFlag = 0;
	extraDrainWaterFlag = 0;
	startLowerLimitCountFlag = 0;
	stopDrainWaterWashBucketFlag = 0;
	startDrainWaterWashBucketFlag = 0;
	alarmFlag = 0;
	allowRunFlagWashBucket = 1;
	allowRunFlagDrainWater = 1;
	allowRunFlagProportion = 1;
	waterValveFailureFlag = 1;
	manualDrainWaterFlag = 0;

	lowerLimitCount = 0;
	overCurrentCount = 0;
	drainWaterCount = 0;
	manualDrainWaterCount = 0;
	extraDrainWaterCount = 0;
	stopDrainWaterWashBucketCount = 0;
	startDrainWaterWashBucketCount = 0;

	shutOffCurrentTopLimit = humiCurrentUpperLimit*1.4;
	startDrainCurrent = humiCurrentUpperLimit * 1.2;

	washBucketStage = 1;								//ϴͰ�����ڵĽ׶�
	
	ledStopWorkFlag = 1;				
	ledNormalWorkFlag = 0;				
	ledWaterUpperLevelFlag = 0;
	ledDrainWaterHandFlag = 0;
	ledCurrentUpperLimitFlag = 0;
	ledDialSwitchErrorFlag = 0;
	ledCurrentLowLimitFlag = 0;
}

	

//�ֶ���ˮ
static void manualDrainWaterScan(int s) {

	if (0 == allowRunFlagDrainWater)
	{
		contactorClose;
		inletValveClose;
		drainValveOpen;
		manualDrainWaterFlag = 1;
		allowRunFlagWashBucket = 1;	//�ֶ�ϴͰʱ�ر��״��ϵ��Զ�ϴͰ
	}
	
	if (manualDrainWaterCount > s)
	{
		manualDrainWaterFlag = 0;
		humiCtrlInit();
	}
}

//��ˮ S ��
static void drainWater(int s) {
	contactorClose;
	inletValveClose;
	drainValveOpen;
	osDelaySecond(s);
	drainValveClose;
	contactorOpen;
}

/**
* @brief ָʾ�ƿ���
* @param color��0����ɫ��1����ɫ
* @param statu: 0:�رա� 1����
*/
static void ledSwitch(uint8_t color, uint8_t statu) {
	//�ر��̵�ʱ��Ҫ�ر�pwm�����
	if (0 == color)
	{
		if (1 == statu)
		{
			HAL_GPIO_WritePin(green_led_GPIO_Port, green_led_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(green_led_GPIO_Port, green_led_Pin, GPIO_PIN_RESET);
		}
	}
	else if (1 == color)
	{
		if (1 == statu)
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_RESET);
		}
	}
}

/**
* @brief ָʾ����˸
* @param color��0����ɫ��1����ɫ
*/
static void ledBlink(uint8_t color) {
	//�ر��̵�ʱ��Ҫ�ر�pwm�����
	if (0 == color)
	{
		if (1 == blinkFlag)
		{
			HAL_GPIO_WritePin(green_led_GPIO_Port, green_led_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(green_led_GPIO_Port, green_led_Pin, GPIO_PIN_RESET);
		}
	}
	else if (1 == color)
	{
		if (1 == blinkFlag)
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_RESET);
		}
	}
}


//�̵ư���ר���ڴ���
static void greenLedDark() {

}


//����
static void humiSuspend() {

	contactorClose;
	inletValveClose;
	drainValveClose;
}
