#include "main.h"

__IO uint16_t ADC_ConvertedValue[NOFCHANEL]={0,0,0,0,0,0};
float ADC_Value[NOFCHANEL];


/**********************************************************************************************************
*@name: 	ADCx_GPIO_Config
*@brief: 	ADC IO��ʼ��
*@param: 	��
*@retval: 	��
**********************************************************************************************************/
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin =  ADC_PIN1|ADC_PIN2|ADC_PIN3|ADC_PIN4|ADC_PIN5|ADC_PIN6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	
	// ��ʼ�� ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);				
}


/**********************************************************************************************************
*@name: 	ADCx_Mode_Config
*@brief: 	����ADC����ģʽ
*@param: 	��
*@retval: 	��
**********************************************************************************************************/
static void ADCx_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(ADC_DMA_CLK, ENABLE);
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	
	// ��λDMA������
	DMA_DeInit(ADC_DMA_CHANNEL);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC_x->DR ) );// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;// �洢����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;// ����Դ��������
	DMA_InitStructure.DMA_BufferSize = NOFCHANEL;// ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // �洢����ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;// �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	// ѭ������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	
	// ��ʼ��DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
	
	
	// ADC ģʽ����
	// ֻʹ��һ��ADC�����ڵ�ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; // ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;// ����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;// �����ⲿ����ת���������������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// ת������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = NOFCHANEL;	// ת��ͨ������
	ADC_Init(ADC_x, &ADC_InitStructure);// ��ʼ��ADC
	
	// ����ADCʱ�ӣΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// ����ADC ͨ����ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL1, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL2, 2, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL3, 3, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL4, 4, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL5, 5, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL6, 6, ADC_SampleTime_55Cycles5);
	
	// ʹ��ADC DMA ����
	ADC_DMACmd(ADC_x, ENABLE);
	
	// ����ADC ������ʼת��
	ADC_Cmd(ADC_x, ENABLE);
	
	//ADCУ׼  
	ADC_ResetCalibration(ADC_x);// ��ʼ��ADC У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC_x));// �ȴ�У׼�Ĵ�����ʼ�����
	
	ADC_StartCalibration(ADC_x);// ADC��ʼУ׼
	while(ADC_GetCalibrationStatus(ADC_x));// �ȴ�У׼���
	
	// ����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(ADC_x, ENABLE);
}

/**********************************************************************************************************
*@name: 	ADCx_Init
*@brief: 	ADC��ʼ��
*@param: 	��
*@retval: 	��
**********************************************************************************************************/
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
}
/**********************************************************************************************************
*@name: 	ADCx_GetValue
*@brief: 	������ֵ���д�����ȡADC�ɼ�����ʵ��ģ���ѹֵ
*@param: 	��
*@retval: 	��
**********************************************************************************************************/\
void ADCx_GetValue(void)
{
	ADC_Value[0] =(float) ADC_ConvertedValue[0]/4096*3.3;
	ADC_Value[1] =(float) ADC_ConvertedValue[1]/4096*3.3;
	ADC_Value[2] =(float) ADC_ConvertedValue[2]/4096*3.3;
	ADC_Value[3] =(float) ADC_ConvertedValue[3]/4096*3.3;
	ADC_Value[4] =(float) ADC_ConvertedValue[4]/4096*3.3;
	ADC_Value[5] =(float) ADC_ConvertedValue[5]/4096*3.3;
}
	
