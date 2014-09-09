#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "GUIAPP_Main.h"
#include "GUI.h"


//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTC时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;//PC6,7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC6,7

	EXTIX_Init();
}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;

//1，KEY1按下
//2，KEY2按下 

//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;

	}else if(KEY1==1&&KEY2==1)key_up=1; 	    
 	return 0;// 无按键按下
}


void EXTIX_Init(void)
{	
	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;



  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	

  
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource6);
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource7);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line6;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	

		EXTI_InitStructure.EXTI_Line=EXTI_Line7;	//KEY2
		EXTI_Init(&EXTI_InitStructure);	


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  	NVIC_Init(&NVIC_InitStructure); 
}



void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line6) != RESET)  
   {  
				delay_ms(10);//
				if(KEY1==0)	
				{
					LED2=!LED2;
				}
       EXTI_ClearITPendingBit(EXTI_Line6);
   }  
  
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)  
   {  	
				delay_ms(10);//
				if(KEY2==0)	
				{
					if(WM_IsWindow(hWinNow))
					{
						WM_DeleteWindow(hWinNow);			//	关闭窗口	
					}
				}
       EXTI_ClearITPendingBit(EXTI_Line7);
   }        
}

