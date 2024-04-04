#ifndef _TEST_H
#define _TEST_H

#include "main.h"

typedef enum{false = 0,success = !false}test_status;


void USART1_Test(void);
void ADC_Test(void);
void SD_Test(void);
void SD_SingleBlockTest(void);
void SD_MultiBlockTest(void);
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
test_status Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);

#endif
