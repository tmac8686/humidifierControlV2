#include "ledCtrl.h"
#include "tim.h"


int pwm_value = 0;
int flag = 0;
uint8_t addFlag;
uint8_t blinkFlag;					//led��˸��־
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
* @brief ָʾ�ƿ���
* @param color��0����ɫ��1����ɫ
* @param statu: 0:�رա� 1����
*/
void ledSwitch(uint8_t color, uint8_t statu) {
	//�ر��̵�ʱ��Ҫ�ر�pwm�����
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
* @brief ָʾ����˸
* @param color��0����ɫ��1����ɫ
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