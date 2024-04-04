#include "stm32f10x.h"                  // Device header


int Tim2_usCnt = 0;
void Timer2_Init(void)
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); //外设时钟使能

	TIM_TimeBaseInitTypeDef TIM_InitStructure;	
	TIM_InitStructure.TIM_ClockDivision= TIM_CKD_DIV1; //默认不分频
	TIM_InitStructure.TIM_CounterMode= TIM_CounterMode_Up; //向上计数
	TIM_InitStructure.TIM_Period= 0; //ARR CNT计数器
	TIM_InitStructure.TIM_Prescaler= 65535; //PSC 预分频器
	TIM_InitStructure.TIM_RepetitionCounter= 0;
	TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //NVIC优先级分组
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn; //选择中断通道，TIM2在NVIC的通道
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 1; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //响应优先级
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM2 , TIM_IT_Update , ENABLE); //使能更新中断
	TIM_Cmd(TIM2 , ENABLE);
	
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
			
    }
}


