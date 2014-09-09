#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"	
#include "sys.h" 

u8* Usart2_Send(u8* pstring);	  	
u8 Usart2_Putc(u8 ch);
void Usart2_Init(u32 bound);
#endif


