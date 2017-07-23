#include "data.h"

u8 datae[256];
extern robot basketballbot;

/********************************************

      �˲��ִ���Դ���3���г�ʼ��
			USART3_TX    --    GPIOB_PIN_10
		  USART3_RX    --    GPIOB_PIN_11

      USART3��Ҫ����������Ϣ��������λ������ͨ�Ż�ȡ�������������
			
********************************************/



void USART2_Init()
{

     USART_InitTypeDef USART2_InitStructure;
	   GPIO_InitTypeDef  GPIO_InitStructure;
   	 NVIC_InitTypeDef  NVIC_InitStructure;
	   
	   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);                                    //ʹ��GPIOBʱ��
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);                                    //ʹ��USART3ʱ��
     
	
	   GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);                                 //GPIOB10����ΪUSART3
	   GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);                                 //GPIOB11����ΪUSART3
	
	
	/*****************************************************************************/
	
	   //USART3�˿�����
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;                                 //GPIOB10��GPIOB11
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                                             //����IO���ù���
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                      //�ٶ�50MHz
	   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                                              //���츴�����
	   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                             //����
	   GPIO_Init(GPIOD,&GPIO_InitStructure);                                                    //��ʼ��GPIOB10��GPIOB11
	
     //USART3 ��ʼ������
	   USART2_InitStructure.USART_BaudRate = 115200;                                            //����������Ϊ115200
	   USART2_InitStructure.USART_WordLength = USART_WordLength_8b;                             //�ֳ�Ϊ8λ���ݸ�ʽ
	   USART2_InitStructure.USART_StopBits = USART_StopBits_1;                                  //һ��ֹͣλ
	   USART2_InitStructure.USART_Parity = USART_Parity_No;                                     //����żУ��λ
	   USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;         //��Ӳ������������
	   USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                        //�շ�ģʽ
     USART_Init(USART2, &USART2_InitStructure);                                               //��ʼ������3
	   
		 USART_Cmd(USART2, ENABLE);                                                               //ʹ�ܴ���3
		 USART_ClearFlag(USART2, USART_FLAG_TC);                                                  //�жϱ�־λ����
		 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);                                           //��������3�Ľ����ж�
     
	   //����3�ж����ȼ�����
     NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;                                        //ѡ�񴮿�3�ж�ͨ��
	   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;                                  //��ռ���ȼ�1
	   NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		                                    //�����ȼ�2
	   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                                    //IRQͨ��ʹ��
	   NVIC_Init(&NVIC_InitStructure);	                                                        //����ָ���Ĳ�����ʼ��VIC�Ĵ���
		
}


void USART2_SendChar(unsigned char b)
{
		while (USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
		USART_SendData(USART2,b);
}
