#include "data.h"
#include "handle.h"


extern robot basketballbot;

/********************************************

      此部分代码对定时器进行初始化
		  定时器3、4为正交编码器解算里程时的主要编码器
			TIM3     ---    编码器1        A  -   PC6             B  -   PC7
		  TIM4     ---    编码器2        A  -   PB6             B  -   PB7
	
      
			定时器14用于产生10ms每次的中断更新里程向Master发送有关里程信息
      
********************************************/


void TIMER_Init()
{   
	 
	  TIM3_Init();
	  TIM4_Init();
	  TIM14_Init(100-1,8400-1);

}


void TIM3_Init()

{
		GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStructure;
    TIM_ICInitTypeDef  TIM_ICInitTypeDef;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource7, GPIO_AF_TIM3);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    TIM_TimeBaseInitStructure.TIM_Prescaler = 0; 
    TIM_TimeBaseInitStructure.TIM_Period = 0xffff;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
		
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);


    TIM_ICInitTypeDef.TIM_Channel = TIM_Channel_1;
    TIM_ICInitTypeDef.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    TIM_ICInit(TIM3, &TIM_ICInitTypeDef);
    TIM_ICInitTypeDef.TIM_Channel = TIM_Channel_2;
    TIM_ICInitTypeDef.TIM_ICSelection = TIM_ICSelection_IndirectTI;
    TIM_ICInit(TIM3, &TIM_ICInitTypeDef);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
 
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

 
    TIM_SetCounter(TIM3,30000);
		

    TIM_Cmd(TIM3, ENABLE);

}



void TIM4_Init()
{
	  
    
	  GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseInitStructure;
    TIM_ICInitTypeDef  TIM_ICInitTypeDef;
    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7, GPIO_AF_TIM4);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


    TIM_TimeBaseInitStructure.TIM_Prescaler = 0; 
    TIM_TimeBaseInitStructure.TIM_Period = 0xffff;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
 
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);


    TIM_ICInitTypeDef.TIM_Channel = TIM_Channel_1;
    TIM_ICInitTypeDef.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM4, &TIM_ICInitTypeDef);
    TIM_ICInitTypeDef.TIM_Channel = TIM_Channel_2;
    TIM_ICInitTypeDef.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM4, &TIM_ICInitTypeDef);


    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
 
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

 
    TIM_SetCounter(TIM4,30000);
    TIM_Cmd(TIM4, ENABLE);

}



void TIM14_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; 
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;     
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);               
	
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE);                         
	TIM_Cmd(TIM14,ENABLE);                                            
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


//定时器3中断服务函数
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)==SET) //溢出中断
	{
		 Data_Handle();
		 Data_Send_To_Master();
		
	}
	TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //清除中断标志位
}



