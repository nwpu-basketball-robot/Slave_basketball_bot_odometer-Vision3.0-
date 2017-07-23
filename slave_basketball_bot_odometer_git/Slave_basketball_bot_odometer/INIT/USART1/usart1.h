#ifndef __USART1_H
#define __USART1_H
#include "stdio.h"	
#include "sys.h" 



void USART1_Init(void);
void USART1_IRQHandler(void);
void USART1_SendChar(unsigned char b);

#endif
