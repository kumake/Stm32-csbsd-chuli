#include "font_change.h"
#include "malloc.h"
#include "GUI.h"
#include "ff.h"
#include <string.h>




//gbk转unicode
u16 GBK_To_UNI(u8 *src,u16 *dst)
{
	u16 temp; 
	u8 buf[2];
	u16 count;

	count=0;

		while(*src!=0)
		{
			if(*src<0X81)	//非汉字
			{
				temp=(u16)ff_convert((WCHAR)*src,1);
				src++;
			}else 			//汉字,占2个字节
			{
				buf[1]=*src++;
				buf[0]=*src++; 
				temp=(u16)ff_convert((WCHAR)*(u16*)buf,1); 
			}
			*dst++=temp;
			count++;
		}


	*dst=0;//添加结束符
	return count;
} 

//gbk转utf8
void GBK_To_UTF8(u8 *src,u8 *dst)
{
	u16 count;
	u8 * tempbuffer;

	count=GBK_To_UNI(src,(u16 *)dst);

	tempbuffer=(u8 *)mymalloc(SRAMEX,count*3);//申请
	GUI_UC_ConvertUC2UTF8((const unsigned short *)dst,count,(char *)tempbuffer,count*3);  //unicoude 转UTF8
	strcpy((char *)dst,(char *)tempbuffer); //复制回来

	myfree(SRAMEX,tempbuffer);//释放内存
}



