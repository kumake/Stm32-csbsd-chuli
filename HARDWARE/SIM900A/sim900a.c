#include "sim900a.h"
#include "delay.h"
#include "led.h"	   

//初始化PG6 PG7 PG8为输出口.并使能这两个口的时钟		    
//LED IO初始化
void SIM900A_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟

  //SIM900A_STATUS浮空输入
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	//端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //浮空输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB5

 //SIM900A_PKEY推挽输出
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB8
 GPIO_ResetBits(GPIOB,GPIO_Pin_8);

 //SIM900A_NRST开漏输出	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //开漏输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB9
 GPIO_SetBits(GPIOB,GPIO_Pin_9);						 //输出低

 

 //开机
 if (!SIM900A_STATUS)
 {
	SIM900A_PKEY=1;
	delay_ms(1500);
	SIM900A_PKEY=0;
 }

 //串口初始化
 Usart3_Init(9600);

 //同步串口
 Usart3_Send("AT\r\n");
 Usart3_Send("AT\r\n");
 delay_ms(500);

}


u8* Usart3_Send(u8* pstring)
{
	u16 i=0;
	      
	while(*(pstring+i))
	{
		Usart3_Putc(*(pstring+i));
		i++;
	}
	return 	pstring;
}

u8 Usart3_Putc(u8 ch)
{      
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
    USART3->DR = ch;      
	return ch;
}

 	  


//bound:波特率
void Usart3_Init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟

 	USART_DeInit(USART3);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PA2
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PA3


  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART3, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口 

	//Usart3 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

}


