#include "digitalDisplay.h"
#include "cmsis_os.h"


static uint8_t* intToFloat(uint16_t data) {
	static uint8_t floatTemp[5];
	if (data<1000)
	{
		floatTemp[0] = (data / 100) % 10+48;
		floatTemp[1] = (data / 10) % 10 + 48;
		floatTemp[2] = '.';
		floatTemp[3] = data % 10 + 48;
		floatTemp[4] = '\0';
	}
	else
	{
		floatTemp[0] = (data / 1000) % 10 + 48;
		floatTemp[1] = (data / 100) % 10 + 48;
		floatTemp[2] = (data / 10) % 10 + 48;
		floatTemp[3] = '\0';
	}

	return floatTemp;
}


/*
num 1-4 对应 1-4数码管
data 1:显示小数点  0：不显示小数点
*/
static void pointDisplay(uint8_t num, uint8_t data) {

	HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);

	switch (num)
	{

	case 1:
		if (1 == data)
		{
			HAL_GPIO_WritePin(dig_s1_GPIO_Port, dig_s1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(dig_s2_GPIO_Port, dig_s2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(dig_s3_GPIO_Port, dig_s3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(dig_s4_GPIO_Port, dig_s4_Pin, GPIO_PIN_SET);

			HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		}
		break;
	case 2:
		if (1 == data)
		{
			HAL_GPIO_WritePin(dig_s1_GPIO_Port, dig_s1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(dig_s2_GPIO_Port, dig_s2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(dig_s3_GPIO_Port, dig_s3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(dig_s4_GPIO_Port, dig_s4_Pin, GPIO_PIN_SET);

			HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		}

		break;
	case 3:
		if (1 == data)
		{
			HAL_GPIO_WritePin(dig_s1_GPIO_Port, dig_s1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(dig_s2_GPIO_Port, dig_s2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(dig_s3_GPIO_Port, dig_s3_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(dig_s4_GPIO_Port, dig_s4_Pin, GPIO_PIN_SET);

			HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		}
		break;
	case 4:
		if (1 == data)
		{
			HAL_GPIO_WritePin(dig_s1_GPIO_Port, dig_s1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(dig_s2_GPIO_Port, dig_s2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(dig_s3_GPIO_Port, dig_s3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(dig_s4_GPIO_Port, dig_s4_Pin, GPIO_PIN_RESET);

			HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		}
		break;

	default:
		break;
	}
	osDelay(1);
}

/*
num 1-4 对应 1-4数码管
data   0，1，2，3，4，5，6，7，8，9，a,b,c,d
*/
static void digitalDisplayNum(uint8_t num, uint8_t data)
{

	HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);

	switch (num)
	{

	case 1:
		HAL_GPIO_WritePin(dig_s1_GPIO_Port, dig_s1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_s2_GPIO_Port, dig_s2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_s3_GPIO_Port, dig_s3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_s4_GPIO_Port, dig_s4_Pin, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(dig_s1_GPIO_Port, dig_s1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_s2_GPIO_Port, dig_s2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_s3_GPIO_Port, dig_s3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_s4_GPIO_Port, dig_s4_Pin, GPIO_PIN_SET);
		break;
	case 3:
		HAL_GPIO_WritePin(dig_s1_GPIO_Port, dig_s1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_s2_GPIO_Port, dig_s2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_s3_GPIO_Port, dig_s3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_s4_GPIO_Port, dig_s4_Pin, GPIO_PIN_SET);
		break;
	case 4:
		HAL_GPIO_WritePin(dig_s1_GPIO_Port, dig_s1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_s2_GPIO_Port, dig_s2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_s3_GPIO_Port, dig_s3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_s4_GPIO_Port, dig_s4_Pin, GPIO_PIN_RESET);
		break;
	default:
		break;
	}

	switch (data)
	{
	case '0':
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;

	case '1':

		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);

		break;
	case '2':
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, 1);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, 1);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, 1);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, 1);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;
	case '3':
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, 1);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, 1);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, 1);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, 1);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;
	case '4':
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, 1);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, 1);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, 1);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;
	case '5':
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, 1);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, 1);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, 1);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, 1);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;
	case '6':
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, 1);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, 1);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, 1);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, 1);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, 1);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;
	case '7':
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, 1);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, 1);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, 1);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;
	case '8':
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, 1);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, 1);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, 1);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, 1);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, 1);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, 1);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;
	case '9':
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, 1);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, 1);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, 1);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, 1);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, 0);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, 1);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;
	case 'a'://a

		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, 1);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, 1);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, 1);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, 1);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, 1);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;
	case 'b'://b
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, 1);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, 1);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, 1);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, 1);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;
	case 'c'://c
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, 1);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, 1);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, 1);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, 1);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;
	case 'd'://d
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, 1);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, 1);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, 1);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, 1);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;

	case 'e'://e
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, 1);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, 1);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, 1);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, 1);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;

	case 'f'://f
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, 1);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, 1);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, 1);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;

	case 'o'://o
		HAL_GPIO_WritePin(dig_a_GPIO_Port, dig_a_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_b_GPIO_Port, dig_b_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_c_GPIO_Port, dig_c_Pin, 1);
		HAL_GPIO_WritePin(dig_d_GPIO_Port, dig_d_Pin, 1);
		HAL_GPIO_WritePin(dig_e_GPIO_Port, dig_e_Pin, 1);
		HAL_GPIO_WritePin(dig_f_GPIO_Port, dig_f_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(dig_g_GPIO_Port, dig_g_Pin, 1);
		HAL_GPIO_WritePin(dig_dp_GPIO_Port, dig_dp_Pin, GPIO_PIN_RESET);
		break;
	default:
		break;
	}
	osDelay(1);
}

/*
num:1-4 对应 a,b,c,d
data 对应具体的数值/10 保留一位小数
*/
void disPlayData(uint8_t num, uint16_t data)
{

	uint8_t *p = intToFloat(data);
	uint8_t bits = 3;


	while (*p)
	{
		if (*p != '.') {

			digitalDisplayNum(bits, *p);
			bits--;
		}
		else
		{
			pointDisplay(2, 1);
		}
		p++;
	}

	switch (num)
	{
	case 1:
		digitalDisplayNum(4, 'a');
		break;
	case 2:
		digitalDisplayNum(4, 'b');
		break;
	case 3:
		digitalDisplayNum(4, 'c');
		break;
	case 4:
		digitalDisplayNum(4, 'd');
		break;
	case 5:
		digitalDisplayNum(4, 'e');
		break;
	default:
		break;
	}
}

/*
num:1-4 对应 a,b,c,d
string 字符串
*/
void disPlayString(uint8_t num, char* string)
{
	uint8_t bits = 3;
	while (bits)
	{
		digitalDisplayNum(bits, *string);
		string++;
		bits--;
	}
	switch (num)
	{
	case 1:
		digitalDisplayNum(4, 'a');
		break;
	case 2:
		digitalDisplayNum(4, 'b');
		break;
	case 3:
		digitalDisplayNum(4, 'c');
		break;
	case 4:
		digitalDisplayNum(4, 'd');
		break;
	case 5:
		digitalDisplayNum(4, 'e');
		break;
	default:
		break;
	}
}