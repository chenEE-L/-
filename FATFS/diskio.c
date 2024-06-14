/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"			/* FatFs lower layer API */
#include "bsp.h"			/* �ײ����� */
#include "SPI_SD_driver.h"
#include "nand.h"
#include "ftl.h"
#include "app.h"
#include "usbh_bsp_msc.h"			/* �ײ����� */

#define SD_CARD	 	0  			//SD��,���Ϊ0
#define EX_NAND  	1			//�ⲿnand flash,���Ϊ1
#define USB_DISK 	2			//U��,���Ϊ2

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
		case SD_CARD:		//SD��
			state=SD_Init();	//SD����ʼ�� 
  			break;
		case EX_NAND:		//�ⲿNAND
			state=FTL_Init();	//NAND��ʼ��
			//state=0;
 			break;
		case USB_DISK:		//U��
			if(HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				 state =  0;           //��ʼ���ɹ�
			}
			else
				 return STA_NOINIT;  //�������󣺳�ʼ��ʧ��
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
        return STA_NOINIT;  //�������󣺳�ʼ��ʧ��
    }
    else
    {
        return 0;           //��ʼ���ɹ�
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
		case 0://SD��
			status &= ~STA_NOINIT;
  		break;
		case 1://nand
			status &= ~STA_NOINIT;
 			break;
		case USB_DISK:		//U��
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
	if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������	
	switch(pdrv)
	{
		case SD_CARD://SD��
		{
			if(count==1)            //1��sector�Ķ�����      
			{                                                
				res = SD_ReadSingleBlock(sector, buff);      
			}                                                
			else                    //���sector�Ķ�����     
			{                                                
				res = SD_ReadMultiBlock(sector, buff, count);
			}
		}
		break;
		case EX_NAND:
		{
			res=FTL_ReadSectors(buff,sector,512,count);	//��ȡ����	
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
//        return RES_PARERR;  //��֧�ֵ����̲�����count���ܵ���0�����򷵻ز�������
//    }
//    if(!SD_DET())
//    {
//        return RES_NOTRDY;  //û�м�⵽SD������NOT READY����
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
    //������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
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
			// ��д����
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
		case EX_NAND:		//�ⲿNAND
			res=FTL_WriteSectors((u8*)buff,sector,512,count);//д������
			break;
		case USB_DISK:
		{
			
			res = USBH_Disk_Write((u8*)buff,sector,count);
			
		}
		break;
	}
    
    // ����ֵת��
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
		        *(WORD*)buff = 512;	//NAND FLASH����ǿ��Ϊ512�ֽڴ�С
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = nand_dev.page_mainsize/512;//block��С,�����һ��page�Ĵ�С
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = nand_dev.valid_blocknum*nand_dev.block_pagenum*nand_dev.page_mainsize/512;//NAND FLASH����������С
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
*	�� �� ��: get_fattime
*	����˵��: ���ϵͳʱ�䣬���ڸ�д�ļ��Ĵ������޸�ʱ�䡣
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
DWORD get_fattime (void)
{
	/* �����ȫ��ʱ�ӣ��ɰ�����ĸ�ʽ����ʱ��ת��. ���������2013-01-01 00:00:00 */

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
