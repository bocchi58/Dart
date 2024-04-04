#include "main.h"
#define USART1_RX_BUFFER 50
u8 rx_buffer[USART1_RX_BUFFER];
uint8_t Flag;

/**********************************************************************************************************
*函 数 名: USART1_Configuration
*功能说明: 串口初始化
*形    参: 无                   
*返 回 值: 无
**********************************************************************************************************/
void USART1_Configuration(void)
{
	GPIO_InitTypeDef gpio;
	USART_InitTypeDef usart1;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//打开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	gpio.GPIO_Pin = USART_TX_PIN;
	gpio.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出模式
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_TX_GPIO,&gpio);
	
	gpio.GPIO_Pin = USART_RX_PIN;
	gpio.GPIO_Mode = GPIO_Mode_IPU; //上拉输入模式
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART_RX_GPIO,&gpio);

	usart1.USART_BaudRate = 115200;  //波特率
	usart1.USART_WordLength = USART_WordLength_8b;  //字长8位
	usart1.USART_StopBits = USART_StopBits_1;
	usart1.USART_Parity = USART_Parity_No;  //校验位
	usart1.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件流控制
	USART_Init(USART1,&usart1);

	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
	USART_Cmd(USART1,ENABLE);	
	
	//配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	NVIC_InitStruct.NVIC_IRQChannel=USART1_IRQn;   //选择USART2的中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;		//中断使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
                        

}
/**********************************************************************************************************
*函 数 名: USART1_IRQHandler
*功能说明: USART1数据闲时中断
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void USART1_IRQHandler(void)
{
if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)   //RXNE 标志位为1 表示可以接收数据
	{
		rx_buffer[0]=USART_ReceiveData(USART1);
		Flag=1;
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);  //清除RXNE标志位
	}
}
/**********************************************************************************************************
*函 数 名: Serial_SendByte
*功能说明: 串口发送字节数据
*形    参: <uint8_t> Byte
*返 回 值: 无
**********************************************************************************************************/
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);

	
}
/**********************************************************************************************************
*函 数 名: Serial_SendArray
*功能说明: 串口发送数组
*形    参: <uint8_t*> Array
		   <uint16_t> Length
*返 回 值: 无
**********************************************************************************************************/
void Serial_SendArray(uint8_t *Array,uint16_t Length)
{
	for(int i= 0; i < Length; i++)
	{
		Serial_SendByte(Array[i]);
	}
	
}
/**********************************************************************************************************
*函 数 名: Serial_SendString
*功能说明: 串口发送字符串
*形    参: <char*> String
*返 回 值: 无
**********************************************************************************************************/
void Serial_SendString(char *String)
{
	for(int i = 0; String[i] != '\0' ; i++)
	{
		Serial_SendByte(String[i]);
	}
}
/**********************************************************************************************************
*函 数 名: fputc
*功能说明: 重定向fput函数到串口，移植printf函数
*形    参: <int> 	ch
		   <FILE*>	f
*返 回 值: 无
**********************************************************************************************************/
int fputc(int ch, FILE *f)//重定向fputc函数到串口，移植printf函数
{
	Serial_SendByte(ch);
	return ch;
}
