#include "beep.h"

	   

//��ʼ��PG6 PG7 PG8Ϊ�����.��ʹ���������ڵ�ʱ��		    
//BEEP IO��ʼ��
void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ��PF�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOF, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOG.678
 GPIO_ResetBits(GPIOF,GPIO_Pin_11);						 //�����
}
