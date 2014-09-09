#ifndef __FIFO_H
#define __FIFO_H	 
#include "sys.h"



typedef struct {
	u8* pBuffer; //存储空间
	u32 Start; //起始地址
	u32 End; //终点地址
	u32 Len; //长度
} FIFO;

//向FIFO中存进一个字节
void Fifo_In_Byte(FIFO * fifo, u8 b_value);
u16 Fifo_Out_Line(FIFO * fifo, u8 * tempBuffer);
void Fifo_Init(FIFO * fifo,u32 buffer_len);
//从FIFO中输出最后一行
u16 Fifo_Out_LastLine(FIFO * fifo, u8 * tempBuffer);
//将FIFO中的数据清空
void Fifo_Clear(FIFO * fifo, u8 * tempBuffer);
 				    
#endif
