#include "data.h"

extern robot basketballbot;
u8 TF_clean_yaw=0;

void UART4_Init()
{

     USART_InitTypeDef UART4_InitStructure;
	   GPIO_InitTypeDef  GPIO_InitStructure;
	   NVIC_InitTypeDef  NVIC_InitStructure;
	   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		 RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

	   GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);
	   GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4);
	
	
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	   GPIO_Init(GPIOC,&GPIO_InitStructure); 
	
	   
     NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	   NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;	
	   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	   NVIC_Init(&NVIC_InitStructure);	
  
	   UART4_InitStructure.USART_BaudRate = 38400;
	   UART4_InitStructure.USART_WordLength = USART_WordLength_8b;
	   UART4_InitStructure.USART_StopBits = USART_StopBits_1;
	   UART4_InitStructure.USART_Parity = USART_Parity_No;
	   UART4_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	   UART4_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
     
		 USART_Init(UART4, &UART4_InitStructure); 
	
		 USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
     USART_Cmd(UART4, ENABLE); 
	  
     clean_imudate();
}


enum imuRecstate								   		//状态机 
	{
		RECFF,RECLEN,SENDID,RECSEL,RECCHECK
	} imu_Recstate_1700 = RECFF;
uint16_t imu_checksum=0;
uint8_t  imu_buffer[30] = {0};
int imu_cur_1700=0;
uint8_t datalen1;
short data_len;	
	

void UART4_IRQHandler(void)     
  {
	u8 data;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) 
	{
			data =USART_ReceiveData(UART4);	
			switch (imu_Recstate_1700)		
				{
				case RECFF :
					if (data == 0x68)	  
						{	
						  LED0=!LED0;
							imu_Recstate_1700 =RECLEN;	
							imu_checksum=0;		
							imu_cur_1700=0;
						}
			  break;
				case RECLEN :	
					imu_checksum += data;	
				  data_len  = data;
				  imu_Recstate_1700=SENDID;
			  break;
				case SENDID :	
					if (data == 0xFE)	  
						{	
							imu_Recstate_1700 =RECSEL;	
							imu_checksum+=data;		
						}	
					else
						{
							imu_Recstate_1700=RECFF;
							imu_checksum=0;
						}
					break;
					case RECSEL :
					  imu_checksum += data;
					  imu_buffer[imu_cur_1700++] = data;
						if(imu_cur_1700 >= data_len-3)
						{
							imu_Recstate_1700 = RECCHECK;
						}
					break;
					case RECCHECK :	
						imu_checksum=imu_checksum%256;
						if(data == imu_checksum)
							{				
								get_imudata();
								imu_checksum=0;	
								imu_Recstate_1700 = RECFF;	 
							}
						else
						{
							imu_Recstate_1700 = RECFF;
							imu_checksum=0;
						}
						   
						break;
									
			}
				
		}
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
}


void get_imudata()
{
	
	if(imu_buffer[0]==0x84)
		basketballbot.IMU1700.now=imu_buffer[7]%16*100+imu_buffer[8]/16*10+imu_buffer[8]%16+imu_buffer[9]/16*0.1f+imu_buffer[9]%16*0.01f;
	if(imu_buffer[0]==0xA8) 
		TF_clean_yaw=1;           //清除飘移指令返回值	
}

void UART4_SendChar(unsigned char b)
{
		while (USART_GetFlagStatus(UART4,USART_FLAG_TC) == RESET);
		USART_SendData(UART4,b);
}
void clean_imudate()         //清除飘移指令，不是清零指令
{

		u8 i=0;
	while(!TF_clean_yaw&&i<0x10)
	{
		i++;
	  UART4_SendChar(0x68);
		UART4_SendChar(0x04);
		UART4_SendChar(0x00);
		UART4_SendChar(0x28);
		UART4_SendChar(0x2c);
		delay_us(10);
	}
	TF_clean_yaw=0;
}

void UART4_start()
{
	
	  u8 i=0;
	  UART4_SendChar(0x68);
		UART4_SendChar(0x05);
		UART4_SendChar(0x00);
		UART4_SendChar(0x0c);
		UART4_SendChar(0x11);
	
		
	while(!TF_clean_yaw&&i<0x10)
	{

		i++;
	  UART4_SendChar(0x68);
		UART4_SendChar(0x04);
		UART4_SendChar(0x00);
		UART4_SendChar(0x28);
		UART4_SendChar(0x2c);
		delay_us(10); 
	}
	
	
	
	}
