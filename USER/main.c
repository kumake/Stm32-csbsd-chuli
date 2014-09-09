#include "led.h"
#include "delay.h"
#include "xdelay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "sram.h"
#include "malloc.h" 
#include "sim900a.h"
#include "timer3.h"
#include "GUI.h"
#include "touch.h"
#include "sdio_sdcard.h"
#include "ff.h"
#include "flash.h"
#include "rtc.h"
#include "beep.h"
#include "ftfile.h"
#include "fifo.h"
#include "usart2.h"
#include "GUIAPP_Main.h"

FIFO fifo_usart1,fifo_usart2,fifo_usart3;  //����123��FIFO����


//u8 usart1_buf[100];
//u8 usart2_buf[100];
//u8 usart3_buf[100];


//extern void Touch_MainTask(void);

//extern void MainTask(void);



 int main(void)
 {
	
	FATFS Fatfs_sd,Fatfs_flash;
	 
	u8 usart1_buf[100];
  u8 usart2_buf[40];
	u8 usart3_buf[100];
	WM_MESSAGE Message;
	 
	u8 led_cnt=0;


	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�


	//��ʼ���ڴ�
	FSMC_SRAM_Init();		//��ʼ���ⲿSRAM
	delay_ms(500);	//�ȴ��ⲿSRAM����
//	mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��

	//��ʼ������FIFO����
	Fifo_Init(&fifo_usart1,1024*1);
	Fifo_Init(&fifo_usart2,1024*1);
	Fifo_Init(&fifo_usart3,1024*1);

	uart_init(9600);	 	//����1��ʼ��Ϊ9600

	 //����2��ʼ��
 	Usart2_Init(9600);

	//��ʼ��LED
 	LED_Init();
	//RTC��ʼ��
	RTC_Init();
	//��������ʼ��
	BEEP_Init();
	//��ʼ������
	KEY_Init();

	//��ʼ��SIM900A
	SIM900A_Init();

	//��ʼ��FLASH
	SPI_Flash_Init();


	//��ʼ��Һ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	GUI_Init();	//��ʼ��LCD
	TP_Init();//��������ʼ��

	TIM3_Int_Init(9,7199);//10Khz�ļ���Ƶ�ʣ�������10Ϊ1ms	Tout= ((9+1)*( 7199+1))/72=1000us=1ms��	   //GUI��Ҫʱ�ӽ���
			     

	//����
	f_mount(0,&Fatfs_flash);
	f_mount(1,&Fatfs_sd);

	delay_ms(500);	//�ȴ��ⲿSRAM����

	font_init();//��������


	//RTC_Set(2014,7,31,9,55,0);//����ʱ��

	 MainTask();

	//Touch_MainTask();	//����У׼

	//GUIDEMO_Main();  //GUI��������
	
	
	while (1)
		{//while------------------------------------------------------------------
		
			
			if (Fifo_Out_Line(&fifo_usart1,usart1_buf))
			{
				Usart3_Send(usart1_buf);
				Usart3_Send("\r\n");
			}
			
			if (Fifo_Out_Line(&fifo_usart2,usart2_buf))
			{
				//printf("%s\r\n",usart2_buf);
				if (WM_IsWindow(hWinNow))
				{
					Message.MsgId  = MSG_MEASURE_UPDATE;
					Message.Data.p = usart2_buf;
					WM_SendMessage(hWinNow, &Message);
				}
			}

			if (Fifo_Out_Line(&fifo_usart3,usart3_buf))
			{
				printf("%s\r\n",usart3_buf);
			}
    GUI_Delay(50);
		
		//����LED
		led_cnt++;
		if (led_cnt>10)
		{
			led_cnt=0;
			LED0=!LED0;
		}
			
  }//while------------------------------------------------------------------

  	  
}




void USART1_IRQHandler(void)                	//����1�жϷ������
{
   u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		Fifo_In_Byte(&fifo_usart1, Res);//���FIFO����	
    }
} 


void USART2_IRQHandler(void)                	//����2�жϷ������
{
   u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//��ȡ���յ�������
		
		Fifo_In_Byte(&fifo_usart2, Res);//���FIFO����	
    }
} 

//SIM900A���ڽ����ж�
void USART3_IRQHandler(void)                	//����3�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�
	{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//��ȡ���յ�������
		
		Fifo_In_Byte(&fifo_usart3, Res);//���FIFO����	
  }
}




