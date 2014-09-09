#ifndef __SIM900A_H
#define __SIM900A_H
#include "sys.h"

#define SIM900A_STATUS PBin(5)// PB5


#define SIM900A_PKEY PBout(8)// PB8
#define SIM900A_NRST PBout(9)// PB9	


void SIM900A_Init(void);//≥ı ºªØ
void Usart3_Init(u32 bound);
u8* Usart3_Send(u8* pstring);
u8 Usart3_Putc(u8 ch);
		 				    
#endif
