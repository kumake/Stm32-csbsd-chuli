#include "xdelay.h"
#include "sys.h"

 
//粗延时函数，微秒
void xdelay_us(u16 time)
{
	u16 i=0;
	while(time--)
	{
		i=10;//自己定义
		while(i--);
	}
}
//毫秒级的延时
void xdelay_ms(u16 time)
{
	u16 i=0;
	while(time--)
	{
		i=12000;//自己定义
		while(i--);
	}
}



