#include "xdelay.h"
#include "sys.h"

 
//����ʱ������΢��
void xdelay_us(u16 time)
{
	u16 i=0;
	while(time--)
	{
		i=10;//�Լ�����
		while(i--);
	}
}
//���뼶����ʱ
void xdelay_ms(u16 time)
{
	u16 i=0;
	while(time--)
	{
		i=12000;//�Լ�����
		while(i--);
	}
}



