/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"			/* FatFs lower layer API */
#include "bsp.h"			/* 底层驱动 */
#include "SPI_SD_driver.h"
#include "nand.h"
#include "ftl.h"
#include "app.h"
#include "usbh_bsp_msc.h"			/* 底层驱动 */

#define SD_CARD	 	0  			//SD卡,卷标为0
#define EX_NAND  	1			//外部nand flash,卷标为1
#define USB_DISK 	2			//U盘,卷标为2

#define SECTOR_SIZE		512

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	u8 state;
	
	switch(pdrv)
	{
		case SD_CARD:		//SD卡
			state=SD_Init();	//SD卡初始化 
  			break;
		case EX_NAND:		//外部NAND
			state=FTL_Init();	//NAND初始化
			//state=0;
 			break;
		case USB_DISK:		//U盘
			if(HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				 state =  0;           //初始化成功
			}
			else
				 return STA_NOINIT;  //其他错误：初始化失败
 			break;
		default:
			state=1; 
	}	

    if(state == STA_NODISK)
    {
        return STA_NODISK;
    }
    else if(state != 0)
    {
        return STA_NOINIT;  //其他错误：初始化失败
    }
    else
    {
        return 0;           //初始化成功
    }
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS status = STA_NOINIT;
	switch(pdrv)
	{
		case 0://SD卡
			status &= ~STA_NOINIT;
  		break;
		case 1://nand
			status &= ~STA_NOINIT;
 			break;
		case USB_DISK:		//U盘
			status &= ~STA_NOINIT;
 			break;
		default:
			status = STA_NOINIT;
			break;
	}
	return status;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..128) */
)
{
	u8 res=0;
	if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误	
	switch(pdrv)
	{
		case SD_CARD://SD卡
		{
			if(count==1)            //1个sector的读操作      
			{                                                
				res = SD_ReadSingleBlock(sector, buff);      
			}                                                
			else                    //多个sector的读操作     
			{                                                
				res = SD_ReadMultiBlock(sector, buff, count);
			}
		}
		break;
		case EX_NAND:
		{
			res=FTL_ReadSectors(buff,sector,512,count);	//读取数据	
			//res=NAND_ReadPage(sector,0,buff,count);
		}
		break;
		case USB_DISK:
		{
			
			res = USBH_Disk_Read(buff,sector,count);
		}
		break;	
	}
//    if (pdrv || !count)
//    {    
//        return RES_PARERR;  //仅支持单磁盘操作，count不能等于0，否则返回参数错误
//    }
//    if(!SD_DET())
//    {
//        return RES_NOTRDY;  //没有检测到SD卡，报NOT READY错误
//    }
                                             
	/*
    do                           
    {                                          
        if(SD_ReadSingleBlock(sector, buff)!=0)
        {                                      
            res = 1;                           
            break;                             
        }                                      
        buff+=512;                             
    }while(--count);                                         
    */
    //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res == 0x00)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{
	u8 res;
	switch(pdrv)
	{
		case SD_CARD:
		{
			// 读写操作
			if(count == 1)
			{
				res = SD_WriteSingleBlock(sector, buff);
			}
			else
			{
				res = SD_WriteMultiBlock(sector, buff, count);
			}
		}
		break;
		case EX_NAND:		//外部NAND
			res=FTL_WriteSectors((u8*)buff,sector,512,count);//写入数据
			break;
		case USB_DISK:
		{
			
			res = USBH_Disk_Write((u8*)buff,sector,count);
			
		}
		break;
	}
    
    // 返回值转换
    if(res == 0)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;

	if (pdrv==SD_CARD) 
		{
			//if (drv) return RES_PARERR;
			res = RES_ERROR;

			//if (Stat & STA_NOINIT) return RES_NOTRDY;
			switch (cmd)
			{
				case CTRL_SYNC :		/* Make sure that no pending write process */
					res = RES_OK;
					break;

				case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
					*(DWORD*)buff = (DWORD)  SD_GetCapacity();
					res = RES_OK;
					break;

				case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
					*(WORD*)buff = 512;
					res = RES_OK;
					break;

				case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */\
					*(DWORD*)buff = 512;
					res = RES_OK;
					break;

				default:
					res = RES_PARERR;
					break;
			}
			return res;
		}
		else if(pdrv==EX_NAND)
		{
			 switch(cmd)
			{
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = 512;	//NAND FLASH扇区强制为512字节大小
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = nand_dev.page_mainsize/512;//block大小,定义成一个page的大小
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = nand_dev.valid_blocknum*nand_dev.block_pagenum*nand_dev.page_mainsize/512;//NAND FLASH的总扇区大小
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
			}
		}
		else if(pdrv==USB_DISK)
		{
			res = RES_ERROR;

			//if (Stat & STA_NOINIT) return RES_NOTRDY;
			switch (cmd)
			{
				case CTRL_SYNC :		/* Make sure that no pending write process */
					res = RES_OK;
					break;

				case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
					*(DWORD*)buff = (DWORD) USBH_MSC_Param.MSCapacity;
					res = RES_OK;
					break;

				case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
					*(WORD*)buff = 512;
					res = RES_OK;
					break;

				case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */\
					*(DWORD*)buff = 512;
					res = RES_OK;
					break;

				default:
					res = RES_PARERR;
					break;
			}
			return res;
		}
		else
		{
			 res = RES_PARERR;
		}

	
	return res;
}
#endif

/*
*********************************************************************************************************
*	函 数 名: get_fattime
*	功能说明: 获得系统时间，用于改写文件的创建和修改时间。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
DWORD get_fattime (void)
{
	/* 如果有全局时钟，可按下面的格式进行时钟转换. 这个例子是2013-01-01 00:00:00 */

//	return	  ((DWORD)(2013 - 1980) << 25)	/* Year = 2013 */
//			| ((DWORD)1 << 21)				/* Month = 1 */
//			| ((DWORD)1 << 16)				/* Day_m = 1*/
//			| ((DWORD)0 << 11)				/* Hour = 0 */
//			| ((DWORD)0 << 5)				/* Min = 0 */
//			| ((DWORD)0 >> 1);				/* Sec = 0 */
	return	  ((DWORD)(model4001.time[TIME_MONTH]+20) << 25)	/* Year*/
			| ((DWORD)model4001.time[TIME_MONTH] << 21)				/* Month*/
			| ((DWORD)model4001.time[TIME_DAY] << 16)				/* Day_m*/
			| ((DWORD)model4001.time[TIME_HOUR] << 11)				/* Hour*/
			| ((DWORD)model4001.time[TIME_MIN] << 5)				/* Min*/
			| ((DWORD)model4001.time[TIME_SEC] >> 1);				/* Sec*/
}
