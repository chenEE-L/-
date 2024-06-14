#include "app.h"
#include "fatfs.h"
#include "SPI_SD_driver.h"
#include "ff.h"
#include "bsp.h"
#include "ftl.h"
#include "usbh_bsp_msc.h"
#include<stdlib.h>

uint8_t g_TestBuf[BUF_SIZE];

FATFS fs[3]; //FatFs�ļ�ϵͳ����
FIL fnew[3]; //�ļ�����
FRESULT res_sd;//�ļ��������
UINT fnum; //�ļ��ɹ���д����
UINT fnum_read,fnum_write; //�ļ��ɹ���д����
TCHAR  ReadBuffer[1024] = {0};
BYTE WriteBuffer[] = "ʱ��,Һ������,Һ��Ũ��,�����������,������������,�������Ũ��,��������Ũ��,��ŷ�״̬,����״̬,ע���״̬,���ӳ����¶�,���ӳ���ʪ��,���ӳ���ѹ��,б��ϵ��\n"; //д������
TCHAR ReadBuffer2[100] = {'\0'}; //du������2

BYTE data_name[] = {"0:201901.csv"};
BYTE log_time[]={"2000��00��00��00ʱ00��    "};
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
		printf("SD����ʼ���ɹ�����������SD����\r\n");
	}
	
	
	res_sd = f_mount(0,&fs[0]); //����SD��
	//***********************��ʽ������****************************
	if(res_sd == FR_NO_FILESYSTEM)
	{
		printf("SD��û���ļ�ϵͳ���������и�ʽ��...\r\n");
		//��ʽ��
		res_sd=f_mkfs(0,1,0);//��ʽ��SD��,2,�̷�;1,����Ҫ������
		
		if(res_sd == FR_OK)
		{
			printf("SD���ɹ���ʽ����\r\n");
			//��ʽ������ȡ������
			res_sd =  f_mount(1,NULL); 
			//�����¹���
			res_sd = f_mount(1,&fs[0]); 
		}
		else
		{
			printf("�ļ���ʽ��ʧ�ܣ�������룺%d\r\n",res_sd);
			//while(1);
		}
	}
	else if(res_sd != FR_OK)
	{
		printf("SD�������ļ�ϵͳʧ�ܣ���������Ϊ�ļ���ʼ��ʧ�ܣ�������룺%d\r\n", res_sd);
	}
	else
	{
		printf("SD���ļ�ϵͳ���سɹ���\r\n");
	}
	res_sd = f_mount(2,&fs[2]); //����U��
	//***********************��ʽ������****************************
	if(res_sd != FR_OK)
	{
		printf("U�̹����ļ�ϵͳʧ�ܣ���������Ϊ�ļ���ʼ��ʧ�ܣ�������룺%d\r\n", res_sd);
	}
	else
	{
		printf("U���ļ�ϵͳ���سɹ���\r\n");
	}
	
	fnew[0].fs = &fs[0];
	res_sd = f_open(&fnew[0], "0:config.txt", FA_OPEN_EXISTING | FA_READ);
		if(res_sd == FR_OK)
		{
			//printf("���ļ��ɹ�����ʼд�����ݣ�\r\n");
			res_sd= f_read(&fnew[0], (TCHAR*)&model_1006, sizeof(model_1006), &fnum);
			 //f_gets((TCHAR*)&model4001,sizeof(model4001),&fnew[1]);
			//�ر��ļ�
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
		if(time_count%10==0)//�����������
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
		if(time_count>=300)//1min����һ������ 100ms*600=1min
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
				//printf("���ļ��ɹ�����ʼд�����ݣ�\r\n");
				res_sd=f_lseek(&fnew[0],f_size(&fnew[0]));//�����ļ��Ľ�β
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
					f_printf(&fnew[0],"%s,","ֹͣ");
				else if(model_1006.start_product[INJECTION_PUMP_STATUS]==1)
					f_printf(&fnew[0],"%s,","��Һ");
				else
					f_printf(&fnew[0],"%s,","��Һ");
				
				sprintf(float_data,"%0.1f",model_1006.file.air_temp/3000.0f);
				f_printf(&fnew[0],"%s,",float_data);
				
				sprintf(float_data,"%0.1f",model_1006.file.air_rh/3000.0f);
				f_printf(&fnew[0],"%s,",float_data);
				
				sprintf(float_data,"%0.1f",model_1006.file.air_pressure/3000.0f);
				
				f_printf(&fnew[0],"%s,",float_data);
				
				sprintf(float_data,"%0.3f",model_1006.file.slope_coefficients/30000.0f);
				f_printf(&fnew[0],"%s,\n",float_data);//���һ�����ݷ��ͻ��з�
				if(res_sd == FR_OK)
				{
					//sive_count++;
					//printf("����д��ɹ���\r\n");
					//printf("���ݣ�%s����д��%d���ַ�\r\n", WriteBuffer, fnum);
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
					//�����ļ�ʧ�ܴ���ʧ����Ϣ
				}
			}
			else //if(res_sd == FR_DISK_ERR)
			{
				f_close(&fnew[0]);
				res_sd =  f_mount(0,NULL); 
			//�����¹���
				SD_Init();
				OSTimeDlyHMSM(0,0,0,100);
				res_sd = f_mount(0,&fs[0]); 
				
				//printf("����д��ʧ�ܣ�\r\n");
			}
			//�ر��ļ�
			f_close(&fnew[0]);
			//��������
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
		
		#if 1//��������
		if(model_1006.new_data)
		{
			res_sd = f_open(&fnew[0], "0:config.txt", FA_OPEN_EXISTING | FA_WRITE);
			if(res_sd == FR_OK)//�ļ��򿪳ɹ�
			{
				//printf("���ļ��ɹ�����ʼд�����ݣ�\r\n");
				res_sd= f_write(&fnew[0], (TCHAR*)&model_1006, sizeof(model_1006), &fnum);
				model_1006.new_data = 0;
				if(res_sd == FR_OK)
				{
					printf("���ò�������ɹ���\r\n");
				}
				else
				{
					printf("���ò�������ʧ�ܣ�\r\n");
				}
				
				//�ر��ļ�
				f_close(&fnew[0]);
			}
			else if(res_sd == FR_NO_FILE)
			{
				res_sd = f_open(&fnew[0], "0:config.txt", FA_CREATE_ALWAYS | FA_WRITE);
				res_sd= f_write(&fnew[0], (TCHAR*)&model_1006, sizeof(model_1006), &fnum);
				model_1006.new_data = 0;
				if(res_sd == FR_OK)
				{
					printf("���ò�������ɹ���\r\n");
				}
				else
				{
					res_sd =  f_mount(0,NULL); 
					//�����¹���
					printf("���ò�������ʧ�ܣ�\r\n");
				}
				
				//�ر��ļ�
				f_close(&fnew[0]);
			}
			else
			{
				FTL_Format();
				res_sd=f_mkfs(0,1,0);//��ʽ��FLASH,2,�̷�;1,����Ҫ������,8������Ϊ1����
				{
					res_sd =  f_mount(0,NULL); 
					//�����¹���
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
					USBH_Process(&USB_OTG_Core, &USB_Host);//�Ͳ�Ѱ
					OSTimeDlyHMSM(0,0,0,1);
				}
//				data_name[0]='2';
//				res_sd = f_open(&fnew[2], (const char*)data_name, FA_CREATE_ALWAYS | FA_WRITE);//��U�̴������ļ�
//				if(res_sd==FR_OK)
//				{
//					res_sd= f_write(&fnew[2], WriteBuffer,sizeof(WriteBuffer), &fnum);
//					printf("����д��ɹ���\r\n");
//				}
//				else
//				{
//					printf("����д��ʧ�ܣ�\r\n");
//				}
//				data_name[0]='0';
				res_sd = f_open(&fnew[0], (const char*)data_name, FA_OPEN_EXISTING | FA_READ);//��SD���ļ�
				if(res_sd==FR_OK)
				{
					printf("�ļ��򿪳ɹ���\r\n");
					data_name[0]='2';
					res_sd = f_open(&fnew[2], (const char*)data_name, FA_CREATE_ALWAYS | FA_WRITE);//��U�̴������ļ�
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
						//�����¹���
						res_sd = f_mount(2,&fs[2]); 
					}
					//res_sd= f_write(&fnew[2], (TCHAR*)&model4001, sizeof(model4001), &fnum);
					data_name[0]='0';
				}
				else
				{
					printf("����д��ʧ�ܣ�\r\n");
				}
				
			}
			model_1006.data_demand[DATA_EXPORT] = 0;
			f_close(&fnew[0]);
			f_close(&fnew[2]);
		}
//		if(model4001.new_log)//��־�洢
//		{
//			res_sd = f_open(&fnew[2], "0:log.txt", FA_OPEN_EXISTING | FA_WRITE);
//			if(res_sd == FR_OK)//�ļ��򿪳ɹ�
//			{
//				//log_time[]={"2000��00��00��00ʱ00��    "};
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
//				res_sd=f_lseek(&fnew[2],f_size(&fnew[2]));//�����ļ��Ľ�β
//				f_printf(&fnew[2],"%s",log_time);
//				if(model4001.new_log&LOG_ABSORB_TEMP_UP)
//				{
//					f_printf(&fnew[2],"%s","�������¶�ƫ��      \n");
//					model4001.new_log&=(~LOG_ABSORB_TEMP_UP);
//				}
//				else if(model4001.new_log&LOG_ABSORB_TEMP_DOWN)
//				{
//					f_printf(&fnew[2],"%s","�������¶�ƫ��      \n");
//					model4001.new_log&=(~LOG_ABSORB_TEMP_DOWN);
//				}
//				else if(model4001.new_log&LOG_ABSORB_TEMP_ERROR)
//				{
//					f_printf(&fnew[2],"%s","�������¶��쳣      \n");
//					model4001.new_log&=(~LOG_ABSORB_TEMP_ERROR);
//				}
//				else if(model4001.new_log&LOG_REACTION_TEMP_UP)
//				{
//					f_printf(&fnew[2],"%s","��Ӧ���¶�ƫ��      \n");
//					model4001.new_log&=(~LOG_REACTION_TEMP_UP);
//				}
//				else if(model4001.new_log&LOG_REACTION_TEMP_DOWN)
//				{
//					f_printf(&fnew[2],"%s","��Ӧ���¶�ƫ��      \n");
//					model4001.new_log&=(~LOG_REACTION_TEMP_DOWN);
//				}
//				else if(model4001.new_log&LOG_REACTION_TEMP_ERROR)
//				{
//					f_printf(&fnew[2],"%s","��Ӧ���¶��쳣      \n");
//					model4001.new_log&=(~LOG_REACTION_TEMP_ERROR);
//				}
//				else if(model4001.new_log&LOG_LED_TEMP_UP)
//				{
//					f_printf(&fnew[2],"%s","��Դ�¶�ƫ��        \n");
//					model4001.new_log&=(~LOG_LED_TEMP_UP);
//				}
//				else if(model4001.new_log&LOG_LED_TEMP_DOWN)
//				{
//					f_printf(&fnew[2],"%s","��Դ�¶�ƫ��        \n");
//					model4001.new_log&=(~LOG_LED_TEMP_DOWN);
//				}
//				else if(model4001.new_log&LOG_LED_TEMP_ERROR)
//				{
//					f_printf(&fnew[2],"%s","��Դ�¶��쳣        \n");
//					model4001.new_log&=(~LOG_LED_TEMP_ERROR);
//				}
//				else if(model4001.new_log&LOG_RAY_VOLTAGE_UP)
//				{
//					f_printf(&fnew[2],"%s","ӫ���ź�ֵƫ��      \n");
//					model4001.new_log&=(~LOG_RAY_VOLTAGE_UP);
//				}
//				else if(model4001.new_log&LOG_RAY_VOLTAGE_DOWN)
//				{
//					f_printf(&fnew[2],"%s","ӫ���ź�ֵƫ��      \n");
//					model4001.new_log&=(~LOG_RAY_VOLTAGE_DOWN);
//				}
//				else if(model4001.new_log&LOG_ZERO_VOLTAGE_UP)
//				{
//					f_printf(&fnew[2],"%s","���ӫ���ź�ֵƫ��  \n");
//					model4001.new_log&=(~LOG_ZERO_VOLTAGE_UP);
//				}
//				else if(model4001.new_log&LOG_ZERO_VOLTAGE_DOWN)
//				{
//					f_printf(&fnew[2],"%s","���ӫ���ź�ֵƫ��  \n");
//					model4001.new_log&=(~LOG_ZERO_VOLTAGE_DOWN);
//				}
//				else if(model4001.new_log&LOG_PMT_VOLTAGE_UP)
//				{
//					f_printf(&fnew[2],"%s","PMT��ѹƫ��         \n");
//					model4001.new_log&=(~LOG_PMT_VOLTAGE_UP);
//				}
//				else if(model4001.new_log&LOG_PMT_VOLTAGE_DOWN)
//				{
//					f_printf(&fnew[2],"%s","PMT��ѹƫ��         \n");
//					model4001.new_log&=(~LOG_PMT_VOLTAGE_DOWN);
//				}
//				else if(model4001.new_log&LOG_GAS_FLOW_UP)
//				{
//					f_printf(&fnew[2],"%s","��������ƫ��        \n");
//					model4001.new_log&=(~LOG_GAS_FLOW_UP);
//				}
//				else if(model4001.new_log&LOG_GAS_FLOW_DOWN)
//				{
//					f_printf(&fnew[2],"%s","��������ƫ��        \n");
//					model4001.new_log&=(~LOG_GAS_FLOW_DOWN);
//				}
//				else if(model4001.new_log&LOG_LIQUID_FLOW_UP)
//				{
//					f_printf(&fnew[2],"%s","Һ������ƫ��        \n");
//					model4001.new_log&=(~LOG_LIQUID_FLOW_UP);
//				}
//				else if(model4001.new_log&LOG_LIQUID_FLOW_DOWN)
//				{
//					f_printf(&fnew[2],"%s","Һ������ƫ��        \n");
//					model4001.new_log&=(~LOG_LIQUID_FLOW_DOWN);
//				}
//				else if(model4001.new_log&LOG_AUTO_ZERO)
//				{
//					f_printf(&fnew[2],"%s","�Զ����У׼        \n");
//					model4001.new_log&=(~LOG_AUTO_ZERO);
//				}
//				else if(model4001.new_log&LOG_AUTO_FLUID)
//				{
//					f_printf(&fnew[2],"%s","�Զ���ҺУ׼        \n");
//					model4001.new_log&=(~LOG_AUTO_FLUID);
//				}
//				else if(model4001.new_log&LOG_AUTO_GAS)
//				{
//					f_printf(&fnew[2],"%s","�Զ�����У׼        \n");
//					model4001.new_log&=(~LOG_AUTO_GAS);
//				}
//				else if(model4001.new_log&LOG_HAND_FLUID)
//				{
//					f_printf(&fnew[2],"%s","�ֶ���ҺУ׼        \n");
//					model4001.new_log&=(~LOG_HAND_FLUID);
//				}
//				else if(model4001.new_log&LOG_HAND_GAS)
//				{
//					f_printf(&fnew[2],"%s","�ֶ�����У׼        \n");
//					model4001.new_log&=(~LOG_HAND_GAS);
//				}
//				else if(model4001.new_log&LOG_FLUID_FLOW_RATE)
//				{
//					f_printf(&fnew[2],"%s","�޸�Һ������У׼ϵ��\n");
//					model4001.new_log&=(~LOG_FLUID_FLOW_RATE);
//				}
//				else if(model4001.new_log&LOG_GAS_FLOW_RATE)
//				{
//					f_printf(&fnew[2],"%s","�޸���������У׼ϵ��\n");
//					model4001.new_log&=(~LOG_GAS_FLOW_RATE);
//				}
//				else if(model4001.new_log&LOG_PUMP_SPEED)
//				{
//					f_printf(&fnew[2],"%s","�޸��䶯��ת��      \n");
//					model4001.new_log&=(~LOG_PUMP_SPEED);
//				}
//				else if(model4001.new_log&LOG_COM_BAUD)
//				{
//					f_printf(&fnew[2],"%s","�޸�COM�˿�         \n");
//					model4001.new_log&=(~LOG_COM_BAUD);
//				}
//				else if(model4001.new_log&LOG_AUTO_FLUID_ERROR)
//				{
//					f_printf(&fnew[2],"%s","�Զ���ҺУ׼�쳣    \n");
//					model4001.new_log&=(~LOG_AUTO_FLUID_ERROR);
//				}
//				else if(model4001.new_log&LOG_AUTO_GAS_ERROR)
//				{
//					f_printf(&fnew[2],"%s","�Զ�����У׼�쳣    \n");
//					model4001.new_log&=(~LOG_AUTO_GAS_ERROR);
//				}
//				//res_sd= f_write(&fnew[2], (TCHAR*)&model4001, sizeof(model4001), &fnum);
//				if(res_sd == FR_OK)
//				{
//					printf("��־����ɹ���\r\n");
//				}
//				else
//				{
//					printf("��־����ʧ�ܣ�\r\n");
//				}
//				
//				//�ر��ļ�
//				model4001.new_log = 0;
//			}
//			else if(res_sd == FR_NO_FILE)
//			{
//				res_sd = f_open(&fnew[2], "0:log.txt", FA_CREATE_ALWAYS | FA_WRITE);
//				if(res_sd == FR_OK)
//				{
//					printf("��־�ļ������ɹ���\r\n");
//				}
//				else
//				{
//					printf("��־�ļ�����ʧ�ܣ�\r\n");
//				}
//				
//				//�ر��ļ�
//				f_close(&fnew[2]);
//			}
//			else if(res_sd == FR_NO_FILESYSTEM)
//			{
//				printf("Nand Flashû���ļ�ϵͳ���������и�ʽ��...\r\n");
//				//��ʽ��
//				res_sd=f_mkfs(0,1,0);//��ʽ��FLASH,2,�̷�;1,����Ҫ������,8������Ϊ1����
//				
//				if(res_sd == FR_OK)
//				{
//					printf("Nand Flash�ɹ���ʽ����\r\n");
//					//��ʽ������ȡ������
//					res_sd =  f_mount(0,NULL); 
//					//�����¹���
//					res_sd = f_mount(0,&fs[1]); 
//				}
//				else
//				{
//					printf("�ļ���ʽ��ʧ�ܣ�������룺%d\r\n",res_sd);
//					//while(1);
//				}
//			}
//			f_close(&fnew[2]);
//		}
		#endif
		//���ݲ�ѯ��ͼ���Ĳ�ѯʱ��
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
//				model4001.data_demand[STAR_MONTH]--;//��--
//				if(model4001.data_demand[STAR_MONTH]==0)//�ж����Ƿ�Ϊ�㣬0��>12
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
//			//������
//			model4001.data_demand[DATA_DEMAND] = 0;
//			Data_display(model4001.data_demand);
//			
//		}
//		log_display();//��ʾ��־
	}

}
u8 referlin[8]={0x5A,0xA5,0x04,0x84,0x02,0x00,0x00};
unsigned char erasrlin[7] = {0x5A,0xA5,0x03,0x80,0xEB,0x57};
void Data_display(short *time)
{
	int data=0,line=0,length;
	char time_d,refertime_d;
	char hoco_data_x;//��������
	//��ȡ�ļ���
	data_name[4] = time[TIME_YEAR]/10%10+0x30;
	data_name[5] = time[TIME_YEAR]%10+0x30;
	data_name[6] = time[TIME_MONTH]/10+0x30;
	data_name[7] = time[TIME_MONTH]%10+0x30;
	refertime_d = time[TIME_DAY];
//	data_name[8] = time[TIME_DAY]/10+0x30;
//	data_name[9] = time[TIME_DAY]%10+0x30;
	comSendBuf(COM1, erasrlin, 6);//��������
	//���ļ�
	res_sd = f_open(&fnew[0], (const char*)data_name, FA_OPEN_EXISTING | FA_READ);
	if(res_sd == FR_OK)//�ɹ����ļ�
	{
		if(!f_gets(ReadBuffer,200,&fnew[0]))//��ȡ��һ�б���
		{
			//�ܴ��ļ�ȴ���������� ȡ������
			f_close(&fnew[0]);
			res_sd =  f_mount(0,NULL);//ȡ������
			return ;
		}
		//���ҵ���������λ��
		referfile_time(fnew[0].fptr,f_size(&fnew[0]),time);
		
		while(f_gets(ReadBuffer,120,&fnew[0]))//һ���ж�ȡ����
		{
			//��ȡʱ�� 20**/**/** **:**
			//for(length=0;ReadBuffer[length]!=',';length++);//���ҵ���һ�����ݣ�ʱ��
			length = 6;
			if(ReadBuffer[length]!='/')//ʮ�����µ���λΪ�ָ�����/��
				length++;
			if(ReadBuffer[length]!='/')//ʮ�����ϵ���λΪ�ָ�����/��
				continue;//û��ȡ�����У�����ȡ
			time_d = ReadBuffer[++length]-0x30;
			if(ReadBuffer[length+1]!=' ')//����ʮ��
				time_d = time_d*10+(ReadBuffer[++length]-0x30);
			if(time_d!=refertime_d)
			{
				f_close(&fnew[0]);
				return;//û���ҵ����� �������ݶ�ȡ���
			}

			line++;
			if(line%3==0)
			{
				for(;ReadBuffer[length]!=',';length++);//���ҵ�һ�����ݣ���ȩ����
				hoco_data_x=length+1;
				for(;ReadBuffer[length]!=',';length++);//���ҵ�һ�����ݣ���ȩ����
				ReadBuffer[length] = '\0';
				data=atof(&ReadBuffer[hoco_data_x])*100;//ת��Ϊ��������
				data+=500;//��ͼʱ�޷����ָ�ֵ��ǿ��������ƫ��5���㡣ʹ��Ϳ��Ի���-5������
				if(data<0)data=0;//ƫ��5������ٳ��ָ�ֵǿ��Ϊ��
				data*=0.08f;
				referlin[5] = (data>>8)&0xff;
				referlin[6] = data&0xff;
				comSendBuf(COM1, referlin, 7);
			//printf("%0.2f\n",data/100.0f);
				data = 0;//�������
			}
			//��������ʾ��������
			OSTimeDlyHMSM(0,0,0,1);
		}
		f_close(&fnew[0]);
		return ;//��ȡ����ʧ�ܣ����������ݶ�ȡ��Ϸ��أ�
	}
	else //if(res_sd == FR_DISK_ERR)
	{
		f_close(&fnew[0]);
		res_sd =  f_mount(0,NULL); 
	//�����¹���
		SD_Init();
		OSTimeDlyHMSM(0,0,0,100);
		res_sd = f_mount(0,&fs[0]); 
		
		//printf("����д��ʧ�ܣ�\r\n");
	}
	f_close(&fnew[0]);
}


int referfile_time(u32 start_fptr,u32 end_fptr,short time[])
{
	u16 length=0;
	char time_d,time_h,time_m,refertime_d;
	
	if(end_fptr-start_fptr<100)//���û�в鵽����
		return end_fptr;
	f_lseek(&fnew[0],(start_fptr+end_fptr)/2);//��ת���м�λ��
	refertime_d = time[TIME_DAY];
	OSTimeDlyHMSM(0,0,0,1);
	f_gets(ReadBuffer,120,&fnew[0]);
	f_gets(ReadBuffer,120,&fnew[0]);
	//*********************��ȡʱ����
	length = 6;
	if(ReadBuffer[length]!='/')//ʮ�����µ���λΪ�ָ�����/��
		length++;
	time_d = ReadBuffer[++length]-0x30;
	if(ReadBuffer[length+1]!=' ')//����ʮ��
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
		//��ȡʱ��
		length+=2;//�����ո�
		time_h = ReadBuffer[length]-0x30;
		if(ReadBuffer[length+1]!=':')//����ʮh
			time_h = time_h*10+(ReadBuffer[++length]-0x30);
		length+=2;//����ð��
		time_m = ReadBuffer[length]-0x30;
		if(ReadBuffer[length+1]!=',')//����ʮh
			time_m = time_m*10+(ReadBuffer[++length]-0x30);
		
		if(time_h>0||time_m>5)//�ҵ���ʼ������ڵ����ݾ����ҵ�
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
				if(model4001.log[LOG_NEW])//���²�ѯ��־
				{
					res_sd=f_lseek(&fnew[2],f_size(&fnew[2]));//�����ļ��Ľ�β
					model4001.log_pftr[LOG_FPTR_END] = fnew[2].fptr;
					if(model4001.log_pftr[LOG_FPTR_END]>250)
						model4001.log_pftr[LOG_FPTR_START] = model4001.log_pftr[LOG_FPTR_END]-250;
					else
						model4001.log_pftr[LOG_FPTR_START] = 0;
					//fnew[2].fptr += 50;//Ϊ�������ǰ��ת100�Ķ�������һ��ǿ�ƺ���50��һ�����ݣ�
				}
				else if(model4001.log[LOG_LAST])//��һҳ��־
				{
					model4001.log_pftr[LOG_FPTR_END] = model4001.log_pftr[LOG_FPTR_START];
					if(model4001.log_pftr[LOG_FPTR_START]>250)
					{
						model4001.log_pftr[LOG_FPTR_START] -= 250;
					}
					else
						model4001.log_pftr[LOG_FPTR_START] = 0;
					//res_sd=f_lseek(&fnew[2],model4001.log_pftr[LOG_FPTR_START]);//�����ϴζ��ļ���λ��

				}
				else
				{
					model4001.log_pftr[LOG_FPTR_START] = model4001.log_pftr[LOG_FPTR_END];
					model4001.log_pftr[LOG_FPTR_END]+=250;
					if(model4001.log_pftr[LOG_FPTR_END]>f_size(&fnew[2]))
					model4001.log_pftr[LOG_FPTR_END] = f_size(&fnew[2]);
				}
				res_sd=f_lseek(&fnew[2],model4001.log_pftr[LOG_FPTR_START]);//���Ҷ��ļ���λ��
				if(model4001.log_pftr[LOG_FPTR_START]!=0&&(!model4001.log[LOG_NEXT]))//��ʼָ�������Ϊ���Ҳ��Ƿ���һҳ����Ҫ����׼ȷ��λ
				{
					f_gets(ReadBuffer2,50,&fnew[2]);//ָ����תʱ���ö����ļ�ǰ��ת����ʼָ�벻׼ȷ���ȰѶ���������ݶ�������f_get���ļ������س����оͽ�����
					model4001.log_pftr[LOG_FPTR_START] = fnew[2].fptr;//���¶�λ׼ȷ��λ��
				}
				for(i=0;i<5;i++)//��ʾ��־
				{
					if(model4001.log_pftr[LOG_FPTR_END] != model4001.log_pftr[LOG_FPTR_START])
					{
						if(fnew[2].fptr<model4001.log_pftr[LOG_FPTR_END])
						{
							if(f_gets(ReadBuffer2,50,&fnew[2]))
								DugsText_Display(0x0300+((i*2)<<4),ReadBuffer2,46);
							else //������������ʾ��
							DugsText_Display(0x0300+((i*2)<<4),&ReadBuffer2[50],46);//��ʾ��
						}
						else //�ļ���ʾ��Ҳ��ʾ��
							for(;i<5;i++)
							DugsText_Display(0x0300+((i*2)<<4),&ReadBuffer2[50],46);//��ʾ��
					}
					else//�ļ���ʾ��Ҳ��ʾ��
						for(;i<5;i++)
							DugsText_Display(0x0300+((i*2)<<4),&ReadBuffer2[50],46);//��ʾ��
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

