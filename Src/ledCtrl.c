#include "ledCtrl.h"
#include "tim.h"


int pwm_value = 0;
int flag = 0;
uint8_t addFlag;
uint8_t blinkFlag;					//led闪烁标志
uint8_t breatheEnalble;


static void user_pwm_setvalue(uint16_t value)
{
	TIM2->CCR1 = value;
}

void greenLedBreatheScan()
{
	if (breatheEnalble)
	{
		if (pwm_value >1000)
		{
			addFlag = 0;
		}
		if (pwm_value<0)
		{
			addFlag = 1;
		}

		if (addFlag)
		{
			pwm_value++;
		}
		else {
			pwm_value--;
		}

		user_pwm_setvalue(pwm_value);
	}
}

void enableGreenLedBreathe(uint8_t enable)
{
	breatheEnalble = enable;
}


/**
* @brief 指示灯开关
* @param color：0：绿色。1：红色
* @param statu: 0:关闭。 1：打开
*/
void ledSwitch(uint8_t color, uint8_t statu) {
	//关闭绿灯时，要关闭pwm输出。
	if (0 == color)
	{
		if (1 == statu)
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_RESET);
			user_pwm_setvalue(1000);
		}
		else
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_RESET);
			user_pwm_setvalue(0);
		}
	}
	else if (1 == color)
	{
		if (1 == statu)
		{
			user_pwm_setvalue(0);
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_RESET);
			user_pwm_setvalue(0);
		}
	}
}

/**
* @brief 指示灯闪烁
* @param color：0：绿色。1：红色
*/
void ledBlink(uint8_t color) {

	if (0 == color)
	{
		if (1 == blinkFlag)
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_RESET);
			user_pwm_setvalue(1000);
		}
		else
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_RESET);
			user_pwm_setvalue(0);
		}
	}
	else if (1 == color)
	{
		if (1 == blinkFlag)
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_SET);
			user_pwm_setvalue(0);
		}
		else
		{
			HAL_GPIO_WritePin(red_led_GPIO_Port, red_led_Pin, GPIO_PIN_RESET);
			user_pwm_setvalue(0);
		}
	}
}