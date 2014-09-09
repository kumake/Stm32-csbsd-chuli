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

FIFO fifo_usart1,fifo_usart2,fifo_usart3;  //串口123的FIFO缓冲


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


	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级


	//初始化内存
	FSMC_SRAM_Init();		//初始化外部SRAM
	delay_ms(500);	//等待外部SRAM就绪
//	mem_init(SRAMIN);		//初始化内部内存池
	mem_init(SRAMEX);		//初始化外部内存池

	//初始化串口FIFO缓冲
	Fifo_Init(&fifo_usart1,1024*1);
	Fifo_Init(&fifo_usart2,1024*1);
	Fifo_Init(&fifo_usart3,1024*1);

	uart_init(9600);	 	//串口1初始化为9600

	 //串口2初始化
 	Usart2_Init(9600);

	//初始化LED
 	LED_Init();
	//RTC初始化
	RTC_Init();
	//蜂鸣器初始化
	BEEP_Init();
	//初始化按键
	KEY_Init();

	//初始化SIM900A
	SIM900A_Init();

	//初始化FLASH
	SPI_Flash_Init();


	//初始化液晶
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	GUI_Init();	//初始化LCD
	TP_Init();//触摸屏初始化

	TIM3_Int_Init(9,7199);//10Khz的计数频率，计数到10为1ms	Tout= ((9+1)*( 7199+1))/72=1000us=1ms。	   //GUI需要时钟节拍
			     

	//挂载
	f_mount(0,&Fatfs_flash);
	f_mount(1,&Fatfs_sd);

	delay_ms(500);	//等待外部SRAM就绪

	font_init();//跟新字体


	//RTC_Set(2014,7,31,9,55,0);//设置时间

	 MainTask();

	//Touch_MainTask();	//触摸校准

	//GUIDEMO_Main();  //GUI各种例程
	
	
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
		
		//心跳LED
		led_cnt++;
		if (led_cnt>10)
		{
			led_cnt=0;
			LED0=!LED0;
		}
			
  }//while------------------------------------------------------------------

  	  
}




void USART1_IRQHandler(void)                	//串口1中断服务程序
{
   u8 Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
		Fifo_In_Byte(&fifo_usart1, Res);//存进FIFO缓冲	
    }
} 


void USART2_IRQHandler(void)                	//串口2中断服务程序
{
   u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
		
		Fifo_In_Byte(&fifo_usart2, Res);//存进FIFO缓冲	
    }
} 

//SIM900A串口接收中断
void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
		
		Fifo_In_Byte(&fifo_usart3, Res);//存进FIFO缓冲	
  }
}




