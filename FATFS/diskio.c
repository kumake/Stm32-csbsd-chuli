/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "stm32f10x.h"
#include <stdio.h>
#include "diskio.h"
#include "sdio_sdcard.h"
#include "flash.h"



#define EX_FLASH 0	//FLASH,卷标为0
#define SD_CARD	 1  //SD卡,卷标为1

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
/*-----------------------------------------------------------------------*/

#define FS_PRINTF_EN	0		/* 1表示打印调试信息 */

#if FS_PRINTF_EN == 1
	#define fs_printf(...)	printf(__VA_ARGS__)
#else
	#define fs_printf(...)
#endif	


#define SECTOR_SIZE		512U
//对于W25Q64 
//前6M字节给fatfs用,6M字节后~6M+500K给用户用,6M+500K以后,用于存放字库,字库占用1.5M.		 			    
u16	    FLASH_SECTOR_COUNT=2048*6;//6M字节,默认为W25Q64





uint32_t Mass_Block_Size,Mass_Block_Count;

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	SD_CardInfo mSDCardInfo;	/* 定义SD卡状态信息结构体 */
	uint32_t DeviceSizeMul = 0, NumberOfBlocks = 0;	
	uint16_t Status;

	switch(drv)
	{
	 case SD_CARD://SD卡
	//SD卡初始化--------------------------------------------------------------------------
				Status = SD_Init();
				if (Status != SD_OK)	
				{
					fs_printf("SD_Init() fail (%d) : file %s on line %d\r\n", Status, __FILE__, __LINE__);
					goto retfail;
				}
			
				SD_GetCardInfo(&mSDCardInfo);	/* 读取SD卡的信息 */
				SD_SelectDeselect((uint32_t) (mSDCardInfo.RCA << 16));
				DeviceSizeMul = (mSDCardInfo.SD_csd.DeviceSizeMul + 2);
				
				if (mSDCardInfo.CardType == SDIO_HIGH_CAPACITY_SD_CARD)	/* 高容量SD卡 SDHC */
				{
					Mass_Block_Count = (mSDCardInfo.SD_csd.DeviceSize + 1) * 1024;
				}
				else	/* 普通SD卡, 最大4G */
				{
					NumberOfBlocks  = ((1 << (mSDCardInfo.SD_csd.RdBlockLen)) / 512);
					Mass_Block_Count = ((mSDCardInfo.SD_csd.DeviceSize + 1) * (1 << DeviceSizeMul) << (NumberOfBlocks/2));
				}
				
				Status = SD_SelectDeselect((uint32_t) (mSDCardInfo.RCA << 16)); 
				Status = SD_EnableWideBusOperation(SDIO_BusWide_4b); 
				if (Status != SD_OK)
				{
					fs_printf("SD_EnableWideBusOperation(SDIO_BusWide_4b) Fail (%d)\r\n", Status);
					goto retfail;
				}
				
				Status = SD_SetDeviceMode(SD_DMA_MODE);    /* 设置SD卡工作模式为DMA, 其它模式由中断、轮询 */     
				if (Status != SD_OK)
				{
					fs_printf("SD_SetDeviceMode(SD_DMA_MODE) Fail (%d)\r\n", Status);				
					goto retfail;
				} 
			
				/* 显示容量 */
				Mass_Block_Size  = 512;
				if (mSDCardInfo.CardType == SDIO_HIGH_CAPACITY_SD_CARD)	/* 高容量SD卡 SDHC */
				{	
					fs_printf("SDHC Card, Memory Size = %uMB\r\n", Mass_Block_Count / (1024 * 4));
				}
				else
				{
					fs_printf("Normal Card, Memory Size = %uMB\r\n", (Mass_Block_Count * Mass_Block_Size) /(1024*1024));
				}

	//SD卡初始化结束--------------------------------------------------------------------------
				break;

	case EX_FLASH://外部flash
			SPI_Flash_Init();
			if(SPI_FLASH_TYPE==W25Q64)FLASH_SECTOR_COUNT=2048*6;//W25Q64
			else FLASH_SECTOR_COUNT=2048*2;						//其他
 			break;
	default:
			goto retfail;
	}
			
	return RES_OK;
retfail:	
	return RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	SD_Error Status = SD_OK;


	switch(drv)
	{
		case SD_CARD://SD卡
	//SD卡开始----------------------------------------
			if (count == 1) 
			{
				Status = SD_ReadBlock(buff, sector << 9 , SECTOR_SIZE);
			} 
			else 
			{
				Status = SD_ReadMultiBlocks(buff, sector << 9 , SECTOR_SIZE, count);
			}
		

	//SD卡结束----------------------------------------
			break;
	case EX_FLASH://外部flash
			for(;count>0;count--)
			{
				SPI_Flash_Read(buff,sector*SECTOR_SIZE,SECTOR_SIZE);
				sector++;
				buff+=SECTOR_SIZE;
			}
			Status=SD_OK;
			break;
	default:
			Status=SD_ERROR;
	}

			
			
			if (Status == SD_OK) 
			{
				return RES_OK;
			} 
			else
			{
				printf("Err: SD_ReadMultiBlocks(,%d,%d)\r\n",sector,count);
				return RES_ERROR;
			}
			
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
/* The FatFs module will issue multiple sector transfer request
/  (count > 1) to the disk I/O layer. The disk function should process
/  the multiple sector transfer properly Do. not translate it into
/  multiple single sector transfers to the media, or the data read/write
/  performance may be drasticaly decreased. */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	SD_Error Status = SD_OK;
	
#if 1

	switch(drv)
	{
		case SD_CARD://SD卡
	//SD卡开始----------------------------------------
			while (count--)
			{
				Status = SD_WriteBlock((uint8_t *)buff, sector << 9 ,SECTOR_SIZE);
				if (Status != SD_OK)
				{
					break;
				}
			}
	//SD卡结束----------------------------------------
			break;
	 case EX_FLASH://外部flash
			for(;count>0;count--)
			{										    
				SPI_Flash_Write((u8*)buff,sector*SECTOR_SIZE,SECTOR_SIZE);
				sector++;
				buff+=SECTOR_SIZE;
			}
			Status=SD_OK;
			break;
		default:
			Status=SD_ERROR; 
	}
#else		/* SD_WriteMultiBlocks() 偶尔会执行出错 */	
	if (count == 1) 
	{
		Status = SD_WriteBlock((uint8_t *)buff, sector << 9 ,SECTOR_SIZE);
	} 
	else 
	{
		Status = SD_WriteMultiBlocks((uint8_t *)buff, sector << 9 ,SECTOR_SIZE, count);	
	}
#endif	

	if (Status == SD_OK) 
	{
		return RES_OK;
	} 
	else
	{
		printf("Err: SD_WriteBlocks(,%d,%d)\r\n",sector,count);
		return RES_ERROR;
	}
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	return RES_OK;
}

