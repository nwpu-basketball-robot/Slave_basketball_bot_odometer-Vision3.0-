#ifndef __DATA_H
#define __DATA_H
#include "sys.h"
#include "led.h"
#include "usart1.h"
#include "uart4.h"
#include "delay.h"
#include "usart2.h" 
#include "stm32f4xx.h"
#include "transform.h"
#include "timer.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "nvic.h"
#include "string.h"

#define  Motor_Num               4 
#define  MAX_DATA_LEN           100   //数据接收缓冲区大小
#define  wheel_R                0.075000f
#define  wheel_D                0.150000f
#define  pi                     3.141592f
#define  encoder_barrier        512*4
#define  Radius                 0.28281f              //轮子到质心的半径  需要修改
#define  TEM_PWM                5000
#define  Reduction_ratio        28.5

#define  ENCODER                1000               //定义编码器线数
#define  Motor_R                0.150000f
#define  Encoder_Std            30000
#define  pi                     3.141592f
#define  Encoder1_distance      0.235f/*********/
#define  Encoder2_distance      0.16175f/*********/
#define  TIM14_length           0.010000f

		
typedef struct motor{
	int M1;
	int M2;
	int M3;
	int M4;
} motor;


typedef struct vector{
	float x;
	float y;
	float w;
}vector;

typedef struct Encoder{   
	  
	      float now;
	      float change;
	      float change_length;
	      float count;
	      float length;	 
        }Encoder; 	    

typedef struct IMU{   
	      double start;
	      double startdata[20];
	      double startall;
	      double now;
	      double last;
	      double change;
	      int count;
        }IMU;  

typedef struct robot{
	//速度
  motor motor_speed;
	Encoder Encoder_1;
	Encoder Encoder_2;
  vector globel_real_speed;
	vector robot_real_speed;
	vector robot_real_position;
	IMU IMU1500;
	IMU IMU1700;
	vector global_real_position;
	vector real_position_radius;
	
	vector globel_expected_speed;	
	vector robot_expected_speed;
	int Control_Moder_globel;
	int Control_Moder_robot;
	
	//状态
	
	//里程
 
}robot;


#endif
				



				
				
				
				
				
				
				
				
