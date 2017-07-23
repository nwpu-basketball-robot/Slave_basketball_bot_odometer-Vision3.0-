#ifndef __UART4_H
#define __UART4_H
#include "sys.h" 

void UART4_Init(void);
void UART4_IRQHandler(void);
void UART4_SendChar(unsigned char b);
void get_imudata(void);
void clean_imudate(void);

#endif
