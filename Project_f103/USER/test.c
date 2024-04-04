#include "main.h"

#include "test.h"


#define BLOCK_SIZE            512 /* Block Size in Bytes */

#define NUMBER_OF_BLOCKS      10  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)

uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile test_status EraseStatus = false, TransferStatus1 = false, TransferStatus2 = false;
SD_Error Status = SD_RESPONSE_NO_ERROR;




void USART1_Test(void)
{
	USART1_Configuration();
}

void ADC_Test(void)
{
	 // ADC 初始化
    ADCx_Init();

    printf("\r\n ----这是一个ADC单通道中断读取实验----\r\n");

//    ADC_ConvertedValueLocal =(float) ADC_ConvertedValue/4096*3.3;

//    printf("\r\n The current AD value = 0x%04X \r\n",
//            ADC_ConvertedValue);
//    printf("\r\n The current AD value = %f V \r\n",
//            ADC_ConvertedValueLocal);
//    printf("\r\n\r\n");

//    Delay(0xffffee);
}
	
void SD_Test(void)
{
	uint8_t status = 0;
	printf("MicroSD configration....\r\n");
	if ((status = SD_Init()) != SD_RESPONSE_NO_ERROR) 
	{
		printf("SD卡初始化失败，请确保SD卡已正确接入开发板，或换一张SD卡测试！\r\n");
    } else {
        printf("SD卡初始化成功！\r\n");
    }
    if (status == SD_RESPONSE_NO_ERROR) {

        /*single block 读写测试*/
        SD_SingleBlockTest();
        /*muti block 读写测试*/
        SD_MultiBlockTest();
    }
}

void SD_SingleBlockTest(void)
{  
  /*------------------- Block Read/Write --------------------------*/
  /* Fill the buffer to send */
  Fill_Buffer(Buffer_Block_Tx, BLOCK_SIZE, 0x320F);

  if (Status == SD_RESPONSE_NO_ERROR)
  {
    /* Write block of 512 bytes on address 0 */
    Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
    /* Check if the Transfer is finished */
  }

  if (Status == SD_RESPONSE_NO_ERROR)
  {
    /* Read block of 512 bytes from address 0 */
    Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);

  }

  /* Check the correctness of written data */
  if (Status == SD_RESPONSE_NO_ERROR)
  {
    TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);
  }
  
  if(TransferStatus1 == success)
  {
    printf("Single block 测试成功！\r\n");

  }
  else
  {
    printf("Single block 测试失败，请确保SD卡正确接入开发板，或换一张SD卡测试！\r\n");
    
  }
}

void SD_MultiBlockTest(void)
{
	 /*--------------- Multiple Block Read/Write ---------------------*/
  /* Fill the buffer to send */
  Fill_Buffer(Buffer_MultiBlock_Tx, MULTI_BUFFER_SIZE, 0x0);

  if (Status == SD_RESPONSE_NO_ERROR)
  {
    /* Write multiple block of many bytes on address 0 */
    Status = SD_WriteMultiBlocks(Buffer_MultiBlock_Tx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    /* Check if the Transfer is finished */
  }

  if (Status == SD_RESPONSE_NO_ERROR)
  {
    /* Read block of many bytes from address 0 */
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    /* Check if the Transfer is finished */
  }

  /* Check the correctness of written data */
  if (Status == SD_RESPONSE_NO_ERROR)
  {
    TransferStatus2 = Buffercmp(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
  }
  
  if(TransferStatus2 == success)
  {
    printf("Multi block 测试成功！\r\n");

  }
  else
  {
    printf("Multi block 测试失败，请确保SD卡正确接入开发板，或换一张SD卡测试！\r\n");
  }
}
test_status Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return false;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return success;
}


void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint16_t index = 0;

  for (index = 0; index < BufferLength; index++)
  {
    pBuffer[index] = index + Offset;
  }
}
