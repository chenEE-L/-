#include "app.h"
#include "fatfs.h"
#include "SPI_SD_driver.h"
#include "ff.h"
#include "bsp.h"
#include "ftl.h"
#include "usbh_bsp_msc.h"
#include<stdlib.h>

uint8_t g_TestBuf[BUF_SIZE];

FATFS fs[3]; //FatFs文件系统对象
FIL fnew[3]; //文件对象
FRESULT res_sd;//文件操作结果
UINT fnum; //文件成功读写数量
UINT fnum_read,fnum_write; //文件成功读写数量
TCHAR  ReadBuffer[1024] = {0};
BYTE WriteBuffer[] = "时间,液体流量,液体浓度,标况气体流量,工况气体流量,标况发生浓度,工况发生浓度,电磁阀状态,雾化器状态,注射泵状态,粒子出口温度,粒子出口湿度,粒子出口压力,斜率系数\n"; //写缓存区
TCHAR ReadBuffer2[100] = {'\0'}; //du缓存区2

BYTE data_name[] = {"0:201901.csv"};
BYTE log_time[]={"2000年00月00日00时00分    "};
BYTE test_name1[] = {"0:201901.csv"};
uint8_t cmd='0';
u8 CID[20],CSD[20];
u32 Capacity=0;
//u32 sive_count=0;
void fatfs_test(void *p_arg)
{
	(void)p_arg;
	unsigned short time_count=0;
	SPI_Configuration();
	SD_IO_Configuration();
	//FSMC_NANDDeInit(FSMC_Bank2_NAND);
	//FTL_Init();
	SD_PWR_ON() ;
	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_cb);  
	OSTimeDlyHMSM(0,0,0,100);
	char float_data[8];
	if(SD_Init() == 0)
	{
		printf("SD卡初始化成功，即将挂载SD卡。\r\n");
	}
	
	
	res_sd = f_mount(0,&fs[0]); //挂载SD卡
	//***********************格式化测试****************************
	if(res_sd == FR_NO_FILESYSTEM)
	{
		printf("SD卡没有文件系统，即将进行格式化...\r\n");
		//格式化
		res_sd=f_mkfs(0,1,0);//格式化SD卡,2,盘符;1,不需要引导区
		
		if(res_sd == FR_OK)
		{
			printf("SD卡成功格式化！\r\n");
			//格式化后先取消挂载
			res_sd =  f_mount(1,NULL); 
			//再重新挂载
			res_sd = f_mount(1,&fs[0]); 
		}
		else
		{
			printf("文件格式化失败！错误代码：%d\r\n",res_sd);
			//while(1);
		}
	}
	else if(res_sd != FR_OK)
	{
		printf("SD卡挂载文件系统失败！可能是因为文件初始化失败！错误代码：%d\r\n", res_sd);
	}
	else
	{
		printf("SD卡文件系统挂载成功！\r\n");
	}
	res_sd = f_mount(2,&fs[2]); //挂载U盘
	//***********************格式化测试****************************
	if(res_sd != FR_OK)
	{
		printf("U盘挂载文件系统失败！可能是因为文件初始化失败！错误代码：%d\r\n", res_sd);
	}
	else
	{
		printf("U盘文件系统挂载成功！\r\n");
	}
	
	fnew[0].fs = &fs[0];
	res_sd = f_open(&fnew[0], "0:config.txt", FA_OPEN_EXISTING | FA_READ);
		if(res_sd == FR_OK)
		{
			//printf("打开文件成功！开始写入数据！\r\n");
			res_sd= f_read(&fnew[0], (TCHAR*)&model_1006, sizeof(model_1006), &fnum);
			 //f_gets((TCHAR*)&model4001,sizeof(model4001),&fnew[1]);
			//关闭文件
			if(res_sd == FR_OK)
			{
				Dugs_send_config();
			}
			f_close(&fnew[0]);
		}
	OSTimeDlyHMSM(0,0,1,100);

	while (1)
	{
		OSTimeDlyHMSM(0,0,0,100);
		USBH_Process(&USB_OTG_Core, &USB_Host);
		time_count++;
		if(time_count%10==0)//保存数据求和
		{
			model_1006.file.fluid_flow_status += model_1006.status[FLUID_FLOW_STATUS];
			model_1006.file.fluid_concentration += model_1006.start_product[FLUID_CONCENTRATION];
			model_1006.file.sc_air_flow_rate += model_1006.status[SC_AIR_FLOW_RATE];
			model_1006.file.wc_air_flow_rate += model_1006.status[WC_AIR_FLOW_RATE];
			model_1006.file.sc_start_concentration += model_1006.status[SC_START_CONCENTRATION];
			model_1006.file.wc_start_concentration += model_1006.status[WC_START_CONCENTRATION];
			model_1006.file.radiotube_status += model_1006.status[RADIOTUBE_STATUS];
			model_1006.file.atomizer_switch += model_1006.status[ATOMIZER_SWITCH];
			model_1006.file.air_temp += model_1006.status[MODEL_1006_TEMP];
			model_1006.file.air_rh += model_1006.status[MODEL_1006_RH];
			model_1006.file.air_pressure += model_1006.status[AIR_PRESSURE];
			model_1006.file.slope_coefficients += 1000;
		}
		if(time_count>=300)//1min保存一组数据 100ms*600=1min
		{
			time_count = 0;
			#if 1
			data_name[4] = model_1006.time[TIME_YEAR]/10+0x30;
			data_name[5] = model_1006.time[TIME_YEAR]%10+0x30;
			data_name[6] = model_1006.time[TIME_MONTH]/10+0x30;
			data_name[7] = model_1006.time[TIME_MONTH]%10+0x30;
			//data_name[8] = model4001.time[TIME_DAY]/10+0x30;
			//data_name[9] = model4001.time[TIME_DAY]%10+0x30;
			
			res_sd = f_open(&fnew[0], (const char*)data_name, FA_OPEN_EXISTING | FA_WRITE);
			if(res_sd == FR_OK)
			{
				//printf("打开文件成功！开始写入数据！\r\n");
				res_sd=f_lseek(&fnew[0],f_size(&fnew[0]));//查找文件的结尾
				f_printf(&fnew[0],"20%d/%d/%d %d:%d:%d,",model_1006.time[TIME_YEAR],model_1006.time[TIME_MONTH],model_1006.time[TIME_DAY],model_1006.time[TIME_HOUR],model_1006.time[TIME_MIN],model_1006.time[TIME_SEC]);
				sprintf(float_data,"%0.2f",model_1006.file.fluid_flow_status/30000.0f);
				if(f_printf(&fnew[0],"%s,",float_data)==EOF)
				{
					f_close(&fnew[0]);
					res_sd =  f_mount(0,NULL);
					continue;
				}
				sprintf(float_data,"%0.2f",model_1006.file.fluid_concentration/30000.0f);
				f_printf(&fnew[0],"%s,",float_data);
				sprintf(float_data,"%0.3f",model_1006.file.sc_air_flow_rate/3000.0f);
				f_printf(&fnew[0],"%s,",float_data);
				
				sprintf(float_data,"%0.3f",model_1006.file.wc_air_flow_rate/3000.0f);
				f_printf(&fnew[0],"%s,",float_data);

				sprintf(float_data,"%0.3f",model_1006.file.sc_start_concentration/300.0f);
				f_printf(&fnew[0],"%s,",float_data);
				
				sprintf(float_data,"%0.3f",model_1006.file.wc_start_concentration/300.0f);
				f_printf(&fnew[0],"%s,",float_data);
				
				sprintf(float_data,"%0.3f",model_1006.file.radiotube_status/30.0f);
				f_printf(&fnew[0],"%s,",float_data);
				
				sprintf(float_data,"%0.1f",model_1006.file.atomizer_switch/30.0f);
				f_printf(&fnew[0],"%s,",float_data);
				
				//sprintf(float_data,"%d",model_1006.start_product[INJECTION_PUMP_STATUS]);
				//f_printf(&fnew[0],"%s,",float_data);
				if(model_1006.start_product[INJECTION_PUMP_STATUS]==0)
					f_printf(&fnew[0],"%s,","停止");
				else if(model_1006.start_product[INJECTION_PUMP_STATUS]==1)
					f_printf(&fnew[0],"%s,","吸液");
				else
					f_printf(&fnew[0],"%s,","排液");
				
				sprintf(float_data,"%0.1f",model_1006.file.air_temp/3000.0f);
				f_printf(&fnew[0],"%s,",float_data);
				
				sprintf(float_data,"%0.1f",model_1006.file.air_rh/3000.0f);
				f_printf(&fnew[0],"%s,",float_data);
				
				sprintf(float_data,"%0.1f",model_1006.file.air_pressure/3000.0f);
				
				f_printf(&fnew[0],"%s,",float_data);
				
				sprintf(float_data,"%0.3f",model_1006.file.slope_coefficients/30000.0f);
				f_printf(&fnew[0],"%s,\n",float_data);//最后一组数据发送换行符
				if(res_sd == FR_OK)
				{
					//sive_count++;
					//printf("数据写入成功！\r\n");
					//printf("数据：%s。共写入%d个字符\r\n", WriteBuffer, fnum);
				}
			}
			else if(res_sd == FR_NO_FILE)
			{
				f_close(&fnew[0]);
				res_sd = f_open(&fnew[0], (const char*)data_name, FA_CREATE_ALWAYS | FA_WRITE);
				if(res_sd == FR_OK)
				{
					res_sd= f_write(&fnew[0], WriteBuffer, sizeof(WriteBuffer), &fnum);
				}
				else
				{
					//创建文件失败处理失败信息
				}
			}
			else //if(res_sd == FR_DISK_ERR)
			{
				f_close(&fnew[0]);
				res_sd =  f_mount(0,NULL); 
			//再重新挂载
				SD_Init();
				OSTimeDlyHMSM(0,0,0,100);
				res_sd = f_mount(0,&fs[0]); 
				
				//printf("数据写入失败！\r\n");
			}
			//关闭文件
			f_close(&fnew[0]);
			//数据清理
			model_1006.file.fluid_flow_status = 0;
			model_1006.file.fluid_concentration = 0;
			model_1006.file.sc_air_flow_rate = 0;
			model_1006.file.wc_air_flow_rate = 0;
			model_1006.file.sc_start_concentration = 0;
			model_1006.file.wc_start_concentration = 0;
			model_1006.file.radiotube_status = 0;
			model_1006.file.atomizer_switch = 0;
			model_1006.file.air_temp = 0;
			model_1006.file.air_rh = 0;
			model_1006.file.air_pressure = 0;
			model_1006.file.slope_coefficients = 0;
			#endif
		}
		
		#if 1//保存数据
		if(model_1006.new_data)
		{
			res_sd = f_open(&fnew[0], "0:config.txt", FA_OPEN_EXISTING | FA_WRITE);
			if(res_sd == FR_OK)//文件打开成功
			{
				//printf("打开文件成功！开始写入数据！\r\n");
				res_sd= f_write(&fnew[0], (TCHAR*)&model_1006, sizeof(model_1006), &fnum);
				model_1006.new_data = 0;
				if(res_sd == FR_OK)
				{
					printf("配置参数保存成功！\r\n");
				}
				else
				{
					printf("配置参数保存失败！\r\n");
				}
				
				//关闭文件
				f_close(&fnew[0]);
			}
			else if(res_sd == FR_NO_FILE)
			{
				res_sd = f_open(&fnew[0], "0:config.txt", FA_CREATE_ALWAYS | FA_WRITE);
				res_sd= f_write(&fnew[0], (TCHAR*)&model_1006, sizeof(model_1006), &fnum);
				model_1006.new_data = 0;
				if(res_sd == FR_OK)
				{
					printf("配置参数保存成功！\r\n");
				}
				else
				{
					res_sd =  f_mount(0,NULL); 
					//再重新挂载
					printf("配置参数保存失败！\r\n");
				}
				
				//关闭文件
				f_close(&fnew[0]);
			}
			else
			{
				FTL_Format();
				res_sd=f_mkfs(0,1,0);//格式化FLASH,2,盘符;1,不需要引导区,8个扇区为1个簇
				{
					res_sd =  f_mount(0,NULL); 
					//再重新挂载
					res_sd = f_mount(0,&fs[0]); 
				}

			}
		}
		if(model_1006.data_demand[DATA_EXPORT])
		{
			data_name[4] = model_1006.data_demand[TIME_YEAR]/10%10+0x30;
			data_name[5] = model_1006.data_demand[TIME_YEAR]%10+0x30;
			data_name[6] = model_1006.data_demand[TIME_MONTH]/10+0x30;
			data_name[7] = model_1006.data_demand[TIME_MONTH]%10+0x30;
			if(model_1006.data_demand[DATA_EXPORT]&&model_1006.data_demand[DATA_EXPORT_EN])
			{
				for(char i=0;i<100;i++)
				{
					USBH_Process(&USB_OTG_Core, &USB_Host);//猛查寻
					OSTimeDlyHMSM(0,0,0,1);
				}
//				data_name[0]='2';
//				res_sd = f_open(&fnew[2], (const char*)data_name, FA_CREATE_ALWAYS | FA_WRITE);//在U盘创建该文件
//				if(res_sd==FR_OK)
//				{
//					res_sd= f_write(&fnew[2], WriteBuffer,sizeof(WriteBuffer), &fnum);
//					printf("数据写入成功！\r\n");
//				}
//				else
//				{
//					printf("数据写入失败！\r\n");
//				}
//				data_name[0]='0';
				res_sd = f_open(&fnew[0], (const char*)data_name, FA_OPEN_EXISTING | FA_READ);//打开SD卡文件
				if(res_sd==FR_OK)
				{
					printf("文件打开成功！\r\n");
					data_name[0]='2';
					res_sd = f_open(&fnew[2], (const char*)data_name, FA_CREATE_ALWAYS | FA_WRITE);//在U盘创建该文件
					if(res_sd==FR_OK)
					{
						while(res_sd==FR_OK)
						{
							res_sd= f_read(&fnew[0], ReadBuffer, 1024, &fnum_read);
							if(res_sd||fnum_read==0)break;
							res_sd= f_write(&fnew[2], ReadBuffer, fnum_read, &fnum_write);
							if(res_sd||fnum_write<fnum_read)break;  
							model_1006.data_demand[DATA_EXPORT_PERCENRT] = fnew[0].fptr*100/fnew[0].fsize;
							DugsCommandSend(DEMAND_PAGE<<4|DATA_EXPORT_PERCENRT,model_1006.data_demand[DATA_EXPORT_PERCENRT]);
							OSTimeDlyHMSM(0,0,0,1);
						}
						
						
					}
					else
					{
						//res_sd=f_mkfs(2,1,0);
						res_sd =  f_mount(2,NULL); 
						//再重新挂载
						res_sd = f_mount(2,&fs[2]); 
					}
					//res_sd= f_write(&fnew[2], (TCHAR*)&model4001, sizeof(model4001), &fnum);
					data_name[0]='0';
				}
				else
				{
					printf("数据写入失败！\r\n");
				}
				
			}
			model_1006.data_demand[DATA_EXPORT] = 0;
			f_close(&fnew[0]);
			f_close(&fnew[2]);
		}
//		if(model4001.new_log)//日志存储
//		{
//			res_sd = f_open(&fnew[2], "0:log.txt", FA_OPEN_EXISTING | FA_WRITE);
//			if(res_sd == FR_OK)//文件打开成功
//			{
//				//log_time[]={"2000年00月00日00时00分    "};
//				log_time[2] = model4001.time[TIME_YEAR]/10+0x30;
//				log_time[3] = model4001.time[TIME_YEAR]%10+0x30;
//				log_time[6] = model4001.time[TIME_MONTH]/10+0x30;
//				log_time[7] = model4001.time[TIME_MONTH]%10+0x30;
//				log_time[10] = model4001.time[TIME_DAY]/10+0x30;
//				log_time[11] = model4001.time[TIME_DAY]%10+0x30;
//				log_time[14] = model4001.time[TIME_HOUR]/10+0x30;
//				log_time[15] = model4001.time[TIME_HOUR]%10+0x30;
//				log_time[18] = model4001.time[TIME_MIN]/10+0x30;
//				log_time[19] = model4001.time[TIME_MIN]%10+0x30;
//				res_sd=f_lseek(&fnew[2],f_size(&fnew[2]));//查找文件的结尾
//				f_printf(&fnew[2],"%s",log_time);
//				if(model4001.new_log&LOG_ABSORB_TEMP_UP)
//				{
//					f_printf(&fnew[2],"%s","吸收室温度偏高      \n");
//					model4001.new_log&=(~LOG_ABSORB_TEMP_UP);
//				}
//				else if(model4001.new_log&LOG_ABSORB_TEMP_DOWN)
//				{
//					f_printf(&fnew[2],"%s","吸收室温度偏低      \n");
//					model4001.new_log&=(~LOG_ABSORB_TEMP_DOWN);
//				}
//				else if(model4001.new_log&LOG_ABSORB_TEMP_ERROR)
//				{
//					f_printf(&fnew[2],"%s","吸收室温度异常      \n");
//					model4001.new_log&=(~LOG_ABSORB_TEMP_ERROR);
//				}
//				else if(model4001.new_log&LOG_REACTION_TEMP_UP)
//				{
//					f_printf(&fnew[2],"%s","反应室温度偏高      \n");
//					model4001.new_log&=(~LOG_REACTION_TEMP_UP);
//				}
//				else if(model4001.new_log&LOG_REACTION_TEMP_DOWN)
//				{
//					f_printf(&fnew[2],"%s","反应室温度偏低      \n");
//					model4001.new_log&=(~LOG_REACTION_TEMP_DOWN);
//				}
//				else if(model4001.new_log&LOG_REACTION_TEMP_ERROR)
//				{
//					f_printf(&fnew[2],"%s","反应室温度异常      \n");
//					model4001.new_log&=(~LOG_REACTION_TEMP_ERROR);
//				}
//				else if(model4001.new_log&LOG_LED_TEMP_UP)
//				{
//					f_printf(&fnew[2],"%s","光源温度偏高        \n");
//					model4001.new_log&=(~LOG_LED_TEMP_UP);
//				}
//				else if(model4001.new_log&LOG_LED_TEMP_DOWN)
//				{
//					f_printf(&fnew[2],"%s","光源温度偏低        \n");
//					model4001.new_log&=(~LOG_LED_TEMP_DOWN);
//				}
//				else if(model4001.new_log&LOG_LED_TEMP_ERROR)
//				{
//					f_printf(&fnew[2],"%s","光源温度异常        \n");
//					model4001.new_log&=(~LOG_LED_TEMP_ERROR);
//				}
//				else if(model4001.new_log&LOG_RAY_VOLTAGE_UP)
//				{
//					f_printf(&fnew[2],"%s","荧光信号值偏高      \n");
//					model4001.new_log&=(~LOG_RAY_VOLTAGE_UP);
//				}
//				else if(model4001.new_log&LOG_RAY_VOLTAGE_DOWN)
//				{
//					f_printf(&fnew[2],"%s","荧光信号值偏低      \n");
//					model4001.new_log&=(~LOG_RAY_VOLTAGE_DOWN);
//				}
//				else if(model4001.new_log&LOG_ZERO_VOLTAGE_UP)
//				{
//					f_printf(&fnew[2],"%s","零点荧光信号值偏高  \n");
//					model4001.new_log&=(~LOG_ZERO_VOLTAGE_UP);
//				}
//				else if(model4001.new_log&LOG_ZERO_VOLTAGE_DOWN)
//				{
//					f_printf(&fnew[2],"%s","零点荧光信号值偏低  \n");
//					model4001.new_log&=(~LOG_ZERO_VOLTAGE_DOWN);
//				}
//				else if(model4001.new_log&LOG_PMT_VOLTAGE_UP)
//				{
//					f_printf(&fnew[2],"%s","PMT高压偏高         \n");
//					model4001.new_log&=(~LOG_PMT_VOLTAGE_UP);
//				}
//				else if(model4001.new_log&LOG_PMT_VOLTAGE_DOWN)
//				{
//					f_printf(&fnew[2],"%s","PMT高压偏低         \n");
//					model4001.new_log&=(~LOG_PMT_VOLTAGE_DOWN);
//				}
//				else if(model4001.new_log&LOG_GAS_FLOW_UP)
//				{
//					f_printf(&fnew[2],"%s","气体流量偏高        \n");
//					model4001.new_log&=(~LOG_GAS_FLOW_UP);
//				}
//				else if(model4001.new_log&LOG_GAS_FLOW_DOWN)
//				{
//					f_printf(&fnew[2],"%s","气体流量偏低        \n");
//					model4001.new_log&=(~LOG_GAS_FLOW_DOWN);
//				}
//				else if(model4001.new_log&LOG_LIQUID_FLOW_UP)
//				{
//					f_printf(&fnew[2],"%s","液体流量偏高        \n");
//					model4001.new_log&=(~LOG_LIQUID_FLOW_UP);
//				}
//				else if(model4001.new_log&LOG_LIQUID_FLOW_DOWN)
//				{
//					f_printf(&fnew[2],"%s","液体流量偏低        \n");
//					model4001.new_log&=(~LOG_LIQUID_FLOW_DOWN);
//				}
//				else if(model4001.new_log&LOG_AUTO_ZERO)
//				{
//					f_printf(&fnew[2],"%s","自动零点校准        \n");
//					model4001.new_log&=(~LOG_AUTO_ZERO);
//				}
//				else if(model4001.new_log&LOG_AUTO_FLUID)
//				{
//					f_printf(&fnew[2],"%s","自动标液校准        \n");
//					model4001.new_log&=(~LOG_AUTO_FLUID);
//				}
//				else if(model4001.new_log&LOG_AUTO_GAS)
//				{
//					f_printf(&fnew[2],"%s","自动标气校准        \n");
//					model4001.new_log&=(~LOG_AUTO_GAS);
//				}
//				else if(model4001.new_log&LOG_HAND_FLUID)
//				{
//					f_printf(&fnew[2],"%s","手动标液校准        \n");
//					model4001.new_log&=(~LOG_HAND_FLUID);
//				}
//				else if(model4001.new_log&LOG_HAND_GAS)
//				{
//					f_printf(&fnew[2],"%s","手动标气校准        \n");
//					model4001.new_log&=(~LOG_HAND_GAS);
//				}
//				else if(model4001.new_log&LOG_FLUID_FLOW_RATE)
//				{
//					f_printf(&fnew[2],"%s","修改液体流量校准系数\n");
//					model4001.new_log&=(~LOG_FLUID_FLOW_RATE);
//				}
//				else if(model4001.new_log&LOG_GAS_FLOW_RATE)
//				{
//					f_printf(&fnew[2],"%s","修改气体流量校准系数\n");
//					model4001.new_log&=(~LOG_GAS_FLOW_RATE);
//				}
//				else if(model4001.new_log&LOG_PUMP_SPEED)
//				{
//					f_printf(&fnew[2],"%s","修改蠕动泵转速      \n");
//					model4001.new_log&=(~LOG_PUMP_SPEED);
//				}
//				else if(model4001.new_log&LOG_COM_BAUD)
//				{
//					f_printf(&fnew[2],"%s","修改COM端口         \n");
//					model4001.new_log&=(~LOG_COM_BAUD);
//				}
//				else if(model4001.new_log&LOG_AUTO_FLUID_ERROR)
//				{
//					f_printf(&fnew[2],"%s","自动标液校准异常    \n");
//					model4001.new_log&=(~LOG_AUTO_FLUID_ERROR);
//				}
//				else if(model4001.new_log&LOG_AUTO_GAS_ERROR)
//				{
//					f_printf(&fnew[2],"%s","自动标气校准异常    \n");
//					model4001.new_log&=(~LOG_AUTO_GAS_ERROR);
//				}
//				//res_sd= f_write(&fnew[2], (TCHAR*)&model4001, sizeof(model4001), &fnum);
//				if(res_sd == FR_OK)
//				{
//					printf("日志保存成功！\r\n");
//				}
//				else
//				{
//					printf("日志保存失败！\r\n");
//				}
//				
//				//关闭文件
//				model4001.new_log = 0;
//			}
//			else if(res_sd == FR_NO_FILE)
//			{
//				res_sd = f_open(&fnew[2], "0:log.txt", FA_CREATE_ALWAYS | FA_WRITE);
//				if(res_sd == FR_OK)
//				{
//					printf("日志文件创建成功！\r\n");
//				}
//				else
//				{
//					printf("日志文件创建失败！\r\n");
//				}
//				
//				//关闭文件
//				f_close(&fnew[2]);
//			}
//			else if(res_sd == FR_NO_FILESYSTEM)
//			{
//				printf("Nand Flash没有文件系统，即将进行格式化...\r\n");
//				//格式化
//				res_sd=f_mkfs(0,1,0);//格式化FLASH,2,盘符;1,不需要引导区,8个扇区为1个簇
//				
//				if(res_sd == FR_OK)
//				{
//					printf("Nand Flash成功格式化！\r\n");
//					//格式化后先取消挂载
//					res_sd =  f_mount(0,NULL); 
//					//再重新挂载
//					res_sd = f_mount(0,&fs[1]); 
//				}
//				else
//				{
//					printf("文件格式化失败！错误代码：%d\r\n",res_sd);
//					//while(1);
//				}
//			}
//			f_close(&fnew[2]);
//		}
		#endif
		//数据查询画图，改查询时间
//		if(model4001.data_demand[DATA_DEMAND]||model4001.data_demand[NEXT_DAY]||model4001.data_demand[LAST_DAY])//
//		{
//			if(model4001.data_demand[NEXT_DAY]==1)
//			{
//				model4001.data_demand[NEXT_DAY]=0;
//				model4001.data_demand[STAR_DAY]++;
//			}
//			if(model4001.data_demand[LAST_DAY]==1)
//			{
//				model4001.data_demand[LAST_DAY]=0;
//				model4001.data_demand[STAR_DAY]--;
//			}
//			if(model4001.data_demand[STAR_DAY]<=0)
//			{
//				model4001.data_demand[STAR_MONTH]--;//月--
//				if(model4001.data_demand[STAR_MONTH]==0)//判断月是否为零，0—>12
//				{
//					model4001.data_demand[STAR_MONTH] = 12;
//					model4001.data_demand[STAR_YEAR]--;
//				}
//				if(model4001.data_demand[STAR_MONTH]==2)
//				{
//					if(model4001.data_demand[STAR_YEAR]%4==0)//
//						model4001.data_demand[STAR_DAY]=29;
//					else
//						model4001.data_demand[STAR_DAY]=28;
//				}
//				else if(model4001.data_demand[STAR_MONTH]==4||model4001.data_demand[STAR_MONTH]==6||
//					model4001.data_demand[STAR_MONTH]==9||model4001.data_demand[STAR_MONTH]==11)
//				model4001.data_demand[STAR_DAY]=30;
//				else
//					model4001.data_demand[STAR_DAY]=31;
//			}
//			else if(model4001.data_demand[STAR_MONTH]==2&&model4001.data_demand[STAR_DAY]>28)
//			{
//				if(model4001.data_demand[STAR_YEAR]%4!=0)//
//				{
//					model4001.data_demand[STAR_DAY] = 1;
//					model4001.data_demand[STAR_MONTH]++;
//				}
//				else if(model4001.data_demand[STAR_DAY]>29)
//				{
//					model4001.data_demand[STAR_DAY] = 1;
//					model4001.data_demand[STAR_MONTH]++;
//				}
//			}
//			else if(model4001.data_demand[STAR_DAY]>30)
//			{
//				if(model4001.data_demand[STAR_MONTH]==4||model4001.data_demand[STAR_MONTH]==6||
//					model4001.data_demand[STAR_MONTH]==9||model4001.data_demand[STAR_MONTH]==11)
//				{
//					model4001.data_demand[STAR_DAY] = 1;
//					model4001.data_demand[STAR_MONTH]++;
//				}
//				else  if(model4001.data_demand[STAR_DAY]>31)
//				{
//					model4001.data_demand[STAR_DAY] = 1;
//					model4001.data_demand[STAR_MONTH]++;
//					if(model4001.data_demand[STAR_MONTH]>12)
//					{
//						model4001.data_demand[STAR_MONTH]=1;
//						model4001.data_demand[STAR_YEAR]++;
//					}
//				}
//			}
//			DugsCommandSend(DEMAND_PAGE<<4|STAR_YEAR,model4001.data_demand[STAR_YEAR]);
//			DugsCommandSend(DEMAND_PAGE<<4|STAR_MONTH,model4001.data_demand[STAR_MONTH]);
//			DugsCommandSend(DEMAND_PAGE<<4|STAR_DAY,model4001.data_demand[STAR_DAY]);
//			//画曲线
//			model4001.data_demand[DATA_DEMAND] = 0;
//			Data_display(model4001.data_demand);
//			
//		}
//		log_display();//显示日志
	}

}
u8 referlin[8]={0x5A,0xA5,0x04,0x84,0x02,0x00,0x00};
unsigned char erasrlin[7] = {0x5A,0xA5,0x03,0x80,0xEB,0x57};
void Data_display(short *time)
{
	int data=0,line=0,length;
	char time_d,refertime_d;
	char hoco_data_x;//数据坐标
	//获取文件名
	data_name[4] = time[TIME_YEAR]/10%10+0x30;
	data_name[5] = time[TIME_YEAR]%10+0x30;
	data_name[6] = time[TIME_MONTH]/10+0x30;
	data_name[7] = time[TIME_MONTH]%10+0x30;
	refertime_d = time[TIME_DAY];
//	data_name[8] = time[TIME_DAY]/10+0x30;
//	data_name[9] = time[TIME_DAY]%10+0x30;
	comSendBuf(COM1, erasrlin, 6);//擦除曲线
	//打开文件
	res_sd = f_open(&fnew[0], (const char*)data_name, FA_OPEN_EXISTING | FA_READ);
	if(res_sd == FR_OK)//成功打开文件
	{
		if(!f_gets(ReadBuffer,200,&fnew[0]))//读取第一行标题
		{
			//能打开文件却读不到数据 取消挂载
			f_close(&fnew[0]);
			res_sd =  f_mount(0,NULL);//取消挂在
			return ;
		}
		//查找到当天数据位置
		referfile_time(fnew[0].fptr,f_size(&fnew[0]),time);
		
		while(f_gets(ReadBuffer,120,&fnew[0]))//一行行读取数据
		{
			//获取时间 20**/**/** **:**
			//for(length=0;ReadBuffer[length]!=',';length++);//查找到第一组数据：时间
			length = 6;
			if(ReadBuffer[length]!='/')//十月以下第六位为分隔符‘/’
				length++;
			if(ReadBuffer[length]!='/')//十月以上第七位为分隔符‘/’
				continue;//没有取到整行，从新取
			time_d = ReadBuffer[++length]-0x30;
			if(ReadBuffer[length+1]!=' ')//大于十天
				time_d = time_d*10+(ReadBuffer[++length]-0x30);
			if(time_d!=refertime_d)
			{
				f_close(&fnew[0]);
				return;//没有找到数据 或者数据读取完毕
			}

			line++;
			if(line%3==0)
			{
				for(;ReadBuffer[length]!=',';length++);//查找到一组数据：甲醛数据
				hoco_data_x=length+1;
				for(;ReadBuffer[length]!=',';length++);//查找到一组数据：甲醛数据
				ReadBuffer[length] = '\0';
				data=atof(&ReadBuffer[hoco_data_x])*100;//转换为整形数据
				data+=500;//画图时无法出现负值，强制让数据偏移5个点。使最低可以画出-5的曲线
				if(data<0)data=0;//偏移5个点后再出现负值强制为零
				data*=0.08f;
				referlin[5] = (data>>8)&0xff;
				referlin[6] = data&0xff;
				comSendBuf(COM1, referlin, 7);
			//printf("%0.2f\n",data/100.0f);
				data = 0;//清楚数据
			}
			//将数据显示到触屏上
			OSTimeDlyHMSM(0,0,0,1);
		}
		f_close(&fnew[0]);
		return ;//读取数据失败，可能是数据读取完毕返回，
	}
	else //if(res_sd == FR_DISK_ERR)
	{
		f_close(&fnew[0]);
		res_sd =  f_mount(0,NULL); 
	//再重新挂载
		SD_Init();
		OSTimeDlyHMSM(0,0,0,100);
		res_sd = f_mount(0,&fs[0]); 
		
		//printf("数据写入失败！\r\n");
	}
	f_close(&fnew[0]);
}


int referfile_time(u32 start_fptr,u32 end_fptr,short time[])
{
	u16 length=0;
	char time_d,time_h,time_m,refertime_d;
	
	if(end_fptr-start_fptr<100)//如果没有查到返回
		return end_fptr;
	f_lseek(&fnew[0],(start_fptr+end_fptr)/2);//跳转到中间位置
	refertime_d = time[TIME_DAY];
	OSTimeDlyHMSM(0,0,0,1);
	f_gets(ReadBuffer,120,&fnew[0]);
	f_gets(ReadBuffer,120,&fnew[0]);
	//*********************获取时间天
	length = 6;
	if(ReadBuffer[length]!='/')//十月以下第六位为分隔符‘/’
		length++;
	time_d = ReadBuffer[++length]-0x30;
	if(ReadBuffer[length+1]!=' ')//大于十天
		time_d = time_d*10+(ReadBuffer[++length]-0x30);
	if(refertime_d>time_d)
	{
		if(fnew[0].fptr!=start_fptr)
			referfile_time(fnew[0].fptr,end_fptr,time);
		else return fnew[0].fptr;
			
	}
	else if(refertime_d<time_d)
	{
		if(fnew[0].fptr!=end_fptr)	
			referfile_time(start_fptr,fnew[0].fptr,time);
			else return fnew[0].fptr;
	}
	else
	{
		//获取时分
		length+=2;//跳过空格
		time_h = ReadBuffer[length]-0x30;
		if(ReadBuffer[length+1]!=':')//大于十h
			time_h = time_h*10+(ReadBuffer[++length]-0x30);
		length+=2;//跳过冒号
		time_m = ReadBuffer[length]-0x30;
		if(ReadBuffer[length+1]!=',')//大于十h
			time_m = time_m*10+(ReadBuffer[++length]-0x30);
		
		if(time_h>0||time_m>5)//找到起始五分钟内的数据就算找到
		{
			if(fnew[0].fptr!=end_fptr)	
			referfile_time(start_fptr,fnew[0].fptr,time);
			else return fnew[0].fptr;
		}
		else return fnew[0].fptr;
	}
	return fnew[0].fptr;
}

void log_display()
{
	if(model4001.log[LOG_NEXT]||model4001.log[LOG_LAST]||model4001.log[LOG_NEW])
		{
			int i=0;
			res_sd = f_open(&fnew[2], "0:log.txt", FA_OPEN_EXISTING | FA_READ);
			
			if(res_sd==FR_OK)
			{
				if(model4001.log[LOG_NEW])//从新查询日志
				{
					res_sd=f_lseek(&fnew[2],f_size(&fnew[2]));//查找文件的结尾
					model4001.log_pftr[LOG_FPTR_END] = fnew[2].fptr;
					if(model4001.log_pftr[LOG_FPTR_END]>250)
						model4001.log_pftr[LOG_FPTR_START] = model4001.log_pftr[LOG_FPTR_END]-250;
					else
						model4001.log_pftr[LOG_FPTR_START] = 0;
					//fnew[2].fptr += 50;//为了配合往前跳转100的动作，第一次强制后移50（一条数据）
				}
				else if(model4001.log[LOG_LAST])//上一页日志
				{
					model4001.log_pftr[LOG_FPTR_END] = model4001.log_pftr[LOG_FPTR_START];
					if(model4001.log_pftr[LOG_FPTR_START]>250)
					{
						model4001.log_pftr[LOG_FPTR_START] -= 250;
					}
					else
						model4001.log_pftr[LOG_FPTR_START] = 0;
					//res_sd=f_lseek(&fnew[2],model4001.log_pftr[LOG_FPTR_START]);//查找上次读文件的位置

				}
				else
				{
					model4001.log_pftr[LOG_FPTR_START] = model4001.log_pftr[LOG_FPTR_END];
					model4001.log_pftr[LOG_FPTR_END]+=250;
					if(model4001.log_pftr[LOG_FPTR_END]>f_size(&fnew[2]))
					model4001.log_pftr[LOG_FPTR_END] = f_size(&fnew[2]);
				}
				res_sd=f_lseek(&fnew[2],model4001.log_pftr[LOG_FPTR_START]);//查找读文件的位置
				if(model4001.log_pftr[LOG_FPTR_START]!=0&&(!model4001.log[LOG_NEXT]))//开始指针如果不为零且不是翻下一页则需要从新准确定位
				{
					f_gets(ReadBuffer2,50,&fnew[2]);//指针跳转时采用多往文件前跳转，开始指针不准确，先把多出来的内容读出来（f_get读文件遇到回车换行就结束）
					model4001.log_pftr[LOG_FPTR_START] = fnew[2].fptr;//从新定位准确的位置
				}
				for(i=0;i<5;i++)//显示日志
				{
					if(model4001.log_pftr[LOG_FPTR_END] != model4001.log_pftr[LOG_FPTR_START])
					{
						if(fnew[2].fptr<model4001.log_pftr[LOG_FPTR_END])
						{
							if(f_gets(ReadBuffer2,50,&fnew[2]))
								DugsText_Display(0x0300+((i*2)<<4),ReadBuffer2,46);
							else //读不到数据显示空
							DugsText_Display(0x0300+((i*2)<<4),&ReadBuffer2[50],46);//显示空
						}
						else //文件显示完也显示空
							for(;i<5;i++)
							DugsText_Display(0x0300+((i*2)<<4),&ReadBuffer2[50],46);//显示空
					}
					else//文件显示完也显示空
						for(;i<5;i++)
							DugsText_Display(0x0300+((i*2)<<4),&ReadBuffer2[50],46);//显示空
					//OSTimeDlyHMSM(0,0,0,10);
				}
				
				model4001.log_pftr[LOG_FPTR_END] = fnew[2].fptr;
			}
			f_close(&fnew[2]);
			model4001.log[LOG_NEXT] =0;
			model4001.log[LOG_LAST] =0;
			model4001.log[LOG_NEW] = 0;
		}
}

