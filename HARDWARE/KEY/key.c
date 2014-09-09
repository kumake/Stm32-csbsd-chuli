#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "GUIAPP_Main.h"
#include "GUI.h"


//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTCʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;//PC6,7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC6,7

	EXTIX_Init();
}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;

//1��KEY1����
//2��KEY2���� 

//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==0||KEY2==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;

	}else if(KEY1==1&&KEY2==1)key_up=1; 	    
 	return 0;// �ް�������
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
						WM_DeleteWindow(hWinNow);			//	�رմ���	
					}
				}
       EXTI_ClearITPendingBit(EXTI_Line7);
   }        
}

