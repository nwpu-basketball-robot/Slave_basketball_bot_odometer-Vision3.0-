#include "data.h"
#include "imu1500.h"

extern robot basketballbot;

void IMU1500_Init()
{
	   int cnt;
		 for(cnt=0;cnt<20;cnt++)
         basketballbot.IMU1500.startall+=basketballbot.IMU1500.startdata[cnt];
     basketballbot.IMU1500.last=basketballbot.IMU1500.startall / 20;	 
		 	 
		 
}

