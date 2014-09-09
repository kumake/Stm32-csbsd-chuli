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


#define EX_FLASH 0	//FLASH,卷标为0
#define SD_CARD	 1  //SD卡,卷标为1


#define BLOCK_SIZE            512 /* Block Size in Bytes */

//对于W25Q64 
//前6M字节给fatfs用,6M字节后~6M+500K给用户用,6M+500K以后,用于存放字库,字库占用1.5M.		 			    
u16	    FLASH_SECTOR_COUNT=2048*6;//6M字节,默认为W25Q64
//#define FLASH_BLOCK_SIZE  	8     //每个BLOCK有8个扇区


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	u8 res=0;	    
	switch(drv)
	{
		case SD_CARD://SD卡
			res = SD_Init();//SD_Initialize() 
  			break;
		case EX_FLASH://外部flash
			SPI_Flash_Init();
			if(SPI_FLASH_TYPE==W25Q64)FLASH_SECTOR_COUNT=2048*6;//W25Q64
			else FLASH_SECTOR_COUNT=2048*2;						//其他
 			break;
		default:
			res=1; 
	}		 
	if(res)return  STA_NOINIT;
	else return 0; //初始化成功

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
    if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
	switch(drv)
	{
		case SD_CARD://SD卡
			if (count > 1)
			{
				SD_ReadMultiBlocks(buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
			
					  /* Check if the Transfer is finished */
			     SD_WaitReadOperation();  //循环查询dma传输是否结束
			
			    /* Wait until end of DMA transfer */
			    while(SD_GetStatus() != SD_TRANSFER_OK);
		
			}
			else
			{
				
				SD_ReadBlock(buff, sector*BLOCK_SIZE, BLOCK_SIZE);
		
					  /* Check if the Transfer is finished */
			     SD_WaitReadOperation();  //循环查询dma传输是否结束
			
			    /* Wait until end of DMA transfer */
			    while(SD_GetStatus() != SD_TRANSFER_OK);
		
			}
			break;
		case EX_FLASH://外部flash
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
   //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
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
    if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
	switch(drv)
	{
		case SD_CARD://SD卡
			if (count > 1)
			{
				SD_WriteMultiBlocks((uint8_t *)buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
				
				  /* Check if the Transfer is finished */
			  	 SD_WaitWriteOperation();	   //等待dma传输结束
			    while(SD_GetStatus() != SD_TRANSFER_OK); //等待sdio到sd卡传输结束
			}
			else
			{
				SD_WriteBlock((uint8_t *)buff,sector*BLOCK_SIZE, BLOCK_SIZE);
				
				  /* Check if the Transfer is finished */
			   		SD_WaitWriteOperation();	   //等待dma传输结束
			    while(SD_GetStatus() != SD_TRANSFER_OK); //等待sdio到sd卡传输结束
			}
			break;
		case EX_FLASH://外部flash
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
    //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
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
