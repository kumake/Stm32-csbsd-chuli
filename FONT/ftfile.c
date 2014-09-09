#include "ftfile.h"
#include "ff.h"	  
#include "flash.h"
#include "malloc.h"
#include "GUI.h"

						   
u32 FONTINFOADDR=(1024*6+500)*1024;//Ĭ����6M�ĵ�ַ
//�ֿ���Ϣ�ṹ��. 
//���������ֿ������Ϣ����ַ����С��
_font_info ftinfo;

//��sd���е�·��
const u8 *UNIGBK_SDPATH="1:/SYSTEM/FONT/UNIGBK.BIN";	//UNIGBK.BIN�Ĵ��λ��
const u8 *GBK12_SDPATH="1:/SYSTEM/FONT/GBK12.FON";		//GBK12�Ĵ��λ��
const u8 *GBK16_SDPATH="1:/SYSTEM/FONT/GBK16.FON";		//GBK16�Ĵ��λ��

//��ʾ��ǰ������½���
//x,y:����
//size:�����С
//fsize:�����ļ���С
//pos:��ǰ�ļ�ָ��λ��
/*
u32 fupd_prog(u16 x,u16 y,u8 size,u32 fsize,u32 pos)
{
	float prog;
	u8 t=0XFF;
	prog=(float)pos/fsize;
	prog*=100;
	if(t!=prog)
	{
		LCD_ShowString(x+3*size/2,y,240,320,size,"%");		
		t=prog;
		if(t>100)t=100;
		LCD_ShowNum(x,y,t,3,size);//��ʾ��ֵ
	}
	return 0;					    
}
  */

//����ĳһ��
//x,y:����
//size:�����С
//fxpath:·��
//fx:���µ����� 0,ungbk;1,gbk12;2,gbk16;
//����ֵ:0,�ɹ�;����,ʧ��.
u8 updata_fontx(u8 *fxpath,u8 fx)
{	

	u32 flashaddr=0;								    
	FIL F;
	u8 *tempbuf;
 	u8 res;	
	u16 bread;
	u32 offx=0;
	u8 rval=0;	     


	tempbuf=mymalloc(SRAMEX,4096);	//����4096���ֽڿռ�
	if(tempbuf==NULL)rval=1;

 	res=f_open(&F,(const TCHAR*)fxpath,FA_READ); 
 	if(res)rval=2;//���ļ�ʧ��
	  
 	if(rval==0)	 
	{
		if(fx==0)		//����UNIGBK.BIN
		{
			ftinfo.ugbkaddr=FONTINFOADDR+sizeof(ftinfo);//��Ϣͷ֮�󣬽���UNIGBKת�����
  			ftinfo.ugbksize=F.fsize;				//UNIGBK��С
 			flashaddr=ftinfo.ugbkaddr;
		}else if(fx==1)	//GBK12
		{				  
			ftinfo.f12addr=ftinfo.ugbkaddr+ftinfo.ugbksize;		//UNIGBK֮�󣬽���GBK12�ֿ�
			ftinfo.gbk12size=F.fsize;						//GBK12�ֿ��С
			flashaddr=ftinfo.f12addr;							//GBK12����ʼ��ַ
		}else			//GBK16
		{
			ftinfo.f16addr=ftinfo.f12addr+ftinfo.gbk12size;		//GBK12֮�󣬽���GBK16�ֿ�
			ftinfo.gkb16size=F.fsize;						//GBK16�ֿ��С
			flashaddr=ftinfo.f16addr;							//GBK16����ʼ��ַ
		}	   
		while(res==FR_OK)//��ѭ��ִ��
		{
	 		res=f_read(&F,tempbuf,4096,(UINT *)&bread);		//��ȡ����	 
			if(res!=FR_OK)break;								//ִ�д���
			SPI_Flash_Write(tempbuf,offx+flashaddr,4096);		//��0��ʼд��4096������  
	  		offx+=bread;	  
			//fupd_prog(x,y,size,fftemp->fsize,offx);	 			//������ʾ
			//GUI_DispDecAT((I32)offx, 150, 150, 10);
			//GUI_DispDecAT((I32)F.fsize, 200, 200, 10);
			if(bread!=4096)break;								//������.
	 	} 	
		f_close(&F);		
	}			 

	myfree(SRAMEX,tempbuf);	//�ͷ��ڴ�
	return res;
}

//���������ļ�,UNIGBK,GBK12,GBK16һ�����
//x,y:��ʾ��Ϣ����ʾ��ַ
//size:�����С
//��ʾ��Ϣ�����С
//src:0,��SD������.
//	  1,��25QXX����											  
//����ֵ:0,���³ɹ�;
//		 ����,�������.	  
u8 update_font(void)
{	
	u8 *gbk16_path;
	u8 *gbk12_path;
	u8 *unigbk_path;
	u8 res;		  

	unigbk_path=(u8*)UNIGBK_SDPATH;
	gbk12_path=(u8*)GBK12_SDPATH;
	gbk16_path=(u8*)GBK16_SDPATH;
	   
 	res=0XFF;		
	ftinfo.fontok=0XFF;
  	SPI_Flash_Write((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//���֮ǰ�ֿ�ɹ��ı�־.��ֹ���µ�һ������,���µ��ֿⲿ�����ݶ�ʧ.
 	SPI_Flash_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//���¶���ftinfo�ṹ������
 	GUI_DispString("updating UNIGBK.BIN .....");		
	res=updata_fontx(unigbk_path,0);			//����UNIGBK.BIN
	if(res)
	{
		GUI_DispString("ERROR!\r\n");
		return 1;
	}
	GUI_DispString("OK!\r\n");

	GUI_DispString("updating GBK12.BIN .....");
	res=updata_fontx(gbk12_path,1);			//����GBK12.FON
	if(res)
	{
		GUI_DispString("ERROR!\r\n");
		return 2;
	}
	GUI_DispString("OK!\r\n");	


	//LCD_ShowString(x,y,240,320,size,"Updating GBK16.BIN  ");
	GUI_DispString("updating GBK16.BIN .....");
	res=updata_fontx(gbk16_path,2);			//����GBK16.FON
	
	if(res)
	{
		GUI_DispString("ERROR!\r\n");
		return 3;
	}
	GUI_DispString("OK!\r\n");
		   
	//ȫ�����º���
	ftinfo.fontok=0XAA;
  	SPI_Flash_Write((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));	//�����ֿ���Ϣ
	return 0;//�޴���.		 
} 
//��ʼ������
//����ֵ:0,�ֿ����.
//		 ����,�ֿⶪʧ
u8 font_init(void)
{			  												 
	//SPI_Flash_Init();
	FONTINFOADDR=(1024*6+500)*1024;			//W25Q64,6M�Ժ�	 
	//ftinfo.ugbkaddr=FONTINFOADDR+25;		//UNICODEGBK �����׵�ַ�̶���ַ
	SPI_Flash_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//����ftinfo�ṹ������
	if(ftinfo.fontok!=0XAA)		  //��Ҫ�����ֿ�
	{
		 GUI_DispString("no ftfile ,need load .....\r\n");
		 if (update_font())
		 {
		 	  GUI_DispString("load ftfile error,please check!.....\r\n");
		 	  while(1);//���³����˴���
		 }
	}
	return 0;		    
}





























