#include "stm32f10x.h"                  // Device header
#include "main.h"

uint8_t status;//状态判断
uint8_t	Rx_buffer[4];//接收缓冲
uint8_t	Tx_buffer[4] = {1,2,3,4};//发送数据
uint8_t	i;
extern float ADC_Value[NOFCHANEL];

int main(void)
{
	USART1_Configuration();
	ADCx_Init();
	while(1)
	{
		ADCx_GetValue();
		printf("\r\n CH0 value = %f V \r\n",ADC_Value[0]);
		printf("\r\n CH1 value = %f V \r\n",ADC_Value[1]);
		printf("\r\n CH2 value = %f V \r\n",ADC_Value[2]);
		printf("\r\n CH3 value = %f V \r\n",ADC_Value[3]);
		printf("\r\n CH4 value = %f V \r\n",ADC_Value[4]);
		printf("\r\n CH5 value = %f V \r\n",ADC_Value[5]);		
	}      
}
