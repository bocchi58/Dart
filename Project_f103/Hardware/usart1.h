#ifndef _USART1_H
#define _USART1_H

#include "main.h"

/****Òý½Å****/
#define USART_TX_GPIO	GPIOA
#define USART_TX_PIN	GPIO_Pin_9
#define USART_RX_GPIO	GPIOA
#define USART_RX_PIN	GPIO_Pin_10


void USART1_Configuration(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendString(char *String);
void Serial_SendArray(uint8_t *Array,uint16_t Length);

#endif
