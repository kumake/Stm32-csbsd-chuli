/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "stm32f10x.h"
#include "sdio_sdcard.h"
#include "flash.h"


#define EX_FLASH 0	//FLASH,���Ϊ0
#define SD_CARD	 1  //SD��,���Ϊ1


#define BLOCK_SIZE            512 /* Block Size in Bytes */

//����W25Q64 
//ǰ6M�ֽڸ�fatfs��,6M�ֽں�~6M+500K���û���,6M+500K�Ժ�,���ڴ���ֿ�,�ֿ�ռ��1.5M.		 			    
u16	    FLASH_SECTOR_COUNT=2048*6;//6M�ֽ�,Ĭ��ΪW25Q64
//#define FLASH_BLOCK_SIZE  	8     //ÿ��BLOCK��8������


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	u8 res=0;	    
	switch(drv)
	{
		case SD_CARD://SD��
			res = SD_Init();//SD_Initialize() 
  			break;
		case EX_FLASH://�ⲿflash
			SPI_Flash_Init();
			if(SPI_FLASH_TYPE==W25Q64)FLASH_SECTOR_COUNT=2048*6;//W25Q64
			else FLASH_SECTOR_COUNT=2048*2;						//����
 			break;
		default:
			res=1; 
	}		 
	if(res)return  STA_NOINIT;
	else return 0; //��ʼ���ɹ�

}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{

	u8 res=0; 
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
	switch(drv)
	{
		case SD_CARD://SD��
			if (count > 1)
			{
				SD_ReadMultiBlocks(buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
			
					  /* Check if the Transfer is finished */
			     SD_WaitReadOperation();  //ѭ����ѯdma�����Ƿ����
			
			    /* Wait until end of DMA transfer */
			    while(SD_GetStatus() != SD_TRANSFER_OK);
		
			}
			else
			{
				
				SD_ReadBlock(buff, sector*BLOCK_SIZE, BLOCK_SIZE);
		
					  /* Check if the Transfer is finished */
			     SD_WaitReadOperation();  //ѭ����ѯdma�����Ƿ����
			
			    /* Wait until end of DMA transfer */
			    while(SD_GetStatus() != SD_TRANSFER_OK);
		
			}
			break;
		case EX_FLASH://�ⲿflash
			for(;count>0;count--)
			{
				SPI_Flash_Read(buff,sector*BLOCK_SIZE,BLOCK_SIZE);
				sector++;
				buff+=BLOCK_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
   //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res==0x00)return RES_OK;	 
    else return RES_ERROR;

}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{

	u8 res=0;  
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
	switch(drv)
	{
		case SD_CARD://SD��
			if (count > 1)
			{
				SD_WriteMultiBlocks((uint8_t *)buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
				
				  /* Check if the Transfer is finished */
			  	 SD_WaitWriteOperation();	   //�ȴ�dma�������
			    while(SD_GetStatus() != SD_TRANSFER_OK); //�ȴ�sdio��sd���������
			}
			else
			{
				SD_WriteBlock((uint8_t *)buff,sector*BLOCK_SIZE, BLOCK_SIZE);
				
				  /* Check if the Transfer is finished */
			   		SD_WaitWriteOperation();	   //�ȴ�dma�������
			    while(SD_GetStatus() != SD_TRANSFER_OK); //�ȴ�sdio��sd���������
			}
			break;
		case EX_FLASH://�ⲿflash
			for(;count>0;count--)
			{										    
				SPI_Flash_Write((u8*)buff,sector*BLOCK_SIZE,BLOCK_SIZE);
				sector++;
				buff+=BLOCK_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
    //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res == 0x00)return RES_OK;	 
    else return RES_ERROR;	


}
#endif /* _READONLY */




/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	return RES_OK;
}
							 
/*-----------------------------------------------------------------------*/
/* Get current time                                                      */
/*-----------------------------------------------------------------------*/ 
DWORD get_fattime(void)
{

 	return 0;

} 
