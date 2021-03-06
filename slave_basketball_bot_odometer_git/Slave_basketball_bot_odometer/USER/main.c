#include "data.h"


//////////////////////////////////////////////////////////////////////////////////	 
//小Low STM32F407ZGT6里程解算代码
//电子组
//创建日期:2017/7/2
//版本：V3.0
//西北工业大学舞蹈机器人基地 篮球-孙杰								  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

robot basketballbot;  	               //定义一个机器人   		
		
void BASE_Init(void);                  //对所用内设初始化

int main(void)
{	  
	 BASE_Init();
	 while(1)
	 {
	 }		 
}

void BASE_Init()
{
    NVIC_Configuration();              //设置系统中断优先级分组2
	  delay_init(168);                   //初始化延时函数
	  UART4_Init();                      //初始化串口4波特率为115200,GPIOC的始终在此被使能   
	  USART1_Init();                     //初始化串口1波特率为115200,GPIOA的始终在此被使能                                         //初始化串口3波特率为115200,GPIOB的始终在此被使能   
    USART2_Init();                     //初始化串口2波特率为115200,GPIOD的始终在此被使能 
    TIMER_Init();                      //初始化各定时器,GPIOB的始终在此被使能 
	  LED_Init();					               //初始化LED                                        	

}

