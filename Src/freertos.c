/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "iwdg.h"
#include "digitalDisplay.h"
#include "dataProcessing.h"
#include "humiCtrl.h"
#include "tim.h"
#include "ledCtrl.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId dataProcessingHandle;
osThreadId checkKeyTaskHandle;
osThreadId displayTaskHandle;
osThreadId humiCtrlTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDataProcessingTask(void const * argument);
void StartcheckKeyTask(void const * argument);
void StartDisplayTask(void const * argument);
void StartHumiCtrlTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of dataProcessing */
  osThreadDef(dataProcessing, StartDataProcessingTask, osPriorityNormal, 0, 128);
  dataProcessingHandle = osThreadCreate(osThread(dataProcessing), NULL);

  /* definition and creation of checkKeyTask */
  osThreadDef(checkKeyTask, StartcheckKeyTask, osPriorityNormal, 0, 128);
  checkKeyTaskHandle = osThreadCreate(osThread(checkKeyTask), NULL);

  /* definition and creation of displayTask */
  osThreadDef(displayTask, StartDisplayTask, osPriorityIdle, 0, 128);
  displayTaskHandle = osThreadCreate(osThread(displayTask), NULL);

  /* definition and creation of humiCtrlTask */
  osThreadDef(humiCtrlTask, StartHumiCtrlTask, osPriorityNormal, 0, 128);
  humiCtrlTaskHandle = osThreadCreate(osThread(humiCtrlTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDataProcessingTask */
/**
  * @brief  Function implementing the dataProcessing thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDataProcessingTask */
void StartDataProcessingTask(void const * argument)
{

  /* USER CODE BEGIN StartDataProcessingTask */
	/* Infinite loop */
	for (;;)
	{
		HAL_IWDG_Refresh(&hiwdg);
		dataProcessing();
		alarmLampHandle();
		osDelay(100);
	}
  /* USER CODE END StartDataProcessingTask */
}

/* USER CODE BEGIN Header_StartcheckKeyTask */
/**
* @brief Function implementing the checkKeyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartcheckKeyTask */
void StartcheckKeyTask(void const * argument)
{
  /* USER CODE BEGIN StartcheckKeyTask */
	/* Infinite loop */
	for (;;)
	{
		keyScan();
		osDelay(1);
	}
  /* USER CODE END StartcheckKeyTask */
}

/* USER CODE BEGIN Header_StartDisplayTask */
/**
* @brief Function implementing the displayTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDisplayTask */
void StartDisplayTask(void const * argument)
{
  /* USER CODE BEGIN StartDisplayTask */
	/* Infinite loop */
	for (;;)
	{
		osDelay(1);
		switch (displayNum)
		{
		case 0:
			disPlayData(1, humiCurrent);
			break;
		case 1:
			if (humiMode == PROPORTIONMODE)
			{
				disPlayData(2, humiOpening);
			}
			else
			{
				disPlayString(2, "off");
			}
			
			break;
		case 2:
			disPlayData(3, powerProportion);
			break;
		case 3:
			disPlayData(4, warningCode);
			break;
		default:
			break;
		}
		greenLedBreatheScan();
	}
  /* USER CODE END StartDisplayTask */
}

/* USER CODE BEGIN Header_StartHumiCtrlTask */
/**
* @brief Function implementing the humiCtrlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartHumiCtrlTask */
void StartHumiCtrlTask(void const * argument)
{
  /* USER CODE BEGIN StartHumiCtrlTask */
	/* Infinite loop */
	for (;;)
	{
		humiCtrl();
		osDelay(1000);
	}
  /* USER CODE END StartHumiCtrlTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
