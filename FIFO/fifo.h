#ifndef __FIFO_H
#define __FIFO_H	 
#include "sys.h"



typedef struct {
	u8* pBuffer; //�洢�ռ�
	u32 Start; //��ʼ��ַ
	u32 End; //�յ��ַ
	u32 Len; //����
} FIFO;

//��FIFO�д��һ���ֽ�
void Fifo_In_Byte(FIFO * fifo, u8 b_value);
u16 Fifo_Out_Line(FIFO * fifo, u8 * tempBuffer);
void Fifo_Init(FIFO * fifo,u32 buffer_len);
//��FIFO��������һ��
u16 Fifo_Out_LastLine(FIFO * fifo, u8 * tempBuffer);
//��FIFO�е��������
void Fifo_Clear(FIFO * fifo, u8 * tempBuffer);
 				    
#endif
