#include "font_change.h"
#include "malloc.h"
#include "GUI.h"
#include "ff.h"
#include <string.h>




//gbkתunicode
u16 GBK_To_UNI(u8 *src,u16 *dst)
{
	u16 temp; 
	u8 buf[2];
	u16 count;

	count=0;

		while(*src!=0)
		{
			if(*src<0X81)	//�Ǻ���
			{
				temp=(u16)ff_convert((WCHAR)*src,1);
				src++;
			}else 			//����,ռ2���ֽ�
			{
				buf[1]=*src++;
				buf[0]=*src++; 
				temp=(u16)ff_convert((WCHAR)*(u16*)buf,1); 
			}
			*dst++=temp;
			count++;
		}


	*dst=0;//��ӽ�����
	return count;
} 

//gbkתutf8
void GBK_To_UTF8(u8 *src,u8 *dst)
{
	u16 count;
	u8 * tempbuffer;

	count=GBK_To_UNI(src,(u16 *)dst);

	tempbuffer=(u8 *)mymalloc(SRAMEX,count*3);//����
	GUI_UC_ConvertUC2UTF8((const unsigned short *)dst,count,(char *)tempbuffer,count*3);  //unicoude תUTF8
	strcpy((char *)dst,(char *)tempbuffer); //���ƻ���

	myfree(SRAMEX,tempbuffer);//�ͷ��ڴ�
}



