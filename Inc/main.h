/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define monitor_switch_Pin GPIO_PIN_13
#define monitor_switch_GPIO_Port GPIOC
#define monitor_water_Pin GPIO_PIN_14
#define monitor_water_GPIO_Port GPIOC
#define select_Pin GPIO_PIN_1
#define select_GPIO_Port GPIOC
#define key_drain_Pin GPIO_PIN_2
#define key_drain_GPIO_Port GPIOC
#define key_toggle_Pin GPIO_PIN_3
#define key_toggle_GPIO_Port GPIOC
#define dig_s4_Pin GPIO_PIN_4
#define dig_s4_GPIO_Port GPIOA
#define dig_a_Pin GPIO_PIN_5
#define dig_a_GPIO_Port GPIOA
#define dig_f_Pin GPIO_PIN_6
#define dig_f_GPIO_Port GPIOA
#define dig_s3_Pin GPIO_PIN_7
#define dig_s3_GPIO_Port GPIOA
#define dig_s2_Pin GPIO_PIN_4
#define dig_s2_GPIO_Port GPIOC
#define dig_b_Pin GPIO_PIN_5
#define dig_b_GPIO_Port GPIOC
#define dig_e_Pin GPIO_PIN_0
#define dig_e_GPIO_Port GPIOB
#define dig_d_Pin GPIO_PIN_1
#define dig_d_GPIO_Port GPIOB
#define dig_dp_Pin GPIO_PIN_2
#define dig_dp_GPIO_Port GPIOB
#define dig_c_Pin GPIO_PIN_10
#define dig_c_GPIO_Port GPIOB
#define dig_g_Pin GPIO_PIN_11
#define dig_g_GPIO_Port GPIOB
#define dig_s1_Pin GPIO_PIN_12
#define dig_s1_GPIO_Port GPIOB
#define relay4_Pin GPIO_PIN_7
#define relay4_GPIO_Port GPIOC
#define relay3_Pin GPIO_PIN_8
#define relay3_GPIO_Port GPIOC
#define relay2_Pin GPIO_PIN_9
#define relay2_GPIO_Port GPIOC
#define relay1_Pin GPIO_PIN_8
#define relay1_GPIO_Port GPIOA
#define s1_pin6_Pin GPIO_PIN_9
#define s1_pin6_GPIO_Port GPIOA
#define s1_pin5_Pin GPIO_PIN_10
#define s1_pin5_GPIO_Port GPIOA
#define s1_pin4_Pin GPIO_PIN_11
#define s1_pin4_GPIO_Port GPIOA
#define s1_pin3_Pin GPIO_PIN_12
#define s1_pin3_GPIO_Port GPIOA
#define green_led_pwm_Pin GPIO_PIN_15
#define green_led_pwm_GPIO_Port GPIOA
#define red_led_Pin GPIO_PIN_10
#define red_led_GPIO_Port GPIOC
#define s1_pin2_Pin GPIO_PIN_11
#define s1_pin2_GPIO_Port GPIOC
#define s1_pin1_Pin GPIO_PIN_12
#define s1_pin1_GPIO_Port GPIOC
#define s2_pin6_Pin GPIO_PIN_2
#define s2_pin6_GPIO_Port GPIOD
#define s2_pin5_Pin GPIO_PIN_3
#define s2_pin5_GPIO_Port GPIOB
#define s2_pin4_Pin GPIO_PIN_4
#define s2_pin4_GPIO_Port GPIOB
#define s2_pin3_Pin GPIO_PIN_5
#define s2_pin3_GPIO_Port GPIOB
#define USART1_RX_Pin GPIO_PIN_7
#define USART1_RX_GPIO_Port GPIOB
#define s2_pin2_Pin GPIO_PIN_8
#define s2_pin2_GPIO_Port GPIOB
#define s2_pin1_Pin GPIO_PIN_9
#define s2_pin1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
