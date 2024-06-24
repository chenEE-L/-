/*
	 file serial_cmd
*/
#include "serial_command.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "App.h"
#include "bsp.h"
#include "gps.h"
#include "crc.h"
#include "ry_wsy301.h"
#include "beionfluid_s60.h"

unsigned char com = 0;

Alicat Alic1;
//extern OS_EVENT *Usart4Serial_Command;
//extern OS_EVENT *Usart3Serial_Command;

#define PRINT_SERIAL_DEBUG_INFO
#ifdef PRINT_SERIAL_DEBUG_INFO
  #define PRINT_SERIAL_DEBUG(__command__) __command__
#else
  #define PRINT_SERIAL_DEBUG(__command__)
#endif


//uint8_t master_serial_command_word[SERIAL_COMMAND_MAX_LENGTH];
//uint8_t BluetoothCommand[SERIAL_COMMAND_MAX_LENGTH];
//uint8_t AGC_data_word[AGC_DATA_MAX_LENGHT];

void Serial_MasterCommandSend(uint8_t command_code,int command_data)
{
	int i;
	uint8_t str_buf[10];
	int check_code ;	
	str_buf[0] = 0xAA; 
	////发送俯仰角信息		
	str_buf[1] = command_code;
	str_buf[2] = (command_data>>8)&0xff;
	str_buf[3] = command_data&0xff;
	check_code = 0;
	for(i=0;i<4;i++)
	{
		check_code += str_buf[i];
	}
	check_code  = check_code &0xff;
	str_buf[4] = check_code;
	str_buf[5]= 0x0d;	
	str_buf[6]= 0x0a;	
	comSendBuf(COM6, str_buf, 7);
	/*int i;
	uint8_t str_buf[10];
	int check_code ;	
	str_buf[0] = 0xff; 
	////发送俯仰角信息		
	str_buf[1] = command_code;		
	sprintf((char*)(str_buf+2),"%04d",command_data);
	check_code = 0;
	for(i=1;i<6;i++)
	{
		check_code += str_buf[i];
	}
	check_code  = check_code % 256;
	str_buf[6] = check_code;
	str_buf[7]= 0;		
	comSendBuf(COM2, str_buf, 7);*/
}

//*
//*       5A A5 +数据长度（单字节） +指令（单字节） + 地址（双字节） +子长度（字节）+数据（字=双字节）
//*说名：
//*
void DealUsart1_Commend()
{
#if UART1_FIFO_EN == 1 
	uint8_t command_code;
	int get_command = 0;
	int i;
	unsigned char command_word[SERIAL_COMMAND_MAX_LENGTH];

	if(Uart1_command[0]==1)
	{
		//**********提出指令******
		//CPU_IntDis();
			for(i=0; i<UART1_COMMEND_SIZE ;i++)
			{
				command_word[i] = Uart1_command[i+1];
				
				if(i>=(Uart1_command[3]+3))
				{
					break;
				}
			}
		
		//CPU_IntEn();
		Uart1_command[0] =0 ; 
		get_command = 1;
	}
	if(get_command == 1)
	{
		unsigned char command_send[10]={0x5A,0xA5,0x00,0x80,0x00,0x00,0x00};
		if(command_word[3]==0x83)
		{
			command_code = (command_word[4]<<4)|(command_word[5]>>4);//解算存储地址
			if(command_code==HOME_PAGE)
			{
				model_1006.home_page[(command_word[5]&0xff)-0x1B] = (command_word[7]<<8)|command_word[8];
			}
			else if(command_code==START_PRODUCT_PAGE)
			{
				model_1006.start_product[command_word[5]&0x0f] = (command_word[7]<<8)|command_word[8];
				
				if((command_word[5]&0x0f)==START_PRODUCT_COMMEND)
				{
//					if(model_1006.start_product[START_PRODUCT_COMMEND]&0x01)
//					{
//						model_1006.start_product[START_PRODUCT_COMMEND] = 0;
//						//Goto_coordinate(0); 
//						Time2_freq(model_1006.start_product[PUMP_SPEED_SET]*20);
//						GPIO_ResetBits(GPIOE,GPIO_Pin_3);
//						TIM_Cmd(TIM2, ENABLE); //启动
//					}
//					else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x02)
//					{
//						model_1006.start_product[START_PRODUCT_COMMEND] = 0; 
//						//Goto_coordinate(50000);
//						Time2_freq(model_1006.start_product[PUMP_SPEED_SET]*20);
//						GPIO_SetBits(GPIOE,GPIO_Pin_3);
//						TIM_Cmd(TIM2, ENABLE); //启动
//						
//					}
//					else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x04)
//					{
//						model_1006.start_product[START_PRODUCT_COMMEND] = 0;
//					}
//					else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x08)
//					{
//						model_1006.start_product[START_PRODUCT_COMMEND] = 0;
//					}
				}
				else if((command_word[5]&0x0f)==PUMP_SPEED_SET)
				{
//					model4001.new_log |= LOG_PUMP_SPEED;//存日志
//					if(model4001.set[PUMP_SPEED_SET]<=1000&&model4001.set[PUMP_SPEED_SET]>0)  //
//						//Set_Moter_data(0x0002,command_data);
//					Time2_freq(model4001.set[PUMP_SPEED_SET]*20);//设置速度*2*10
//					model4001.status[PUMP_SPEED_STATUS] = model4001.set[PUMP_SPEED_SET];
//					if(model4001.set[PUMP_SPEED_SET]>0)
//						TIM_Cmd(TIM2, ENABLE); //启动
//					else
//						TIM_Cmd(TIM2, DISABLE); //关闭时钟
				}
				else if((command_word[5]&0x0f)==AIR_FLOW_RATE_SET)
				{
//					uint8_t alicat_init[10]={'A',0x36,0x34,0x30,0x30,0x30,0x0d,0x0a,0x0d,'\0'};//初始设定值64000 1L最大流量
//					int flow_rate;
//					flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
//					alicat_init[1] = flow_rate/10000+0x30;
//					alicat_init[2] = flow_rate/1000%10+0x30;
//					alicat_init[3] = flow_rate/100%10+0x30;
//					alicat_init[4] = flow_rate/10%10+0x30;
//					alicat_init[5] = flow_rate%10+0x30;
//					comSendBuf(COM2, alicat_init, 9);
				}
			}
			else if(command_code==STATUS_PAGE)
			{
				model_1006.status[command_word[5]&0x0f] = (command_word[7]<<8)|command_word[8];
			}
			else if(command_code==SET_PAGE)
			{
				model_1006.set[command_word[5]&0x0f] = (command_word[7]<<8)|command_word[8];
//				if((command_word[5]&0x0f)==SC_TEMP_SET)
//				{
//					uint8_t alicat_init[18]={'A','W',0x31,0x33,0x37,'=',0x32,0x37,0x33,0x31,0x35,0x30,0x30,0x30,0x0d,0x0a,0x0d,'\0'};//
//					if(model_1006.set[SC_TEMP_SET])//==0 20210305标况温度设反了
//					{
//						alicat_init[7] = 0x39;
//						alicat_init[8] = 0x38;
//						//comSendBuf(COM6, alicat_init, 17);//设气体流量
//					}
////					else
////					{
////						comSendBuf(COM6, alicat_init, 17);//设气体流量
////					}
//					comSendBuf(COM6, alicat_init, 17);//设气体流量
//				}

			}
			else if(command_code==CALIBRATION_PAGE)
			{
				model_1006.calibration[command_word[5]&0x0f] = (command_word[7]<<8)|command_word[8];
				
				if(model_1006.calibration[CALIBRATION_PASSWORD]==1006)
				{
					command_send[2]=0x04;
					command_send[3]=0x80;
					command_send[4]=0x03;
					command_send[5]=0x00;
					command_send[6]=0x0C;//跳转页12
					comSendBuf(COM1, command_send, 7);
					model_1006.calibration[CALIBRATION_PASSWORD] = 0;
				}
				else if(model_1006.calibration[CALIBRATION_PASSWORD]==1024)
				{
					command_send[2]=0x04;
					command_send[3]=0x80;
					command_send[4]=0x03;
					command_send[5]=0x00;
					command_send[6]=0x11;//跳转页 17
					comSendBuf(COM1, command_send, 7);
					model_1006.calibration[CALIBRATION_PASSWORD] = 0;
				}
			}
			else if(command_code==DEMAND_PAGE)
			{
				model_1006.data_demand[command_word[5]&0x0f] = (command_word[7]<<8)|command_word[8];
			}
						//****************接收序列数据************************
			else if((command_code>>4)==LIST_PAGE) //地址在第一个字节后四位
			{
				model_1006.data_list[command_word[5]] = (command_word[7]<<8)|command_word[8]; //第二个字节均为分地址
			}
			//****************************************************
			model_1006.new_data = 1;//接受到新的数据
		}
		else if(command_word[3]==0x81)
		{
			command_code = command_word[4];//解算寄存器地址
			if(command_code==0x20)//时间寄存器地址
			{
				model_1006.time[TIME_YEAR] = (command_word[6+TIME_YEAR]>>4)*10+(command_word[6+TIME_YEAR]&0x0f);
				model_1006.time[TIME_MONTH] = (command_word[6+TIME_MONTH]>>4)*10+(command_word[6+TIME_MONTH]&0x0f);
				model_1006.time[TIME_DAY] = (command_word[6+TIME_DAY]>>4)*10+(command_word[6+TIME_DAY]&0x0f);
				model_1006.time[TIME_WEAK] = command_word[6+TIME_YEAR];//周只有个位数 不用做转换
				model_1006.time[TIME_HOUR] = (command_word[6+TIME_HOUR]>>4)*10+(command_word[6+TIME_HOUR]&0x0f);
				model_1006.time[TIME_MIN] = (command_word[6+TIME_MIN]>>4)*10+(command_word[6+TIME_MIN]&0x0f);
				model_1006.time[TIME_SEC] = (command_word[6+TIME_SEC]>>4)*10+(command_word[6+TIME_SEC]&0x0f);
			}
		}
	}
	#endif
}



void Serial_MasterCommandProcess_NEW()
{
	#if 0	
	#endif
}

#if 1
//extern SYSTEM_CONFIG_PARA_t system_config;
void help_information()
{
	/*
	printf("Basic Parameters:\n");
	printf("elsubdivsion = %d\n",ElevationMotor.subdivsion);
	printf("elreduction_ration = %0.2f\n",ElevationMotor.reduction_ration);
	printf("azsubdivsion = %d\n",AzimuthMotor.subdivsion);
	printf("azsubdivsion = %0.2f\n",AzimuthMotor.reduction_ration);
	printf("Set information:\n");
	//system_config.ElevationAxis_rotate_time = 111.0;
	printf("eltime11 = %0.2f\n",0.3);
	printf("eltime = %0.2f\n",system_config.ElevationAxis_rotate_time/100.0);
	printf("elspeed = %0.2f\n",system_config.ElevationAxis_rotate_speed_max);
	printf("aztime = %0.2f\n",system_config.AzimuthAxis_rotate_time/100.0);
	printf("azspeed = %0.2f\n",system_config.AzimuthAxis_rotate_speed_max);
	printf("ElevationAxis_angle_top=%d\n",Antenna.ElevationAxis.angle_top);
	printf("command:\n");
	printf(" aztime\n azspeed\n azstop\n azstart\n eltime\n elspeed\n elstop\n elstart\n start\n stop\n");
	*/
}
void help_command(char *command)
{
//	if(strcmp(command,"elspeed")==0)
//	{
//		printf("Set ElevationAxis max speed!\n");
//		printf("for excample:");
//		printf("$elspeed=xxx.xx(Degree per second)\n");
//	}
//	else if(strcmp(command,"eltime")==0)
//	{
//		printf("Set ElevationAxis time!\n");
//		printf("for excample:");
//		printf("$eltime=xxx.xx(second)\n");
//	}
//	else if(strcmp(command,"azspeed")==0)
//	{
//		printf("Set AzimuthAxis max speed!\n");
//		printf("for excample:");
//		printf("$azspeed=xxx.xx(Degree per second)\n");
//		
//	}
//	else if(strcmp(command,"aztime")==0)
//	{
//		printf("Set AzimuthAxis time!\n");
//		printf("for excample:");
//		printf("$aztime=xxx.xx(second)\n");
//	}
//	else if(strcmp(command,"elstop")==0)
//	{
//		printf("$elstop:Set ElevationAxis off!\n");
//	}
//	else if(strcmp(command,"elstart")==0)
//	{
//		printf("$elstart:Open ElevationAxis!\n");
//	}
//	else if(strcmp(command,"azstop")==0)
//	{
//		printf("$azstop:Set AzimuthAxis off!\n");
//	}
//	else if(strcmp(command,"azstart")==0)
//	{
//		printf("$azstart:Open AzimuthAxis!\n");
//	}
//	else if(strcmp(command,"stop")==0)
//	{
//		printf("$stop:Set AzimuthAxis and ElevationAxis off!\n");
//	}
//	else if(strcmp(command,"start")==0)
//	{
//		printf("$start:Open AzimuthAxis and ElevationAxis!\n");
//	}
//	else
//	{
//		printf("can not find the %s instruction!\n",command);
//	}
		
}
void DealUsart3_Commend()
{
	#if UART3_FIFO_EN == 1
	//uint16_t command_code;
	unsigned short command_data;
	//int check_code = 0;
	int get_command = 0;
	int i;
	//int command_num;
	char command_word[UART3_COMMEND_SIZE];
	//char *command_hold[SERIAL_COMMAND_HOLD_LENGTH];
	
	if(Uart3_command[0]==1)
	{
		//**********提出指令******
		//CPU_IntDis();
			for(i=0; i<UART3_COMMEND_SIZE ;i++)
			{
				command_word[i] = Uart3_command[i+1];
			}
		
		//CPU_IntEn();
		Uart3_command[0] =0 ; 
		get_command = 1;
	}
	if(get_command == 1)
	{
		get_command = 0;
		command_data = (((command_word[3]<<8)|command_word[4])-4000);
		model_1006.status[MODEL_1006_TEMP]  = command_data*model_1006.calibration[TEMP_K]/1000.0f+model_1006.calibration[TEMP_B]/10.0f;
		command_data = (command_word[5]<<8)|command_word[6];
		model_1006.status[MODEL_1006_RH] = command_data*model_1006.calibration[HUMIDITY_K]/1000.0f+model_1006.calibration[HUMIDITY_B]/10.0f;
		command_data = (command_word[7]<<8)|command_word[8];
		model_1006.status[AIR_PRESSURE] = command_data*model_1006.calibration[PRESSURE_K]/1000.0f+model_1006.calibration[PRESSURE_B]/10.0f;
	}
	#endif
}


void BluetoothCommandProcess()
{
	#if 1
	//uint16_t command_code;
	float command_data=0;
	//int check_code = 0;
	int get_command = 0;
	int i;
	int command_num;
	char command_word[UART4_COMMEND_SIZE];
	char *command_hold[SERIAL_COMMAND_HOLD_LENGTH];
	
	if(Uart4_command[0]==1)
	{
		//**********提出指令******
		//CPU_IntDis();
			for(i=0; i<UART4_COMMEND_SIZE ;i++)
			{
				command_word[i] = Uart4_command[i+1];
				
				if((command_word[i] =='\r')||(command_word[i]=='\n'))
				{
					//command_word[i] = '\0';
					break;
				}
			}
		
		//CPU_IntEn();
		Uart4_command[0] =0 ; 
		get_command = 1;
	}
	if(get_command == 1)
	{
		get_command = 0;
		command_num = gps_extract_field(command_word,command_hold,' ',SERIAL_COMMAND_HOLD_LENGTH);
	
		for(i=0;i<command_num;i++)//对每条指令进行解析
			{
			
				for(char j=0;command_hold[i][j]!='\0';j++)//分解指令码和数据码
				{
					if(command_hold[i][j]=='=')
					{
						if(!((command_hold[i][0] == 'A')||(command_hold[i][0] == '*')))
						command_hold[i][j] = '\0';
						command_data = atof(&command_hold[i][j+1]);
						break;
					}
				}
				
				#if 1
				if(strcmp(command_hold[i],"Temp1")==0)
				{	
					if(command_data<=200&&command_data>=0)  //
						printf("吸收室控温设置%0.1f℃\n",command_data/10.0f);//
					else
						printf("吸收室控温设置超限！\n");
				}
				else if(strcmp(command_hold[i],"Temp2")==0)
				{
				if(command_data<=900&&command_data>=500)  //
						printf("反应室控温设置%0.1f℃\n",command_data/10.0f);//
					else
						printf("反应室控温设置超限！\n");
						
				}
				else if(strcmp(command_hold[i],"Temp3")==0)
				{
						
					if(command_data<=300&&command_data>=150)  //
						printf("LED控温设置%0.1f℃\n",command_data/10.0f);//
					else
						printf("LED控温设置超限！\n");
					
				}
				else if(strcmp(command_hold[i],"Relay1")==0)
				{
					if(command_hold[i+1][1]=='n')
					{
						printf("电磁阀1开!\n");
						GPIOC_0_ON;
						i++;
					}
					else if(command_hold[i+1][1]=='f')
					{
						printf("电磁阀1关!\n");
						GPIOC_0_OFF;
						i++;
					}
				}
				else if(strcmp(command_hold[i],"help")==0)
				{
					if((i+1)<command_num)
					{
						for(i++;i<command_num;i++)
						help_command(command_hold[i]);
					}
					else
						help_information();
					return ;			
				}
				else if(strcmp(command_hold[i],"Relay2")==0)
				{
					//Antenna.ElevationAxis.rotate_status = ANTENNA_WORK_MODE_STOP;
					if(command_hold[i+1][1]=='n')
					{
						printf("电磁阀2开!\n");
						GPIOC_1_ON;
						i++;
					}
					else if(command_hold[i+1][1]=='f')
					{
						printf("电磁阀2关!\n");
						GPIOC_1_OFF;
						i++;
					}
					
				}
				else if(strcmp(command_hold[i],"Moterspeed")==0)
				{
					if(command_data<=1000&&command_data>0)  //
						//Set_Moter_data(0x0002,command_data);
					Time2_freq(command_data*20);//设置速度*2*10
					model4001.status[PUMP_SPEED_STATUS] = command_data;
					if(command_data>0)
						TIM_Cmd(TIM2, ENABLE); //启动
					else
						TIM_Cmd(TIM2, DISABLE); //关闭时钟
					printf("Moterspeed=%0.1f\n",command_data/10.0f);
				}
				else if((command_hold[i][0] == 'A')||(command_hold[i][0] == '*'))
				{
					unsigned char A_commd[15],j;
					//******A####:设定点  A$$##:选定气体   A$$R/W###=xxx：设/读寄存器
						for(j=0;command_hold[i][j]!=0x0a&&command_hold[i][j]!=0x0d;j++)
						{
							if(j>=15)break;
							A_commd[j] = command_hold[i][j];
						}
						A_commd[j] = 0x0d;
						A_commd[j+1] = 0x0a;
						A_commd[j+2] = 0x0d;
						comSendBuf(COM6, A_commd, j+3);
				}
				else if(strcmp(command_hold[i],"zero")==0)
				{
					//Zero = command_data;
					//printf("Zer0=%f\n",Zero);
				}
				else if(strcmp(command_hold[i],"sens")==0)
				{
					//Sens = command_data;
					//printf("Sens=%f!\n",Sens);
				}
				else if(strcmp(command_hold[i],"stop")==0)
				{
					
				}
				else
				{
					printf("can not find the %s instruction!\n",command_hold[i]);
				}
				#endif
			}
	
				
			
	}
	#endif
}


//*
//*
//*
//*
void DealUsart6_Commend()
{
#if UART6_FIFO_EN == 1 
	//float command_data=0;
	//int check_code = 0;
	//char *hexdata;
	int get_command = 0;
	int i;
	char command_word[SERIAL_COMMAND_MAX_LENGTH];
	int command_num;
	char *command_hold1[SERIAL_COMMAND_HOLD_LENGTH];
	if(Uart6_command[0]==1)
	{
		//**********提出指令******
		//CPU_IntDis();
			for(i=0; i<USART6_SERIAL_COMMAND_MAX_LENGTH ;i++)
			{
				command_word[i] = Uart6_command[i+1];
				
				if((command_word[i] ==0x0d)||(command_word[i] == 0x0a))
				{
					break;
				}
			}
		
		//CPU_IntEn();
		Uart6_command[0] =0 ; 
		get_command = 1;
	}
	if(get_command == 1)
	{
		if(i>40)
		{
			command_num = gps_extract_field(command_word,command_hold1,' ',SERIAL_COMMAND_HOLD_LENGTH);
			Alic1.addr = command_hold1[0][0];
			Alic1.pressure = atof(&command_hold1[1][1]);
			Alic1.temp = atof(&command_hold1[2][1]);
			Alic1.cubage = atof(&command_hold1[3][1]);
			Alic1.quality = even_filter_float(&Alicat_filter1,atof(&command_hold1[4][1]));//滤波
			//Alic1.quality = atof(&command_hold1[4][1]);
			Alic1.set_value = atof(&command_hold1[5][1]);
			strcpy(Alic1.gas_name,command_hold1[command_num-1]);
			Alic1.new_data = 1;
		}
		else
		{
			printf("%s\n",command_word);
		}
	}
	#endif
}
//*modbus-RTU
//*
//*
//*
void DealUsart4_Commend()
{
#if UART4_FIFO_EN == 1 
	//float command_data=0;
	//int check_code = 0;
	//char *hexdata;
	int get_command = 0;
	int i;
	unsigned char command_word[USART4_SERIAL_COMMAND_MAX_LENGTH];
	//short command_num=0;
	//uint8_t command_page,command_addr=0;
	if(Uart4_command[0]==1)
	{
		//**********提出指令******
		//CPU_IntDis();
			for(i=0; i<USART4_SERIAL_COMMAND_MAX_LENGTH ;i++)
			{
				command_word[i] = Uart4_command[i+1];
			}
		

			//CPU_IntEn();
		Uart4_command[0] =0 ; 
		get_command = 1;
	}
	if(get_command == 1)
	{
		//获取地址
		if(command_word[1]==0x06)
		{
			levi_2043e.New_command_flag  = 0x06;
		}
		else if(command_word[1]==0x03)
		{
//			if(command_word[2]==0x04)
//			{
//				levi_2043e.inner = (command_word[3]<<8)|command_word[4];
//			}
			if(command_word[2]==0x02)
			{
				levi_2043e.command_status = command_word[4];
//				levi_2043e.value_num = command_word[10];
//				levi_2043e.value_status = command_word[12];
//				levi_2043e.motor_status = command_word[14];
//				levi_2043e.push_switch_status = command_word[16];
//				levi_2043e.pull_switch_status = command_word[18];
			}
			levi_2043e.New_command_flag  = 0x03;
		}
		else if(command_word[1]==0x10)
		{
			levi_2043e.New_command_flag  = 0x10;
		}
	}
	#endif
}




//float AD1,ratio=0;
u8 lin[8]={0x5A,0xA5,0x04,0x84,0x01,0x00,0x00};
//解算/处理串口指令
void DealSerial_Command(void *p_arg)
{
	(void)p_arg;
	//uint8_t SemPend_status;
	//UART1SendByte('c');
	Rs485_Init();
	static int command_sendtime=0;
	float ll;
	uint8_t read_alicat[5]={'A',0x0d,0x0a,0x0d,'\0'};
	//uint8_t alicat_init[10]={'A',0x36,0x34,0x30,0x30,0x30,0x0d,0x0a,0x0d,'\0'};//初始设定值64000 1L最大流量
	filter_init();
	bsp_InitUart();
	OSTimeDlyHMSM(0,0,1,800);
	//comSendBuf(COM2, alicat_init, 9);
	printf("MODEL_1006 V1.0 2021.1.15\n");
	while(1)
	{
		//OSSemPend(Usart4Serial_Command,0,&SemPend_status);
		//Uart4_command[0] = 0; 
		//printf("shoudao\n");
		//BluetoothCommandProcess();
		//Serial_MasterCommandProcess_NEW();
		DealUsart6_Commend();
		DealUsart4_Commend();
		DealUsart3_Commend();
		//Set_relay_onoff(0x01,data);
		DealUsart1_Commend();
		command_sendtime++;
		if(command_sendtime%50==0) //250ms
		{
			MODS_Poll();  // MODBUS上位机
		}
		if(command_sendtime%80==0)
		{
			comSendBuf(COM6,read_alicat, 4);
		}
		if(command_sendtime%200==0)
		{
			if(model_1006.status[AIR_PRESSURE]==0)
			{
				if(command_sendtime%400==0)//20201102温湿压掉线后改为两秒读一次
				get_thermo_hygro_barograph();
			}
			else
				get_thermo_hygro_barograph();
			printf("AIR=%0.3f,标=%0.3f,工=%0.3f\n",model_1006.status[SC_AIR_FLOW_RATE]/1000.0f,model_1006.status[SC_START_CONCENTRATION]/1000.0f,model_1006.status[WC_START_CONCENTRATION]/1000.0f);
			//printf("\n");
			
		}
		OSTimeDlyHMSM(0,0,0,5);
		//OSSemPend(Usart3Serial_Command,0,&SemPend_status);
		//DealUsart3_Commend();
	}
}

#else

#endif

