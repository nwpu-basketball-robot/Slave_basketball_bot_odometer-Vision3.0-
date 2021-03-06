#include "usart.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "string.h"
#include "data.h"

extern robot basketballbot;
void USART1_Init()
{

     USART_InitTypeDef USART1_InitStructure;
	   GPIO_InitTypeDef  GPIO_InitStructure;
	   NVIC_InitTypeDef  NVIC_InitStructure;
	   
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
  	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
  	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	  GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	  USART1_InitStructure.USART_BaudRate = 115200;//波特率设置
	  USART1_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	  USART1_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	  USART1_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	  USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	  USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART1_InitStructure); //初始化串口1
	   
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
		USART_Cmd(USART1, ENABLE);  //使能串口1
		
	  USART_ClearFlag(USART1, USART_FLAG_TC);
     
	   //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	  NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	//  USART1_SendChar(0xff);
	   LED0=!LED0;
  //  clean_imudate();
}
void USART1_SendChar(unsigned char b)
{
		while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
		USART_SendData(USART1,b);
}


enum imuRecstate								   		//状态机 
	{
		RECFF,SENDID,RECSEL,RECCHECK
	} Slave_Recstate = RECFF;
u16 checksum=0;
u8 usart1_buffer[60] = {0};
int usart1_count=0;
char data_len;	
u8 dataet[256];
int flag;
int usart1_led1_count;
void USART1_IRQHandler(void)     
{
	u8 data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
		{
			usart1_led1_count++;
			if(usart1_led1_count==150) 
			{
			   usart1_led1_count=0;
			   LED1=!LED1;
			}
			data =USART_ReceiveData(USART1);
			switch (Slave_Recstate)		
				{
				case RECFF :
					if (data == 0xff)	  
						{	
							Slave_Recstate =SENDID;	
							checksum=0;		
							usart1_count=0;
						}
			  break;
				case SENDID :	
					if (data == 0xfe)	  
						{	
							Slave_Recstate =RECSEL;	
							checksum+=data;		
						}	
					else
						{
							Slave_Recstate=RECFF;
							checksum=0;
						}
					break;
					case RECSEL :
					  checksum += data;
					  usart1_buffer[usart1_count++] = data;
						if(usart1_count >= 12)
						{
							Slave_Recstate = RECCHECK;
						}
					break;
					case RECCHECK :	
						checksum=checksum%256;
						if(data == checksum)
							{				
			          memcpy(&basketballbot.global_real_position, usart1_buffer ,12 );	
								memcpy(&basketballbot.globel_real_speed, usart1_buffer+12 ,12 );	
								if(basketballbot.robot_expected_speed.w==0)
								basketballbot.globel_expected_speed.w=-basketballbot.globel_real_speed.w/2/pi*80;
								memcpy(&basketballbot.real_position_radius, usart1_buffer ,12 );	
								basketballbot.real_position_radius.w=basketballbot.real_position_radius.w /360*2*pi;
								checksum=0;	
								Slave_Recstate = RECFF;	 
							}
						else
						{
							Slave_Recstate = RECFF;
							checksum=0;
						}
						   
						break;
									
			}				
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		USART_ClearITPendingBit(USART1, USART_IT_TC);
}


