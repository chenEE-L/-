#include "app.h"
#include "bsp.h"
#include "math.h"

#include "usbh_bsp_msc.h"
#include "beionfluid_s60.h"

MODEL_4001 model4001;
MODEL_1006 model_1006;
uint8_t gettime[7] = {0x5A,0xA5,0x03,0x81,0x20,0x07};
extern float AD1,HCHO,Sens,Zero;
//short home_page_time[6]={0};
//short home_page_lin_length=0;
char auto_time_day[6]={0};//前三个内存未使用

//static char calibration_num = 0; //将 x=0 y=0 直接加入
//static float fluidflow_tem[5];//临时存放当前溶液流量
//static float ratio_tem[5];//临时存放比例  实际浓度/当前浓度
/*--------------------------------------------------------------------
内部函数：拉格朗日插值（Polynomial interpolation）
-----------------------------------------------------------------------
多项式公式：f(x) = a(0) + a(1)*x + a(2)*(x^2) + ... + a(n) * (x^n)
-----------------------------------------------------------------------
输入参数：
			x_signal				x的序列信号
			y_signal				y的序列信号
			num						信号长度。备注：x和y的元素个数必须相等。
			t						指定插值点
-----------------------------------------------------------------------*/
//float Lagrange_Interpolation( float t)
//{
//	float z_value = 0.0; 		//插值点的拟合值
//	if (model_1006.calibration_num <= 0) {return 1;} //没校准系数为1
//	else if (model_1006.calibration_num == 1) { z_value = model_1006.ratio_tem[0]; }
//	else if (model_1006.calibration_num == 2) { z_value = (model_1006.ratio_tem[0] * (t - model_1006.fluidflow_tem[1]) - model_1006.ratio_tem[1] * (t - model_1006.fluidflow_tem[0])) / (model_1006.fluidflow_tem[0] - model_1006.fluidflow_tem[1]); }
//	else if (model_1006.calibration_num >= 3)
//	{
//		//利用拉格朗日插值公式，计算在t点对应的f(t)。
//		for (int ii = 0; ii <= model_1006.calibration_num; ii++)
//		{
//			float s_base = 1.0;		//s是拉格朗日基函数
//			for (int jj = 0; jj <= model_1006.calibration_num; jj++)
//				if (jj != ii)			//去掉xi与xj相等的情况，分母为0没意义。
//					s_base = s_base * (t - model_1006.fluidflow_tem[jj]) / (model_1006.fluidflow_tem[ii] - model_1006.fluidflow_tem[jj]);		//拉格朗日插值公式
//			z_value = z_value + s_base * model_1006.ratio_tem[ii];		//（基函数 * y）的累加计算，得到该x想的拉格朗日函数。
//		}
//	}
//	return z_value;
//}
static short FLUID_FLOW_B; //校准前液体流速
static void GETKB(float *x, float *y, char num)
{
//	if (num <= 0)
//	{
//		for(int i =0; i< 5; i++)
//		{
//			model_1006.CONCALI_K[i] = 0;
//			model_1006.CONCALI_B[i] = 1.0; 
//		}
//	}
//	else
//	{
		model_1006.CONCALI_K[0] = 0;
		model_1006.CONCALI_B[0] = y[0]; // 第一组数之前使用第一组的Y值
		for(int j =1; j< num; j++)  
		{
			model_1006.CONCALI_K[j] = (y[j]-y[j-1])/(x[j]-x[j-1]);
			model_1006.CONCALI_B[j] = y[j] - model_1006.CONCALI_K[j]*x[j]; 
		}
//	}

}

static float linearCalibration(float t)
{
	float afterdealvalue;
	if (model_1006.calibration_num <= 0)
	{
		afterdealvalue =  1.0; //没校准默认1.0系数
	}
	else
	{
		if(t < model_1006.fluidflow_tem[0])
		{
			afterdealvalue = model_1006.CONCALI_K[0]*t +	model_1006.CONCALI_B[0];
		}
		else if(t > model_1006.fluidflow_tem[model_1006.calibration_num-1])
		{
			afterdealvalue = model_1006.CONCALI_K[model_1006.calibration_num-1]*t +	model_1006.CONCALI_B[model_1006.calibration_num-1];
		}
		else
		{
			for(int i =0; i<(model_1006.calibration_num-1); i++)
			{
				
				if((t >= model_1006.fluidflow_tem[i]) && (t <= model_1006.fluidflow_tem[i+1]))
				{
					afterdealvalue = model_1006.CONCALI_K[i+1]*t +	model_1006.CONCALI_B[i+1];
					break;
				}

		  }
		}
	}
	return afterdealvalue;
	
}

void DugsCommandSend(uint16_t addr,uint16_t command_data)
{
	uint8_t str_buf[10];
	uint16_t check_code ;	
	str_buf[0] = 0x5A; 
	////发送俯仰角信息		
	str_buf[1] = 0xA5;
	str_buf[2] = 5;
	str_buf[3] = 0x82;
	str_buf[4] = addr>>8;
	str_buf[5] = addr&0xff;
	str_buf[6] = command_data>>8;
	str_buf[7] = command_data&0xff;
	check_code = 0;
	//crc
	check_code  = check_code &0xff;
	
	
	comSendBuf(COM1, str_buf, 8);
	
}
//*******************
//*函数：DugsText_Display 串口屏文本显示
//*addr：文本在触屏显示位置地址
//*text：显示的文本指针
//*length：显示的文本长度
void DugsText_Display(uint16_t addr,char text[],uint16_t length)
{
	uint8_t str_buf[length+7];//帧头2+数据长度1+	指令码1+地址2+length+1结束符‘\0’
	uint16_t check_code ,i=0;	
	str_buf[0] = 0x5A; 
	////发送俯仰角信息		
	str_buf[1] = 0xA5;
	str_buf[2] = 3+length;//地址2+指令码1+数据长度length
	str_buf[3] = 0x82;
	str_buf[4] = addr>>8;
	str_buf[5] = addr&0xff;
	for(i=0;i<length;i++)
	str_buf[i+6] = text[i];
	check_code = 0;
	//crc
	check_code  = check_code &0xff;
	
	
	comSendBuf(COM1, str_buf, str_buf[2]+3);
}

void Dugs_init()
{
	//流量校准系数
	model_1006.calibration[PUMP_CALIBRATION_FACTOR] = 1000;//注射泵比例
	model_1006.calibration[ACTUAL_FLOW_RATE] = 100; //流量计比例
	model_1006.calibration[TEMP_K] = 1000;
	model_1006.calibration[TEMP_B] = 0;
	model_1006.calibration[HUMIDITY_K] = 1000;
	model_1006.calibration[HUMIDITY_B] = 0;
	model_1006.calibration[PRESSURE_K] = 1000;
	model_1006.calibration[PRESSURE_B] = 0;
//	model_1006.set[SLOPE_COEFFICIENTS] = 1000;//斜率系数，补偿吸附原因导致的损失
	model_1006.start_product[HAND_CLEAN_ML] = 5000;//默认清洗体积5ml
	model_1006.start_product[PUMP_SPEED_SET] = 2000;//默认吸流量
//	model_1006.calibration_num = 0;//
	
}

void Dugs_send_config()
{
	model_1006.new_data = 0;
	model_1006.start_product[START_PRODUCT_COMMEND] = 0;//指令清零
	model_1006.calibration[CALIBRATION_COMMEND] = 0;//指令清零
	model_1006.start_product[START_INJECTION_COMMEND] = 0;//指令清零
	model_1006.home_page[HOME_WORK_STATUS] = 0;//首页显示状态清零
	model_1006.start_product[FLUID_FLOW] = 0;//液体流量清零
	model_1006.start_product[HAND_CLEAN_STATUS] = 0x00; //
	model_1006.start_product[AUTO_CLEAN_STATUS] = 0x00; //清洗页显示清除
	for(int i=0;i<=WARNING_ALL;i++)
	{
		model_1006.warning[i] = 0;
	}
	
}

//数据刷新显示
void Dugs(void *p_arg)
{
	(void) p_arg;
	char i=0;
	Dugs_init();
	//SPI3_Configuration();
	while(1)
	{
		///*******************更新电磁阀的状态
		if(GPIO_ReadOutputDataBit(GPIOC , GPIO_Pin_0))
			model_1006.status[RADIOTUBE_STATUS] = 1;
		else
			model_1006.status[RADIOTUBE_STATUS] = 0;
		
		if(GPIO_ReadOutputDataBit(GPIOC , GPIO_Pin_1))
			model_1006.status[ATOMIZER_SWITCH] = 1;
		else
			model_1006.status[ATOMIZER_SWITCH] = 0;
		//**************注射泵状态***************
		if(levi_2043e.command_status==0)
			model_1006.start_product[INJECTION_PUMP_STATUS] = 0;//停止
//		else	
//		{
////			if(levi_2043e.value_num == 0x02)
////				model_1006.start_product[INJECTION_PUMP_STATUS] = 2;//排液
////			else
////				model_1006.start_product[INJECTION_PUMP_STATUS] = 1;//吸液
//			model_1006.start_product[INJECTION_PUMP_STATUS] = levi_2043e.value_num;
//		}
		
		//*************警告提示计算*******
		if(model_1006.status[FLUID_FLOW_STATUS]>200)//大于0.2ml/min报警
			model_1006.warning[FLUID_FLOW_STATUS] = 1;
		else 
			model_1006.warning[FLUID_FLOW_STATUS] = 0;
		if(model_1006.start_product[AIR_FLOW_RATE_SET]>0)
		{
			if(model_1006.status[SC_AIR_FLOW_RATE]*100000/(model_1006.start_product[AIR_FLOW_RATE_SET]*1000)>105)//大于5%报警
				model_1006.warning[SC_AIR_FLOW_RATE] = 1;
			else if(model_1006.status[SC_AIR_FLOW_RATE]*100000/(model_1006.start_product[AIR_FLOW_RATE_SET]*1000)<95)//小于5%报警
				model_1006.warning[SC_AIR_FLOW_RATE] = 2;
			else
				model_1006.warning[SC_AIR_FLOW_RATE] = 0;
		}
		if(model_1006.start_product[SC_CONCENTRATION]>0)
		{
			if(model_1006.status[SC_START_CONCENTRATION]*100/model_1006.start_product[SC_CONCENTRATION]>105)//大于5%报警
			{
				model_1006.warning[SC_START_CONCENTRATION] = 1;//标况和工况相同
				model_1006.warning[WC_START_CONCENTRATION] = 1;
			}
			else if(model_1006.status[SC_START_CONCENTRATION]*100/model_1006.start_product[SC_CONCENTRATION]<95)//小于5%报警
			{
				model_1006.warning[SC_START_CONCENTRATION] = 2;
				model_1006.warning[WC_START_CONCENTRATION] = 2;
			}
			else
			{
				model_1006.warning[SC_START_CONCENTRATION] = 0;
				model_1006.warning[WC_START_CONCENTRATION] = 0;
			}
		}
		if(model_1006.status[MODEL_1006_TEMP]>4500)//大于45℃报警
			model_1006.warning[MODEL_1006_TEMP] = 1;
		else if(model_1006.status[MODEL_1006_TEMP]<500)//小于5℃报警
			model_1006.warning[MODEL_1006_TEMP] = 2;
		else
			model_1006.warning[MODEL_1006_TEMP] = 0;
		
		if(model_1006.status[MODEL_1006_RH]>5000)//湿度大于50%报警
			model_1006.warning[MODEL_1006_RH] = 1;
		else
			model_1006.warning[MODEL_1006_RH] = 0;
		
		if(model_1006.status[AIR_PRESSURE]>11000)//大于110KPa报警
			model_1006.warning[AIR_PRESSURE] = 1;
		else if(model_1006.status[AIR_PRESSURE]<9000)//小于90KPa报警
			model_1006.warning[AIR_PRESSURE] = 2;
		else
			model_1006.warning[AIR_PRESSURE] = 0;
			
		model_1006.warning[WARNING_ALL] = 0;
		for(i=0;i<WARNING_ALL;i++)
		{
			if(model_1006.home_page[HOME_WORK_STATUS] == 0x02)//只在发生状态时检测报警，不发生时清除所有报警 2021125
				model_1006.warning[WARNING_ALL] += model_1006.warning[i];
			else 
				model_1006.warning[i] = 0;
			OSTimeDlyHMSM(0,0,0,10);
		}
		if(model_1006.warning[WARNING_ALL]>0)
			model_1006.warning[WARNING_ALL] = 1;
		//********************显示更新**************
		for(i=0;i<=AIR_PRESSURE;i++)
		{
			DugsCommandSend(MODEL_1006_STATUS<<4|i,model_1006.status[i]);
			OSTimeDlyHMSM(0,0,0,10);
		}
			
		for(i=0;i<=Actual_operating_concentration;i++)
		{
			DugsCommandSend(HOME_PAGE<<4|i,model_1006.home_page[i]);
			OSTimeDlyHMSM(0,0,0,10);
		}
		for(i=0;i<=CALI_CON_COMMEND1;i++)  //增加多点校准
		{
			DugsCommandSend(SET_PAGE<<4|i,model_1006.set[i]);
			OSTimeDlyHMSM(0,0,0,10);
		}
		for(i=0;i<=AUTO_CLEAN_STATUS;i++)
		{
			DugsCommandSend(START_PRODUCT_PAGE<<4|i,model_1006.start_product[i]);
			OSTimeDlyHMSM(0,0,0,10);
		}
		for(i=0;i<=CALIBRATION_COMMEND;i++)
		{
			DugsCommandSend(CALIBRATION_PAGE<<4|i,model_1006.calibration[i]);
			OSTimeDlyHMSM(0,0,0,10);
		}
		for(i=0;i<=WARNING_ALL;i++)
		{
			DugsCommandSend(WARNING<<4|i,model_1006.warning[i]);
			OSTimeDlyHMSM(0,0,0,10);
		}
		
		for(i=0;i<=DATA_EXPORT_PERCENRT;i++)
		{
			DugsCommandSend(DEMAND_PAGE<<4|i,model_1006.data_demand[i]);
			OSTimeDlyHMSM(0,0,0,10);
		}
		///计算浓度
		if(model_1006.status[SC_AIR_FLOW_RATE]!=0)
			model_1006.status[SC_START_CONCENTRATION] = model_1006.start_product[FLUID_CONCENTRATION]*model_1006.status[FLUID_FLOW_STATUS]
		*1000/model_1006.status[SC_AIR_FLOW_RATE]/linearCalibration(FLUID_FLOW_B);//（/100/1000/1000）/（100）//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
		else 
			model_1006.status[SC_START_CONCENTRATION] = 0;
		
		if(model_1006.status[MODEL_1006_TEMP]!=0)
			model_1006.status[WC_START_CONCENTRATION] = model_1006.status[SC_START_CONCENTRATION]*model_1006.status[AIR_PRESSURE]*273.15f/
		(101.325f*(model_1006.status[MODEL_1006_TEMP]*1.0f+27315));
		else
			model_1006.status[WC_START_CONCENTRATION] = 0;
		
		model_1006.home_page[1] = model_1006.start_product[FLUID_FLOW];
		model_1006.status[FLUID_FLOW_STATUS]= model_1006.start_product[FLUID_FLOW];
		model_1006.home_page[2] = model_1006.status[FLUID_FLOW_STATUS];
		model_1006.home_page[3] = model_1006.start_product[AIR_FLOW_RATE_SET];
		model_1006.home_page[4] = model_1006.status[SC_AIR_FLOW_RATE];
		model_1006.home_page[5] = model_1006.start_product[SC_CONCENTRATION];
		model_1006.home_page[6] = model_1006.status[SC_START_CONCENTRATION];
		model_1006.home_page[7] = model_1006.start_product[WC_CONCENTRATION];
		model_1006.home_page[8] = model_1006.status[WC_START_CONCENTRATION];
		
		model_1006.status[SC_AIR_FLOW_RATE] = Alic1.quality*model_1006.calibration[ACTUAL_FLOW_RATE];//50L的流量计
		if(model_1006.status[AIR_PRESSURE]!=0)
		model_1006.status[WC_AIR_FLOW_RATE] = model_1006.status[SC_AIR_FLOW_RATE]*101.325f*(model_1006.status[MODEL_1006_TEMP]*1.0f+27315)/
		(model_1006.status[AIR_PRESSURE]*273.15f);
		
		if(model_1006.status[MODEL_1006_TEMP]!=0)
		model_1006.start_product[WC_CONCENTRATION] =  model_1006.start_product[SC_CONCENTRATION]*model_1006.status[AIR_PRESSURE]*273.15f/
		(101.325f*(model_1006.status[MODEL_1006_TEMP]*1.0f+27315));
		
		model_1006.data_demand[DATA_EXPORT_EN] = USB_OTG_Core.host.ConnSts;//U盘状态
		//model_1006.status[SC_START_CONCENTRATION] = model_1006.start_product[SC_CONCENTRATION];//
		comSendBuf(COM1,gettime, 6);//获取时间
		OSTimeDlyHMSM(0,0,0,200);
	}
}
//执行操作
void auto_hand_action(void *p_arg)
{
	(void)p_arg;
	IO_init();
	//u32 time=0;//,time_length=0;
	uint8_t alicat_init[10]={'A',0x36,0x34,0x30,0x30,0x30,0x0d,0x0a,0x0d,'\0'};//初始设定值64000 1L最大流量
//	int calinum_tem = 0;
	int flow_rate;
//	u8 auto_injection_count;//自动清洗计数
// 	char next_start_hourtime=0;//下次发生的时间（洗液时间）
	RADIOTUBE_OFF;//关闭电磁阀
	OSTimeDlyHMSM(0,0,2,0);//开机延时两秒等待配置参数
	while(1)
	{

		//*****************************粒子发生*************************************
		if(model_1006.start_product[START_PRODUCT_COMMEND]&0x08)//开始发生
		{
			if(model_1006.start_product[FLUID_CONCENTRATION]!=0)
			FLUID_FLOW_B = model_1006.start_product[AIR_FLOW_RATE_SET]*model_1006.start_product[SC_CONCENTRATION]/
				(model_1006.start_product[FLUID_CONCENTRATION]*1000);//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
			model_1006.start_product[FLUID_FLOW] = FLUID_FLOW_B* linearCalibration(FLUID_FLOW_B); 
			ATOMIZER_ON;//雾化器上电
			OSTimeDlyHMSM(0,0,2,0);
			flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*6.4f*1000/5/model_1006.calibration[ACTUAL_FLOW_RATE];//20201010流量设定是增加校准比率 20201228增加斜率系数
			//flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//设气体流量
			LED1_OFF;
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//开始发生
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x08);//标志位清零
			
			model_1006.home_page[HOME_WORK_STATUS] = 0x02;//首页显示发生中
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新
		}
		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x10)//停止发生
		{
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x10);
			model_1006.start_product[FLUID_FLOW] = 0;//液体流量清零
			//RADIOTUBE_OFF;
			ATOMIZER_OFF;//关闭雾化器
			alicat_init[1] = 0x30;
			alicat_init[2] = 0x30;
			alicat_init[3] = 0x30;
			alicat_init[4] = 0x30;
			alicat_init[5] = 0x30;
			comSendBuf(COM6, alicat_init, 9);//关闭气体流量
			
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x04;//关闭排液
			model_1006.home_page[HOME_WORK_STATUS] = 0x00;//首页显示待机
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新
		}
//		else if(model_1006.home_page[HOME_WORK_STATUS] == 0x02&&model_1006.start_product[START_PRODUCT_COMMEND]==0)
//		{
//			if(levi_2043e.command_status==0)
//			{
//				if(levi_2043e.value_num == 0x02)
//				{
//					model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//吸液
//					OSTimeDlyHMSM(0,0,0,200);
//					RADIOTUBE_OFF;//关闭电磁阀
//					levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新
//				}
//				else
//				{
//					model_1006.start_product[START_PRODUCT_COMMEND] |= 0x0100;//排液
//					RADIOTUBE_OFF;//关闭电磁阀
//					
//					//******************20201013增加自动吸液功能
//					//计算并标记下次吸液时间 以小时为单位计， time = （当前分钟数+ 可注射分钟数）/60分钟
//					next_start_hourtime = (model_1006.time[TIME_MIN] + 5000/model_1006.start_product[FLUID_FLOW])/60;
//					
//					OSTimeDlyHMSM(0,0,0,200);
//					levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新  
//					
//				}
//			}
//			//***********************20201013增加自动吸液功能 next_start_hourtime ******************
//			else if(model_1006.time[TIME_MIN] ==0 && model_1006.time[TIME_SEC]==0)
//			{
//				
//				if(next_start_hourtime<=1)//大于等于洗液标记时间则开始吸液
//				{
//					model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//吸液
//					OSTimeDlyHMSM(0,0,0,200);
//					RADIOTUBE_OFF;//关闭电磁阀
//					levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新
//				}
//				else
//				{
//					next_start_hourtime--;
//					OSTimeDlyHMSM(0,0,1,100);//延时一秒+，防止重复减时间
//				}
//			}
//			//******************************************************************************************
//		}
		if(Alic1.set_value == 0)
			LED1_ON;
		else
			LED1_OFF;
		//**************************************浓度校准*******************************
		//10
		if(model_1006.set[CALI_CON_COMMEND]&0x01)//0x04流量计校准启动流量计
		{
			model_1006.set[CALI_CON_COMMEND]&= ~0x01;//清除指令
			//20201010流量设定是增加校准比率
			if(model_1006.set[CALI_CON_FLUID]!=0)
			FLUID_FLOW_B = model_1006.set[CAIL_CON_SETFLOW]*model_1006.set[SET_CON1]/
				(model_1006.set[CALI_CON_FLUID]*1000);//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
			model_1006.start_product[FLUID_FLOW] = FLUID_FLOW_B* linearCalibration(FLUID_FLOW_B); 
			//赋值到粒子发生参数,以便显示和报警判定
			model_1006.start_product[AIR_FLOW_RATE_SET] = model_1006.set[CAIL_CON_SETFLOW];
			model_1006.start_product[FLUID_CONCENTRATION] = model_1006.set[CALI_CON_FLUID];
			model_1006.start_product[SC_CONCENTRATION] = model_1006.set[SET_CON1];
			
			ATOMIZER_ON;//雾化器上电
			OSTimeDlyHMSM(0,0,2,0);
			flow_rate = model_1006.set[CAIL_CON_SETFLOW]*6.4f*1000/5/model_1006.calibration[ACTUAL_FLOW_RATE];//20201010流量设定是增加校准比率 20201228增加斜率系数
			//flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//设气体流量
			LED1_OFF;
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//吸液
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x08);//标志位清零
			
			model_1006.home_page[HOME_WORK_STATUS] = 0x02;//首页显示发生中
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新
		}
		else if(model_1006.set[CALI_CON_COMMEND1]&0x01)//10L流量校准参数保存
		{
			model_1006.set[CALI_CON_COMMEND1]&= ~0x01;//清除指令
			model_1006.fluidflow_tem[0] = model_1006.start_product[FLUID_FLOW];
			model_1006.ratio_tem[0] = (float)model_1006.set[ACTUAL_CON1]/(float)model_1006.status[SC_START_CONCENTRATION];
		}
		//20
		if(model_1006.set[CALI_CON_COMMEND]&0x02)//0x04流量计校准启动流量计
		{
			model_1006.set[CALI_CON_COMMEND]&= ~0x02;//清除指令
			//20201010流量设定是增加校准比率
			if(model_1006.set[CALI_CON_FLUID]!=0)
			FLUID_FLOW_B = model_1006.set[CAIL_CON_SETFLOW]*model_1006.set[SET_CON2]/
				(model_1006.set[CALI_CON_FLUID]*1000);//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
			model_1006.start_product[FLUID_FLOW] = FLUID_FLOW_B* linearCalibration(FLUID_FLOW_B);
			//赋值到粒子发生参数,以便显示和报警判定
			model_1006.start_product[AIR_FLOW_RATE_SET] = model_1006.set[CAIL_CON_SETFLOW];
			model_1006.start_product[FLUID_CONCENTRATION] = model_1006.set[CALI_CON_FLUID];
			model_1006.start_product[SC_CONCENTRATION] = model_1006.set[SET_CON2];			
			ATOMIZER_ON;//雾化器上电
			OSTimeDlyHMSM(0,0,2,0);
			//flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
			flow_rate = model_1006.set[CAIL_CON_SETFLOW]*6.4f*1000/5/model_1006.calibration[ACTUAL_FLOW_RATE];//20201010流量设定是增加校准比率 20201228增加斜率系数
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//设气体流量
			LED1_OFF;
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//吸液
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x08);//标志位清零
			
			model_1006.home_page[HOME_WORK_STATUS] = 0x02;//首页显示发生中
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新
		}
		else if(model_1006.set[CALI_CON_COMMEND1]&0x02)//10L流量校准参数保存
		{
			model_1006.set[CALI_CON_COMMEND1]&= ~0x02;//清除指令
			model_1006.fluidflow_tem[1] = model_1006.start_product[FLUID_FLOW];
			model_1006.ratio_tem[1] = (float)model_1006.set[ACTUAL_CON2]/(float)model_1006.status[SC_START_CONCENTRATION];
		}
		//30
		if(model_1006.set[CALI_CON_COMMEND]&0x04)//0x04流量计校准启动流量计
		{
			model_1006.set[CALI_CON_COMMEND]&= ~0x04;//清除指令
			//20201010流量设定是增加校准比率
			if(model_1006.set[CALI_CON_FLUID]!=0)
			FLUID_FLOW_B = model_1006.set[CAIL_CON_SETFLOW]*model_1006.set[SET_CON3]/
				(model_1006.set[CALI_CON_FLUID]*1000);//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
			model_1006.start_product[FLUID_FLOW] = FLUID_FLOW_B* linearCalibration(FLUID_FLOW_B); 
			//赋值到粒子发生参数,以便显示和报警判定
			model_1006.start_product[AIR_FLOW_RATE_SET] = model_1006.set[CAIL_CON_SETFLOW];
			model_1006.start_product[FLUID_CONCENTRATION] = model_1006.set[CALI_CON_FLUID];
			model_1006.start_product[SC_CONCENTRATION] = model_1006.set[SET_CON3];
			ATOMIZER_ON;//雾化器上电
			OSTimeDlyHMSM(0,0,2,0);
			//flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
			flow_rate = model_1006.set[CAIL_CON_SETFLOW]*6.4f*1000/5/model_1006.calibration[ACTUAL_FLOW_RATE];//20201010流量设定是增加校准比率 20201228增加斜率系数
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//设气体流量
			LED1_OFF;
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//吸液
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x08);//标志位清零
			
			model_1006.home_page[HOME_WORK_STATUS] = 0x02;//首页显示发生中
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新
		}
		else if(model_1006.set[CALI_CON_COMMEND1]&0x04)//10L流量校准参数保存
		{
			model_1006.set[CALI_CON_COMMEND1]&= ~0x04;//清除指令
			model_1006.fluidflow_tem[2] = model_1006.start_product[FLUID_FLOW];
			model_1006.ratio_tem[2] = (float)model_1006.set[ACTUAL_CON3]/(float)model_1006.status[SC_START_CONCENTRATION];
		}
		//40
		if(model_1006.set[CALI_CON_COMMEND]&0x08)//0x04流量计校准启动流量计
		{
			model_1006.set[CALI_CON_COMMEND]&= ~0x08;//清除指令
			//20201010流量设定是增加校准比率
			if(model_1006.set[CALI_CON_FLUID]!=0)
			FLUID_FLOW_B = model_1006.set[CAIL_CON_SETFLOW]*model_1006.set[SET_CON4]/
				(model_1006.set[CALI_CON_FLUID]*1000);//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
			model_1006.start_product[FLUID_FLOW] = FLUID_FLOW_B* linearCalibration(FLUID_FLOW_B); 
			//赋值到粒子发生参数,以便显示和报警判定
			model_1006.start_product[AIR_FLOW_RATE_SET] = model_1006.set[CAIL_CON_SETFLOW];
			model_1006.start_product[FLUID_CONCENTRATION] = model_1006.set[CALI_CON_FLUID];
			model_1006.start_product[SC_CONCENTRATION] = model_1006.set[SET_CON4];
			ATOMIZER_ON;//雾化器上电
			OSTimeDlyHMSM(0,0,2,0);
			//flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
			flow_rate = model_1006.set[CAIL_CON_SETFLOW]*6.4f*1000/5/model_1006.calibration[ACTUAL_FLOW_RATE];//20201010流量设定是增加校准比率 20201228增加斜率系数
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//设气体流量
			LED1_OFF;
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//吸液
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x08);//标志位清零
			
			model_1006.home_page[HOME_WORK_STATUS] = 0x02;//首页显示发生中
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新
		}
		else if(model_1006.set[CALI_CON_COMMEND1]&0x08)//10L流量校准参数保存
		{
			model_1006.set[CALI_CON_COMMEND1]&= ~0x08;//清除指令
			model_1006.fluidflow_tem[3] = model_1006.start_product[FLUID_FLOW];
			model_1006.ratio_tem[3] = (float)model_1006.set[ACTUAL_CON4]/(float)model_1006.status[SC_START_CONCENTRATION];
		}
		//50
		if(model_1006.set[CALI_CON_COMMEND]&0x10)//0x04流量计校准启动流量计
		{
			model_1006.set[CALI_CON_COMMEND]&= ~0x10;//清除指令
			//20201010流量设定是增加校准比率
			if(model_1006.set[CALI_CON_FLUID]!=0)
			FLUID_FLOW_B = model_1006.set[CAIL_CON_SETFLOW]*model_1006.set[SET_CON5]/
				(model_1006.set[CALI_CON_FLUID]*1000);//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
			model_1006.start_product[FLUID_FLOW] = FLUID_FLOW_B* linearCalibration(FLUID_FLOW_B);
			//赋值到粒子发生参数,以便显示和报警判定
			model_1006.start_product[AIR_FLOW_RATE_SET] = model_1006.set[CAIL_CON_SETFLOW];
			model_1006.start_product[FLUID_CONCENTRATION] = model_1006.set[CALI_CON_FLUID];
			model_1006.start_product[SC_CONCENTRATION] = model_1006.set[SET_CON5];			
			ATOMIZER_ON;//雾化器上电
			OSTimeDlyHMSM(0,0,2,0);
			//flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
			flow_rate = model_1006.set[CAIL_CON_SETFLOW]*6.4f*1000/5/model_1006.calibration[ACTUAL_FLOW_RATE];//20201010流量设定是增加校准比率 20201228增加斜率系数
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//设气体流量
			LED1_OFF;
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//吸液
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x08);//标志位清零
			
			model_1006.home_page[HOME_WORK_STATUS] = 0x02;//首页显示发生中
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新
		}
		else if(model_1006.set[CALI_CON_COMMEND1]&0x10)//10L流量校准参数保存
		{
			model_1006.set[CALI_CON_COMMEND1]&= ~0x10;//清除指令
			model_1006.fluidflow_tem[4] = model_1006.start_product[FLUID_FLOW];
			model_1006.ratio_tem[4] = (float)model_1006.set[ACTUAL_CON5]/(float)model_1006.status[SC_START_CONCENTRATION];
		}
		//开始校准
		if(model_1006.set[CALI_CON_COMMEND]&0x20)//0x08流量计校准开始校准
		{
			model_1006.set[CALI_CON_COMMEND]&= ~0x20;//清除指令
//			if(calinum_tem) //不等于0说明上方有确认数据
//			{
//				model_1006.calibration_num = calinum_tem; //根据数量判定是否校准

//			}
//			calinum_tem = 0;
			model_1006.calibration_num = 0; //序列数量清零
			for(int j = 0; j<5; j++)  //重新排序放到序列里
			{
				if((model_1006.set[SET_CON1+j] != 0)&&(model_1006.set[ACTUAL_CON1+j] != 0))
				{
						model_1006.fluidflow_tem[model_1006.calibration_num] = model_1006.fluidflow_tem[j];
						model_1006.ratio_tem[model_1006.calibration_num] = model_1006.ratio_tem[j];
						model_1006.calibration_num++;
				}
			}
			GETKB(model_1006.fluidflow_tem, model_1006.ratio_tem, model_1006.calibration_num);
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x10;//校准完成后停止发生

//			//model_1006.calibration[ACTUAL_FLOW_RATE] = model_1006.calibration[ACTUAL_FLOW]*model_1006.calibration[ACTUAL_FLOW_RATE]/model_1006.status[SC_AIR_FLOW_RATE];
//			model_1006.calibration[CALIBRATION_FLOW_K1] = model_1006.calibration[ACTUAL_FLOW1]/flow_tem[0];//20201010，修改流量计校准方法
//			if(model_1006.calibration[CALIBRATION_FLOW_K1]<0)
//				model_1006.calibration[CALIBRATION_FLOW_K1] = 1000;
		}
		//**************************************流量计校准*******************************
		if(model_1006.calibration[CALIBRATION_COMMEND]&0x04)//0x04流量计校准启动流量计
		{
			model_1006.calibration[CALIBRATION_COMMEND]&= ~0x04;//清除指令
			//20201010流量设定是增加校准比率
			flow_rate = model_1006.calibration[SET_FLOW]*6400/5/model_1006.calibration[ACTUAL_FLOW_RATE];//*64000*1000/5000/model_1006.calibration[ACTUAL_FLOW_RATE];
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//设气体流量
		}
		else if(model_1006.calibration[CALIBRATION_COMMEND]&0x08)//0x08流量计校准开始校准
		{
			model_1006.calibration[CALIBRATION_COMMEND]&= ~0x08;//清除指令
			//model_1006.calibration[ACTUAL_FLOW_RATE] = model_1006.calibration[ACTUAL_FLOW]*model_1006.calibration[ACTUAL_FLOW_RATE]/model_1006.status[SC_AIR_FLOW_RATE];
			model_1006.calibration[ACTUAL_FLOW_RATE] = model_1006.calibration[ACTUAL_FLOW]/Alic1.quality;//20201010，修改流量计校准方法
			if(model_1006.calibration[ACTUAL_FLOW_RATE]<0)
				model_1006.calibration[ACTUAL_FLOW_RATE] = 1000;
		}
		//*****************************注射泵校准************************************
		if(model_1006.calibration[CALIBRATION_COMMEND]&0x01)//0x01注射泵校准启动注射泵
		{
			model_1006.calibration[CALIBRATION_COMMEND]&= ~0x01;//清除指令
			model_1006.home_page[HOME_WORK_STATUS] = 0x01;//首页显示等待发生
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//吸液
			levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新
		}
		else if(model_1006.home_page[HOME_WORK_STATUS]==0x01&&model_1006.start_product[START_PRODUCT_COMMEND]==0)//0x01注射泵校准校准过程计时
		{
			if(levi_2043e.command_status==0)
			{
				model_1006.start_product[START_PRODUCT_COMMEND] |= 0x0200;//排液
				RADIOTUBE_OFF;//关闭电磁阀
				OSTimeDlyHMSM(0,0,0,200);
				levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新 
				model_1006.home_page[HOME_WORK_STATUS] &=0x00;//	清除首页显示等待发生		
			}
		}
		else if(model_1006.calibration[CALIBRATION_COMMEND]&0x02)//0x08注射泵校准开始校准
		{
			model_1006.calibration[CALIBRATION_COMMEND]&= ~0x02;//清除指令
			model_1006.calibration[PUMP_CALIBRATION_FACTOR] = model_1006.calibration[PUMP_BULK_ML]*1000/model_1006.calibration[FLOW_WEIGHT];
		}
		OSTimeDlyHMSM(0,0,0,10);
	}
}

