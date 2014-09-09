#include "fifo.h"
#include "sram.h"
#include "malloc.h" 
#include <string.h>	   




void Fifo_Init(FIFO * fifo,u32 buffer_len)
{

	fifo->pBuffer=(u8 *)mymalloc(SRAMEX,buffer_len);//申请50K字节
	fifo->Start=0;
	fifo->End=0;
	fifo->Len=buffer_len;
}



//向FIFO中存进一个字节
void Fifo_In_Byte(FIFO * fifo, u8 b_value)
{
	fifo->pBuffer[fifo->End]=b_value;
	fifo->End++;

	if 	(fifo->End==fifo->Len)	//判断是否到最后
	{
		fifo->End=0;
	}


	//溢出的情况，修改了fifo->Start，读取的时候可能会出问题，尽量不要溢出
	if (fifo->Start==fifo->End)
	{
		 fifo->Start++;
	}

	if 	(fifo->Start==fifo->Len)	//判断是否到最后
	{
		fifo->Start=0;
	}

}


//从FIFO中输出一行(返回得到的字符数)
u16 Fifo_Out_Line(FIFO * fifo, u8 * tempBuffer)
{
	u16 i;
	
	u32 temp_start;
	u32 temp_end;

	tempBuffer[0]='\0';

	temp_start=fifo->Start;
	temp_end=fifo->End;


	//清除前面的\r\n
	while((temp_start!=temp_end)&&((fifo->pBuffer[temp_start]=='\r')||(fifo->pBuffer[temp_start]=='\n')))
	{
		temp_start++;
		if 	(temp_start==fifo->Len)	//判断是否到最后
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
		   if 	(temp_start==fifo->Len)	//判断是否到最后
			{
				temp_start=0;
			}
	
	}

	if ((temp_start==temp_end))   //结束
	{
		tempBuffer[0]='\0';
		return 0;
	}

	//清除后面的的\r\n
	while((temp_start!=temp_end)&&((fifo->pBuffer[temp_start]=='\r')||(fifo->pBuffer[temp_start]=='\n')))
	{
		temp_start++;
		if 	(temp_start==fifo->Len)	//判断是否到最后
			{
				temp_start=0;
			}
	}

	fifo->Start=temp_start;

	return i;
}


//从FIFO中输出最后一行
u16 Fifo_Out_LastLine(FIFO * fifo, u8 * tempBuffer)
{
   u16 i;
   u8 * m_buffer;
   u16 m_len;
   m_len=100;

   m_buffer=(u8 *)mymalloc(SRAMEX,m_len);//申请


	while(1)
	{
		i=Fifo_Out_Line(fifo, tempBuffer);
		if (i)
		{
			if(i>m_len)
			{
			   myfree(SRAMEX,m_buffer);
			   m_len=i;
			   m_buffer=(u8 *)mymalloc(SRAMEX,m_len);//申请
			}
			
			strcpy((char *)m_buffer,(char *)tempBuffer); //复制
		}
		
		if (i==0) break;
	}

	 strcpy((char *)tempBuffer,(char *)m_buffer); //复制回来
	 
	 myfree(SRAMEX,m_buffer);//释放内存
	 return  strlen((char *)tempBuffer);
}


//将FIFO中的数据清空
void Fifo_Clear(FIFO * fifo, u8 * tempBuffer)
{
	 fifo->Start=0;
	 fifo->End=0;
}



