#include "data.h"

extern robot basketballbot;
int cntone;
int cnttwo;
int flag;


void USART1_Init()
{

     USART_InitTypeDef USART1_InitStructure;
	   GPIO_InitTypeDef  GPIO_InitStructure;
	   NVIC_InitTypeDef  NVIC_InitStructure;
	   
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
     
	
	
	   GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	   GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_USART1); 
	
	  
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
	   GPIO_Init(GPIOA,&GPIO_InitStructure);
	
    
	   USART1_InitStructure.USART_BaudRate = 115200;
	   USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
	   USART1_InitStructure.USART_StopBits = USART_StopBits_1;
	   USART1_InitStructure.USART_Parity = USART_Parity_No;
	   USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	   USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
     USART_Init(USART1, &USART1_InitStructure);
	   
		 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		 USART_Cmd(USART1, ENABLE);   
	
    
	  
     NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	   NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		
	   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	   NVIC_Init(&NVIC_InitStructure);	
		 
	   LED0=!LED0;
 
}


u8 IMU_data[100] = {0};
float yaw = 0;
u8 imu1500_data;
u16 imu1500_check;
int imu_cur_1500;
u8 CRC_check[2];
enum imuRecstate								                       		
	{
		RECFF1,RECFF2,RECDATA
	} imu_Recstate_1500 = RECFF1;

void USART1_IRQHandler(void)                            	
{
	u8 byte[4] = {0};

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		imu1500_data= USART_ReceiveData(USART1);   
		switch (imu_Recstate_1500)		
				{
				case RECFF1 :
					if (imu1500_data == 0xaa)	  
						{	
							imu_cur_1500=0;
							imu_Recstate_1500 =RECFF2;	
						}
			  break;
						case RECFF2 :
					if (imu1500_data == 0x55)	  
						{	
							imu_Recstate_1500 =RECDATA;	
						}
						 else imu_Recstate_1500=RECFF1;
						
			  break;
						case RECDATA :
						IMU_data[imu_cur_1500]=imu1500_data;
						imu_cur_1500++;
			
				if(imu_cur_1500==23)
				{		
					  imu_Recstate_1500 =RECFF1;
						{				
						int i,j;
						unsigned char*d = (unsigned char*)IMU_data;
						unsigned char c;
						imu1500_check = 0xffff;
						for(i=0; i<21; i++){
								imu1500_check ^= (unsigned int)(*(d++));
								for(j=0; j<8; j++){
										c = imu1500_check & 0x0001;
										imu1500_check >>= 1;
										if(c) imu1500_check ^= 0xa001;
								}
						}
						memcpy( CRC_check, &imu1500_check, 2 );
						}
            if(CRC_check[0]==IMU_data[21]&&CRC_check[1]==IMU_data[22])
						{
							 byte[0] = IMU_data[17];
			         byte[1] = IMU_data[18];
				       byte[2] = IMU_data[19];
				       byte[3] = IMU_data[20];	
							 memcpy(&yaw,byte,sizeof(float));
               if(fabs(yaw)>360||fabs(yaw)<0.0001f) goto L1;
          							
							 if(flag==0)
									{
                   basketballbot.IMU1500.last=yaw;
									 flag++;
										if( basketballbot.IMU1500.last==0) flag--;
								  }			
              	else  basketballbot.IMU1500.now=yaw;
								  
							  cntone++;
								cnttwo++;
								if(cnttwo==15)
								{	
									LED1=!LED1;	
									cnttwo=0;
								}
								if(cntone==25)
								{	
									LED1=!LED1;	
									cntone=0;
								}									
						 }
						}  
						L1:break;			
			}	
		}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);

}

