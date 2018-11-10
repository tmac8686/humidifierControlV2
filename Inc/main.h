/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

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
#define s1_pin1_Pin GPIO_PIN_9
#define s1_pin1_GPIO_Port GPIOA
#define s1_pin2_Pin GPIO_PIN_10
#define s1_pin2_GPIO_Port GPIOA
#define s1_pin3_Pin GPIO_PIN_11
#define s1_pin3_GPIO_Port GPIOA
#define s1_pin4_Pin GPIO_PIN_12
#define s1_pin4_GPIO_Port GPIOA
#define green_led_pwm_Pin GPIO_PIN_15
#define green_led_pwm_GPIO_Port GPIOA
#define red_led_Pin GPIO_PIN_10
#define red_led_GPIO_Port GPIOC
#define s1_pin5_Pin GPIO_PIN_11
#define s1_pin5_GPIO_Port GPIOC
#define s1_pin6_Pin GPIO_PIN_12
#define s1_pin6_GPIO_Port GPIOC
#define s2_pin1_Pin GPIO_PIN_2
#define s2_pin1_GPIO_Port GPIOD
#define s2_pin2_Pin GPIO_PIN_3
#define s2_pin2_GPIO_Port GPIOB
#define s2_pin3_Pin GPIO_PIN_4
#define s2_pin3_GPIO_Port GPIOB
#define s2_pin4_Pin GPIO_PIN_5
#define s2_pin4_GPIO_Port GPIOB
#define USART1_RX_Pin GPIO_PIN_7
#define USART1_RX_GPIO_Port GPIOB
#define s2_pin5_Pin GPIO_PIN_8
#define s2_pin5_GPIO_Port GPIOB
#define s2_pin6_Pin GPIO_PIN_9
#define s2_pin6_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
