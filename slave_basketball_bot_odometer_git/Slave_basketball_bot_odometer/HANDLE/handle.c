#include "data.h"
#include "handle.h"
#include "math.h"

extern robot basketballbot;
 u8 Palstance[30];

int count;
 float error;
 float x_x;
 float x_angle;
 float x_y;

void Data_Handle()
{    
	   Angle_Get();
     Encoder1_get();
	   Encoder2_get();
	   R_Robot_Get();
	   TF_Robot_to_Global();
	   G_Robot_Get();   
}

void Encoder1_get()
{  
     basketballbot.Encoder_1.now=TIM3->CNT;
	   TIM3->CNT=Encoder_Std;
     basketballbot.Encoder_1.change=Encoder_Std- basketballbot.Encoder_1.now;
	   basketballbot.Encoder_1.count+=  basketballbot.Encoder_1.change/4000;
	   basketballbot.Encoder_1.change_length=  basketballbot.Encoder_1.change/4000/(2*pi);
}

void Encoder2_get()
{     
	   basketballbot.Encoder_2.now=TIM4->CNT;
	   TIM4->CNT=Encoder_Std;
     basketballbot.Encoder_2.change= basketballbot.Encoder_2.now-Encoder_Std;
	   basketballbot.Encoder_2.count+=  basketballbot.Encoder_2.change/4000;
	   basketballbot.Encoder_2.change_length=  basketballbot.Encoder_2.change/4000/(2*pi);
}	

void R_Robot_Get()
{  
	   basketballbot.robot_real_speed.w=basketballbot.IMU1500.change*2*pi/360/ TIM_14_Duration;
     basketballbot.globel_real_speed.w= basketballbot.robot_real_speed.w;
   	 basketballbot.robot_real_speed.y=  basketballbot.Encoder_1.change_length / TIM_14_Duration;
     basketballbot.robot_real_speed.x=  basketballbot.Encoder_2.change_length / TIM_14_Duration+basketballbot.robot_real_speed.w*Encoder2_distance;
	/*   if(fabs(basketballbot.robot_real_speed.y)<0.1)
		 {
	   if( basketballbot.robot_real_speed.x>0)  
			  basketballbot.robot_real_speed.y-= basketballbot.robot_real_speed.x*8/400;
	   
		 else basketballbot.robot_real_speed.y-= basketballbot.robot_real_speed.x*8/400;
	   }
		 
		 
     if(fabs(basketballbot.robot_real_speed.x)<0.1)
		 {
		 if( basketballbot.robot_real_speed.y>0)  
			   basketballbot.robot_real_speed.x-= basketballbot.robot_real_speed.y*6/400;
	   else  ;
		 */
		 
		 
	   basketballbot.robot_real_position.x=basketballbot.robot_real_position.x+ basketballbot.Encoder_2.change_length;
	   basketballbot.robot_real_position.y=basketballbot.robot_real_position.y+ basketballbot.Encoder_1.change_length;
	   basketballbot.robot_real_position.w=basketballbot.robot_real_position.w+basketballbot.IMU1500.change;
	   basketballbot.global_real_position.w=basketballbot.global_real_position.w+basketballbot.IMU1500.change;
	   if(basketballbot.global_real_position.w>360) 
  			basketballbot.global_real_position.w= basketballbot.global_real_position.w-360;
	   if(basketballbot.global_real_position.w<0) 
  			basketballbot.global_real_position.w= basketballbot.global_real_position.w+360;
}

void G_Robot_Get()
{
     basketballbot.global_real_position.x+=basketballbot.globel_real_speed.x*TIM_14_Duration;
	   basketballbot.global_real_position.y+=basketballbot.globel_real_speed.y*TIM_14_Duration;
}

void TF_Robot_to_Global()
{
     basketballbot.globel_real_speed.x=basketballbot.robot_real_speed.x*cos(basketballbot.global_real_position.w*2*pi/360)+basketballbot.robot_real_speed.y*sin(basketballbot.global_real_position.w*2*pi/360);
	   basketballbot.globel_real_speed.y=basketballbot.robot_real_speed.y*cos(basketballbot.global_real_position.w*2*pi/360)-basketballbot.robot_real_speed.x*sin(basketballbot.global_real_position.w*2*pi/360);
     basketballbot.globel_real_speed.w=basketballbot.robot_real_speed.w;
}

void Angle_Get()
{

	   basketballbot.IMU1500.change=-(basketballbot.IMU1500.now-basketballbot.IMU1500.last);
     if(basketballbot.IMU1500.change>180) basketballbot.IMU1500.change=360-basketballbot.IMU1500.change;
	   if(basketballbot.IMU1500.change<-180) basketballbot.IMU1500.change=-360-basketballbot.IMU1500.change;
    	  basketballbot.IMU1500.change*=0.9965; 
	   basketballbot.IMU1500.last=basketballbot.IMU1500.now;
}

void Data_Send_To_Master()
{
	  
	    u8 checksum;
      int cnt;
      count++;
	    if(count==20)			
			{
			   count=0;
				 LED2=!LED2;
			}
	    memcpy( Palstance, &basketballbot.global_real_position, 12 );	
    	USART2_SendChar(0xff); 
	    USART2_SendChar(0xfe);  
	
      checksum=0xfe;
	
			for(cnt=0 ;cnt < 12 ; cnt++)
			{
				USART2_SendChar( Palstance[cnt]);
				checksum=checksum+Palstance[cnt];
			}
			
			USART2_SendChar(checksum);

}
