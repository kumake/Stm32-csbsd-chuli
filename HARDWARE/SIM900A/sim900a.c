#include "sim900a.h"
#include "delay.h"
#include "led.h"	   

//��ʼ��PG6 PG7 PG8Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void SIM900A_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��

  //SIM900A_STATUS��������
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	//�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB5

 //SIM900A_PKEY�������
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB8
 GPIO_ResetBits(GPIOB,GPIO_Pin_8);

 //SIM900A_NRST��©���	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //��©���
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB9
 GPIO_SetBits(GPIOB,GPIO_Pin_9);						 //�����

 

 //����
 if (!SIM900A_STATUS)
 {
	SIM900A_PKEY=1;
	delay_ms(1500);
	SIM900A_PKEY=0;
 }

 //���ڳ�ʼ��
 Usart3_Init(9600);

 //ͬ������
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
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
    USART3->DR = ch;      
	return ch;
}

 	  


//bound:������
void Usart3_Init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3ʱ��

 	USART_DeInit(USART3);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PA2
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PA3


  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

	//Usart3 NVIC ����

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

}


