#include "fifo.h"
#include "sram.h"
#include "malloc.h" 
#include <string.h>	   




void Fifo_Init(FIFO * fifo,u32 buffer_len)
{

	fifo->pBuffer=(u8 *)mymalloc(SRAMEX,buffer_len);//����50K�ֽ�
	fifo->Start=0;
	fifo->End=0;
	fifo->Len=buffer_len;
}



//��FIFO�д��һ���ֽ�
void Fifo_In_Byte(FIFO * fifo, u8 b_value)
{
	fifo->pBuffer[fifo->End]=b_value;
	fifo->End++;

	if 	(fifo->End==fifo->Len)	//�ж��Ƿ����
	{
		fifo->End=0;
	}


	//�����������޸���fifo->Start����ȡ��ʱ����ܻ�����⣬������Ҫ���
	if (fifo->Start==fifo->End)
	{
		 fifo->Start++;
	}

	if 	(fifo->Start==fifo->Len)	//�ж��Ƿ����
	{
		fifo->Start=0;
	}

}


//��FIFO�����һ��(���صõ����ַ���)
u16 Fifo_Out_Line(FIFO * fifo, u8 * tempBuffer)
{
	u16 i;
	
	u32 temp_start;
	u32 temp_end;

	tempBuffer[0]='\0';

	temp_start=fifo->Start;
	temp_end=fifo->End;


	//���ǰ���\r\n
	while((temp_start!=temp_end)&&((fifo->pBuffer[temp_start]=='\r')||(fifo->pBuffer[temp_start]=='\n')))
	{
		temp_start++;
		if 	(temp_start==fifo->Len)	//�ж��Ƿ����
			{
				temp_start=0;
			}
	}

	i=0;

	while(temp_start!=temp_end)
	{
		   if ((fifo->pBuffer[temp_start]=='\r')||(fifo->pBuffer[temp_start]=='\n'))
		   {
		   		tempBuffer[i++]='\0';
				break;
		   }

		   tempBuffer[i++]=fifo->pBuffer[temp_start];
		   
		   temp_start++;
		   if 	(temp_start==fifo->Len)	//�ж��Ƿ����
			{
				temp_start=0;
			}
	
	}

	if ((temp_start==temp_end))   //����
	{
		tempBuffer[0]='\0';
		return 0;
	}

	//�������ĵ�\r\n
	while((temp_start!=temp_end)&&((fifo->pBuffer[temp_start]=='\r')||(fifo->pBuffer[temp_start]=='\n')))
	{
		temp_start++;
		if 	(temp_start==fifo->Len)	//�ж��Ƿ����
			{
				temp_start=0;
			}
	}

	fifo->Start=temp_start;

	return i;
}


//��FIFO��������һ��
u16 Fifo_Out_LastLine(FIFO * fifo, u8 * tempBuffer)
{
   u16 i;
   u8 * m_buffer;
   u16 m_len;
   m_len=100;

   m_buffer=(u8 *)mymalloc(SRAMEX,m_len);//����


	while(1)
	{
		i=Fifo_Out_Line(fifo, tempBuffer);
		if (i)
		{
			if(i>m_len)
			{
			   myfree(SRAMEX,m_buffer);
			   m_len=i;
			   m_buffer=(u8 *)mymalloc(SRAMEX,m_len);//����
			}
			
			strcpy((char *)m_buffer,(char *)tempBuffer); //����
		}
		
		if (i==0) break;
	}

	 strcpy((char *)tempBuffer,(char *)m_buffer); //���ƻ���
	 
	 myfree(SRAMEX,m_buffer);//�ͷ��ڴ�
	 return  strlen((char *)tempBuffer);
}


//��FIFO�е��������
void Fifo_Clear(FIFO * fifo, u8 * tempBuffer)
{
	 fifo->Start=0;
	 fifo->End=0;
}



