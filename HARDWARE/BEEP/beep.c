#include "beep.h"

	   

//初始化PG6 PG7 PG8为输出口.并使能这两个口的时钟		    
//BEEP IO初始化
void BEEP_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);	 //使能PF端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOF, &GPIO_InitStructure);					 //根据设定参数初始化GPIOG.678
 GPIO_ResetBits(GPIOF,GPIO_Pin_11);						 //输出低
}
