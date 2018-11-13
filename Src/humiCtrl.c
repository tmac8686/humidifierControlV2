#include "humiCtrl.h"
#include "dataProcessing.h"
#include "cmsis_os.h"
#include "main.h"
#include "ledCtrl.h"

#define contactorOpen		HAL_GPIO_WritePin(relay4_GPIO_Port,relay4_Pin,GPIO_PIN_SET)		//接触器开关
#define contactorClose		HAL_GPIO_WritePin(relay4_GPIO_Port,relay4_Pin,GPIO_PIN_RESET)

#define drainValveOpen		HAL_GPIO_WritePin(relay3_GPIO_Port,relay3_Pin,GPIO_PIN_SET)		//排水
#define drainValveClose		HAL_GPIO_WritePin(relay3_GPIO_Port,relay3_Pin,GPIO_PIN_RESET)

#define inletValveOpen		HAL_GPIO_WritePin(relay2_GPIO_Port,relay2_Pin,GPIO_PIN_SET)		//进水
#define inletValveClose		HAL_GPIO_WritePin(relay2_GPIO_Port,relay2_Pin,GPIO_PIN_RESET)

#define signalRelayOpen		HAL_GPIO_WritePin(relay1_GPIO_Port,relay1_Pin,GPIO_PIN_SET)		//输出信号继电器
#define signalRelayClose	HAL_GPIO_WritePin(relay1_GPIO_Port,relay1_Pin,GPIO_PIN_RESET)

#define waterLevelWarning	HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_14)						//高水位报警
#define switchSignal		HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)						//开关信号

const uint8_t KEY_TIME = 50;							//按键消抖延时时间
const uint8_t WATER_LEVEL_OFF_COUNT_CONST = 15;			//高水位报警断开每秒计数设定值
const uint8_t WATER_LEVEL_ON_COUNT_CONST = 5;			//高水位报警生效每秒计数设定值
const uint32_t NEED_WASH_BUCKET_COUNT_CONST = 259200;	//洗桶计数上限72*60*60 72小时 = 259200    测试值：30秒
const uint16_t CONTINUE_INLET_WATER = 1800;				//进水时间计时，超过30分钟，判断为进水阀或出水阀损坏	
const uint16_t EXTRA_DRAIN_WATER_TIME = 1200;			//额外排水时间 20*60 = 1200
const uint16_t MAUNAL_DRAIN_WATER_BACK_TIME = 1200;		//手动排水后的自动关闭时间
const uint16_t LOW_CURRENT_OFF_TIME = 600;				//低电流关机 测试为30秒，实际为10*60秒
const uint32_t WORK_TIME_ADDUP_CONST = 2160000;			//累计运行时间报警上限600小时 600*3600 = 2160000

uint8_t nonstopWorkFlag;			//连续工作标志
uint32_t nonstopWorkCount;			//连续工作计数


uint8_t ledBlinkFlagTemp4;			//红绿灯交错闪烁标志
uint8_t ledBlinkFlagTemp8;
uint8_t ledBlinkFlagWaterLevel;

uint8_t startLowerLimitCountFlag;	//低电流计数标志
uint16_t lowerLimitCount;			//低电流计数

uint8_t extraDrainWaterFlag;		//外部排水标志
uint16_t extraDrainWaterCount;		//外部排水计数

uint8_t manualDrainWaterFlag;		//手动排水标志
uint16_t manualDrainWaterCount;		//手动排水计数

uint8_t drainWaterFlag;				//排水标志
uint16_t drainWaterCount;			//排水计数

uint8_t overCurrentFlag;			//超电流标志
uint16_t overCurrentCount;			//超电流计数

uint8_t alarmFlag;					//报警标志

uint8_t waterValveFailureFlag;		//水阀故障标记	无故障:1;有故障:1
uint8_t inletFlag;					//进水标志		进水开：1；进水关：0
uint16_t inletTimeCount;			//进水时间计数

uint8_t allowRunFlagWashBucket;		//允许运行信号，洗桶相关
uint8_t allowRunFlagDrainWater;		//允许运行信号，排水相关
uint8_t allowRunFlagProportion;		//允许运行信号，比例相关
uint8_t proportionLessThan5Flag;	//比例信号小于5%标志
uint8_t proportionGreaterThan25Flag;//比例信号大于25%标志

uint16_t shutOffCurrentTopLimit;	//关断电流上限
uint16_t shutOffCurrentLowerLimit;	//关断电流下限
uint16_t startInletCurrent;			//开始进水电流
uint16_t startDrainCurrent;			//开始排水电流
uint16_t stopInletCurrent;			//停止进水电流

uint8_t keyLock1;					//按键触发后自锁的变量标志
uint16_t keyTimeCount1;				//按键去抖演示计数器
uint8_t keyStatus1;					//按键状态   按下：1；抬起：0

uint8_t keyLock2;					//按键触发后自锁的变量标志
uint16_t keyTimeCount2;				//按键去抖演示计数器
uint8_t keyStatus2;					//按键状态   按下：1；抬起：0

uint16_t waterLevelOffCount;					//高水位报警断开每秒计数
uint16_t waterLevelOnCount;						//高水位报警生效每秒计数
uint8_t waterLevelFlag;							//高水位报警标志
uint8_t waterLevelWarningEffect;				//高水位报警确认生效

uint8_t needWashBucketFlag;							//需要洗桶判断标志位
uint8_t startDrainWaterWashBucketFlag;				//洗桶开始排水标志位
uint8_t stopDrainWaterWashBucketFlag;				//洗桶停止排水标志位
uint32_t needWashBucketCount;						//需要洗桶判断洗桶每秒计数
uint16_t startDrainWaterWashBucketCount;			//洗桶开始排水计数
uint16_t stopDrainWaterWashBucketCount;				//洗桶停止排水计数

uint8_t washBucketStage;							//洗桶所处在的阶段
uint8_t nostopWorkTake;							
uint8_t beyond120Count;

uint8_t ledStopWorkFlag;				//停止工作
uint8_t ledNormalWorkFlag;				//正常工作
uint8_t ledWaterUpperLevelFlag;			//高水位
uint8_t ledDrainWaterHandFlag;			//手动排水
uint8_t ledReplaceBucketFlag;			//换桶提示
uint8_t ledCurrentUpperLimitFlag;		//超电流
uint8_t ledDialSwitchErrorFlag;			//拨码错误
uint8_t ledCurrentLowLimitFlag;			//低电流

uint8_t displayNum;						//数码管显示顺序
uint16_t warningCode;					//报警代码

static void osDelaySecond(int s);
static void drainWater(int s);
static void humiSuspend();
static void inletValveOpenWithLimit();
static void manualDrainWaterScan(int s);
static void alarmLampHandle();
static void extraDrainWater();

/*
		设置电流90%			设置电流110%			基准电流120%			基准电流140%
---------------------------------------------------------------------------------------
			|		排水			|					|					|
	进水		|		进水			|		停止进水		|		排水			|	15s后停止工作
			|		不动作		|					|					|

*/

void humiCtrl() {

	inletFlag = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);		//读取进水阀状态
	
	if (inletTimeCount > CONTINUE_INLET_WATER)			//进水时间计时，超过30分钟，判断为进水阀或出水阀损坏			
	{
		waterValveFailureFlag = 0;
	}

	if (1 == waterLevelWarning)			//高水位报警
	{
		waterLevelFlag = 1;
	}
	else
	{
		waterLevelFlag = 0;
	}

	if (waterLevelOnCount >= WATER_LEVEL_ON_COUNT_CONST)		//高水位报警5秒消抖
	{
		waterLevelWarningEffect = 1;
		inletValveClose;
		ledWaterUpperLevelFlag = 1;
	}
	
	if (waterLevelOffCount >= WATER_LEVEL_OFF_COUNT_CONST)		//高水位恢复正常15秒消抖
	{
		waterLevelWarningEffect = 0;
		ledWaterUpperLevelFlag = 0;
	}

	if (1 == switchSetFlag)				//拨码开关正常初始化
	{
		ledDialSwitchErrorFlag = 0;

		if (humiMode == PROPORTIONMODE)
		{
			shutOffCurrentLowerLimit = humiCurrentUpperLimit*humiOpening / 1000.0 * powerProportion / 1000.0 * 0.3;
			startInletCurrent = humiCurrentUpperLimit*humiOpening / 1000.0 * powerProportion / 1000.0 * 0.9;
			stopInletCurrent = humiCurrentUpperLimit*humiOpening / 1000.0 * powerProportion / 1000.0 * 1.1;
			startDrainCurrent = humiCurrentUpperLimit * humiOpening / 1000.0 * powerProportion / 1000.0* 1.2;

			if (humiOpening < 50)						//当比例信号低于5%是记录标志位
			{
				proportionLessThan5Flag = 1;
				proportionGreaterThan25Flag = 0;
				allowRunFlagProportion = 0;
				needWashBucketFlag = 0;					//停机洗桶计数标志
			}

			if (humiOpening > 250)						//当比例信号大于25%时记录标志位
			{
				proportionLessThan5Flag = 0;
				proportionGreaterThan25Flag = 1;
				allowRunFlagProportion = 1;
				needWashBucketFlag = 1;
			}

			if ((humiOpening >= 50)&&(humiOpening <= 250))	//当比例信号处于5%和25%之间时，根据上面的标志位来判断开关
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
			startDrainCurrent = humiCurrentUpperLimit *  powerProportion / 1000.0* 1.2;
		}

		if ((needWashBucketCount > NEED_WASH_BUCKET_COUNT_CONST)&&(1 == allowRunFlagProportion))			//接触器持续断开时间大于72小时
		{
			allowRunFlagWashBucket = 0;
		}
	
		//运行需满足四个条件：1.开关信号闭合。2.非排水状态。3.非报警。4.比例模式时，比例值大于25%
		if ((1 == allowRunFlagDrainWater) && (0 == alarmFlag) && (1 == allowRunFlagProportion)&&(1 == waterValveFailureFlag)&&(1 == allowRunFlagWashBucket))
		{

			if (1 == switchSignal)
			{
				signalRelayClose;

				if (humiCurrent >= shutOffCurrentTopLimit)			//超过关断电流，关机
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

				else if (humiCurrent >= startDrainCurrent)			//超过排水电流，排水
				{
					osDelaySecond(1);
					if (humiCurrent > startDrainCurrent) {

						ledStopWorkFlag = 0;
						ledNormalWorkFlag = 1;
						ledCurrentUpperLimitFlag = 0;
						ledCurrentLowLimitFlag = 0;

						drainWater(autoDrainWaterTime);				//此处排水该为阻塞式，因为排水时接触器会断开，无电流，会误进入其他状态
						beyond120Count++;
						if (beyond120Count >= 5 )					//基准电流超过120%，自动排水五次，触发高电流报警
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

				else if (humiCurrent >= stopInletCurrent)			//停止进水
				{
					ledStopWorkFlag = 0;
					ledNormalWorkFlag = 1;

					extraDrainWater();//正常运行20分钟后开始自动排水

					//累计工作600小时后绿灯闪烁,报警继电器吸合
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

				else if ((humiCurrent >= startInletCurrent) && (humiCurrent <= startDrainCurrent))//电流在正常工作范围内
				{
					contactorOpen;
					ledStopWorkFlag = 0;
					ledNormalWorkFlag = 1;
					ledCurrentUpperLimitFlag = 0;
					ledCurrentLowLimitFlag = 0;
					
					extraDrainWater();//正常运行20分钟后开始自动排水

					//累计工作600小时后绿灯闪烁,报警继电器吸合
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

				else if (humiCurrent < startInletCurrent)			//电流不足，进水
				{

					ledStopWorkFlag = 0;
					ledNormalWorkFlag = 1;
					ledCurrentUpperLimitFlag = 0;
					ledCurrentLowLimitFlag = 0;

					//累计工作600小时后绿灯闪烁,报警继电器吸合
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
						if (lowerLimitCount >= LOW_CURRENT_OFF_TIME)	//低电流关机 测试为30秒，实际为10*60秒
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
		else if (1 == alarmFlag)		//有报警信号
		{
			humiSuspend();
			signalRelayOpen;
			nonstopWorkFlag = 0;
		}

		else if (0 == allowRunFlagWashBucket)	//需要洗桶
		{

			if (1 == washBucketStage)
			{
				startDrainWaterWashBucketFlag = 1;
				if (startDrainWaterWashBucketCount < 30)
				{
				//	printf("进入同时进排水阶段 \n");
					ledStopWorkFlag = 0;
					ledNormalWorkFlag = 1;

					inletValveOpenWithLimit();
					drainValveOpen;
				}
				else						//超过30秒，关闭排水阀
				{
				//	printf("进入关闭排水阀阶段 \n");
					drainValveClose;
					contactorOpen;

					if ((humiCurrent > humiCurrentUpperLimit*0.9) || (1 == waterLevelWarningEffect))	//当加湿电流达到90%，或者高水位报警
					{
						stopDrainWaterWashBucketFlag = 1;		//进入下一个阶段，销毁上一个阶段的标志位
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
				//	printf("进入5分钟排水阶段 \n");
					
					if (stopDrainWaterWashBucketCount < cleanDrainWaterTime)	//不到5分钟
					{
						contactorClose;							//关闭接触器，打开排水阀，关闭进水阀
						drainValveOpen;
						inletValveClose;
					}
					else
					{
					//	printf("进入最后阶段 \n");
						drainValveClose;						//5分钟后关闭排水阀,进水阀
						inletValveClose;
						stopDrainWaterWashBucketFlag = 0;
						startDrainWaterWashBucketFlag = 0;		//复位用到的状态位和计数器
						allowRunFlagWashBucket = 1;
						stopDrainWaterWashBucketCount = 0;
						startDrainWaterWashBucketCount = 0;
						needWashBucketCount = 0;
						washBucketStage = 1;					//洗桶完毕，返回第一阶段
					}
				}
			}

		//	ledSwitch(1, 0);
			ledSwitch(0, 1);				//洗桶的时候亮绿灯
		}

		else if (0 == allowRunFlagProportion)	//外部比例信号过低，停止加湿
		{
			ledStopWorkFlag = 1;
			ledNormalWorkFlag = 0;
			ledCurrentUpperLimitFlag = 0;
			ledCurrentLowLimitFlag = 0;
			humiSuspend();

		}
		else if(0 == waterValveFailureFlag)		//水阀损坏
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

		if (1 == keyStatus1)
		{
			keyStatus1 = 0;

			if (1 == allowRunFlagDrainWater)
			{
				allowRunFlagDrainWater = 0;
			}
			else {
				allowRunFlagDrainWater = 1;
				humiCtrlInit();
			}
		}

		if (1 == keyStatus2)
		{
			keyStatus2 = 0;
			displayNum++;
			if (displayNum>3)
			{
				displayNum = 0;
			}
		}

		if (0 == allowRunFlagDrainWater)		//手动排水时，红绿，红绿交错闪烁
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

		manualDrainWaterScan(30);	//手动排水的扫描函数。时间cleanDrainWaterTime自动关闭
	}
	else
	{	
		ledDialSwitchErrorFlag = 1;
	}
	alarmLampHandle();
}


//额外排水
static void extraDrainWater() {

	extraDrainWaterFlag = 1;								//开始额外排水计时
	if (extraDrainWaterCount > EXTRA_DRAIN_WATER_TIME)		//正常运行20分钟后开始自动排水
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

//报警灯集中处理
static void alarmLampHandle() {

	if (1 == ledDialSwitchErrorFlag)								//拨码错误
	{
		switch (ledBlinkFlagTemp8)
		{
		case 0: 
			ledSwitch(1, 1);
			break;
		case 1:	
			ledSwitch(1, 0);
			break;
		case 2: 
			ledSwitch(1, 1);
			break;
		case 3:	
			ledSwitch(1, 0);
			break;
		case 4: 
			ledSwitch(0, 1);
			break;
		case 5:	
			ledSwitch(1, 0);
			break;
		case 6:	
			ledSwitch(0, 1);
			break;
		case 7:	
			ledSwitch(1, 0);
			break;
		default:
			break;
		}
	}

	else if (1 == ledDrainWaterHandFlag)							//手动排水
	{
		switch (ledBlinkFlagTemp4)
		{
		case 0:
			ledSwitch(1, 1);
			break;
		case 1:	
			ledSwitch(0, 1);
			break;
		case 2:	
			ledSwitch(1, 1);
			break;
		case 3:	
			ledSwitch(0, 1);
			break;
		default:
			break;
		}
	}

	else if(1 == ledWaterUpperLevelFlag)							//最大水位
	{
		if (ledBlinkFlagWaterLevel ==1 )
		{
			ledSwitch(1, 1);
		}
		else {
			ledSwitch(0, 1);
		}
	}

	else if(1 == ledCurrentUpperLimitFlag)							//电流超
	{
		ledSwitch(1, 1);
	}

	else if (1 == ledCurrentLowLimitFlag)							//电流低
	{
		ledBlink(1);
	}

	else if ((1 == ledNormalWorkFlag)&&(1 == ledReplaceBucketFlag))	//需要换桶
	{
		ledBlink(0);
	}
	else															//正常工作
	{
		ledSwitch(0, 1);
	}

	enableGreenLedBreathe(ledStopWorkFlag);							//停止工作 呼吸灯
}


//按键扫描函数
void keyScan() {

	if (1 == HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2))		//IO 是高电平，说明按键没有被按下，这时要及时清零一些标志位
	{
		keyLock1 = 0;			//按键自锁标志清零
		keyTimeCount1 = 0;		//按键去抖动延时计数器清零
	}
	else if(keyLock1 == 0)		//有按键按下，且是第一次被按下
	{
		keyTimeCount1++;
		if (keyTimeCount1 > KEY_TIME)
		{
			keyTimeCount1 = 0;
			keyLock1 = 1;
			keyStatus1 = 1;
		}
	}

	if (1 == HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3))		//IO 是高电平，说明按键没有被按下，这时要及时清零一些标志位
	{
		keyLock2 = 0;			//按键自锁标志清零
		keyTimeCount2 = 0;		//按键去抖动延时计数器清零
	}
	else if (keyLock2 == 0)		//有按键按下，且是第一次被按下
	{
		keyTimeCount2++;
		if (keyTimeCount2 > KEY_TIME)
		{
			keyTimeCount2 = 0;
			keyLock2 = 1;
			keyStatus2 = 1;
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

//第一次上电洗桶

void fristWashBucket(){

	allowRunFlagWashBucket = 0;
}


//加湿数据初始化
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
//	startDrainCurrent = humiCurrentUpperLimit * 1.2;

	washBucketStage = 1;								//洗桶所处在的阶段
	
	ledStopWorkFlag = 1;				
	ledNormalWorkFlag = 0;				
	ledWaterUpperLevelFlag = 0;
	ledDrainWaterHandFlag = 0;
	ledCurrentUpperLimitFlag = 0;
	ledDialSwitchErrorFlag = 0;
	ledCurrentLowLimitFlag = 0;
}

	

//手动排水
static void manualDrainWaterScan(int s) {

	if (0 == allowRunFlagDrainWater)
	{
		contactorClose;
		inletValveClose;
		drainValveOpen;
		manualDrainWaterFlag = 1;
		allowRunFlagWashBucket = 1;	//手动洗桶时关闭首次上电自动洗桶
	}
	
	if (manualDrainWaterCount > s)
	{
		manualDrainWaterFlag = 0;
		humiCtrlInit();
	}
}

//排水 S 秒
static void drainWater(int s) {
	contactorClose;
	inletValveClose;
	drainValveOpen;
	osDelaySecond(s);
	drainValveClose;
	contactorOpen;
}


//待机
static void humiSuspend() {

	contactorClose;
	inletValveClose;
	drainValveClose;
}
