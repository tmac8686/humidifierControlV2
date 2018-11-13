#pragma once
#include "stm32f1xx_hal.h"
#include "main.h"


extern uint8_t needWashBucketFlag;							//需要洗桶判断标志位
extern uint8_t startDrainWaterWashBucketFlag;				//洗桶开始排水标志位
extern uint8_t stopDrainWaterWashBucketFlag;				//洗桶停止排水标志位
extern uint32_t needWashBucketCount;						//需要洗桶判断洗桶每秒计数
extern uint16_t startDrainWaterWashBucketCount;			//洗桶开始排水计数
extern uint16_t stopDrainWaterWashBucketCount;				//洗桶停止排水计数
extern uint16_t waterLevelOffCount;		//高水位报警断开每秒计数
extern uint16_t waterLevelOnCount;		//高水位报警生效每秒计数
extern uint8_t waterLevelFlag;			//高水位报警标志
extern uint8_t inletFlag;				//进水标志		进水开：1；进水关：0
extern uint16_t inletTimeCount;			//进水时间计数
extern uint8_t nonstopWorkFlag;			//连续工作标志
extern uint32_t nonstopWorkCount;		//连续工作计数
extern uint8_t ledBlinkFlagTemp4;
extern uint8_t ledBlinkFlagTemp8;
extern uint8_t ledBlinkFlagWaterLevel;
extern uint8_t extraDrainWaterFlag;
extern uint16_t extraDrainWaterCount;
extern uint8_t manualDrainWaterFlag;
extern uint16_t manualDrainWaterCount;
extern uint8_t drainWaterFlag;
extern uint16_t drainWaterCount;
extern uint8_t overCurrentFlag;
extern uint16_t overCurrentCount;
extern uint8_t startLowerLimitCountFlag;
extern uint16_t lowerLimitCount;
extern uint8_t displayNum;						//数码管显示顺序
extern uint16_t warningCode;					//报警代码

void humiCtrlInit();
void humiCtrl();
void keyScan();
void fristWashBucket();