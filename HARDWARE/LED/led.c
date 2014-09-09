#include "led.h"

	   

//初始化PG6 PG7 PG8为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //使能PG端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;	//端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOG, &GPIO_InitStructure);					 //根据设定参数初始化GPIOG.678
 GPIO_ResetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);						 //输出低
}
