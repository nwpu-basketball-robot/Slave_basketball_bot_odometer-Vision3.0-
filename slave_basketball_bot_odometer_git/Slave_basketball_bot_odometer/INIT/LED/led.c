#include "data.h"



/********************************************

      �˲��ִ����3��LED���г�ʼ��
			LED0    ----    PB(0)
			LED1    ----    PD(8)
			LED2    ----    PA(15)

********************************************/


    
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

 
/********************************************************************/
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;                       //LED2��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                    //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;               //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //����
  GPIO_Init(GPIOA, &GPIO_InitStructure);                           //��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                        //LED1��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                    //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;               //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //����
  GPIO_Init(GPIOD, &GPIO_InitStructure);                           //��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                        //LED0��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                    //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                   //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;               //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                     //����
  GPIO_Init(GPIOB, &GPIO_InitStructure);                           //��ʼ��GPIO
	 
/********************************************************************/
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);                                //LED0��
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);                                //LED1��
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);	                               //LED2��

}


