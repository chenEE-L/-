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
char auto_time_day[6]={0};//ǰ�����ڴ�δʹ��

//static char calibration_num = 0; //�� x=0 y=0 ֱ�Ӽ���
//static float fluidflow_tem[5];//��ʱ��ŵ�ǰ��Һ����
//static float ratio_tem[5];//��ʱ��ű���  ʵ��Ũ��/��ǰŨ��
/*--------------------------------------------------------------------
�ڲ��������������ղ�ֵ��Polynomial interpolation��
-----------------------------------------------------------------------
����ʽ��ʽ��f(x) = a(0) + a(1)*x + a(2)*(x^2) + ... + a(n) * (x^n)
-----------------------------------------------------------------------
���������
			x_signal				x�������ź�
			y_signal				y�������ź�
			num						�źų��ȡ���ע��x��y��Ԫ�ظ���������ȡ�
			t						ָ����ֵ��
-----------------------------------------------------------------------*/
//float Lagrange_Interpolation( float t)
//{
//	float z_value = 0.0; 		//��ֵ������ֵ
//	if (model_1006.calibration_num <= 0) {return 1;} //ûУ׼ϵ��Ϊ1
//	else if (model_1006.calibration_num == 1) { z_value = model_1006.ratio_tem[0]; }
//	else if (model_1006.calibration_num == 2) { z_value = (model_1006.ratio_tem[0] * (t - model_1006.fluidflow_tem[1]) - model_1006.ratio_tem[1] * (t - model_1006.fluidflow_tem[0])) / (model_1006.fluidflow_tem[0] - model_1006.fluidflow_tem[1]); }
//	else if (model_1006.calibration_num >= 3)
//	{
//		//�����������ղ�ֵ��ʽ��������t���Ӧ��f(t)��
//		for (int ii = 0; ii <= model_1006.calibration_num; ii++)
//		{
//			float s_base = 1.0;		//s���������ջ�����
//			for (int jj = 0; jj <= model_1006.calibration_num; jj++)
//				if (jj != ii)			//ȥ��xi��xj��ȵ��������ĸΪ0û���塣
//					s_base = s_base * (t - model_1006.fluidflow_tem[jj]) / (model_1006.fluidflow_tem[ii] - model_1006.fluidflow_tem[jj]);		//�������ղ�ֵ��ʽ
//			z_value = z_value + s_base * model_1006.ratio_tem[ii];		//�������� * y�����ۼӼ��㣬�õ���x����������պ�����
//		}
//	}
//	return z_value;
//}
static short FLUID_FLOW_B; //У׼ǰҺ������
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
		model_1006.CONCALI_B[0] = y[0]; // ��һ����֮ǰʹ�õ�һ���Yֵ
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
		afterdealvalue =  1.0; //ûУ׼Ĭ��1.0ϵ��
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
	////���͸�������Ϣ		
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
//*������DugsText_Display �������ı���ʾ
//*addr���ı��ڴ�����ʾλ�õ�ַ
//*text����ʾ���ı�ָ��
//*length����ʾ���ı�����
void DugsText_Display(uint16_t addr,char text[],uint16_t length)
{
	uint8_t str_buf[length+7];//֡ͷ2+���ݳ���1+	ָ����1+��ַ2+length+1��������\0��
	uint16_t check_code ,i=0;	
	str_buf[0] = 0x5A; 
	////���͸�������Ϣ		
	str_buf[1] = 0xA5;
	str_buf[2] = 3+length;//��ַ2+ָ����1+���ݳ���length
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
	//����У׼ϵ��
	model_1006.calibration[PUMP_CALIBRATION_FACTOR] = 1000;//ע��ñ���
	model_1006.calibration[ACTUAL_FLOW_RATE] = 100; //�����Ʊ���
	model_1006.calibration[TEMP_K] = 1000;
	model_1006.calibration[TEMP_B] = 0;
	model_1006.calibration[HUMIDITY_K] = 1000;
	model_1006.calibration[HUMIDITY_B] = 0;
	model_1006.calibration[PRESSURE_K] = 1000;
	model_1006.calibration[PRESSURE_B] = 0;
//	model_1006.set[SLOPE_COEFFICIENTS] = 1000;//б��ϵ������������ԭ���µ���ʧ
	model_1006.start_product[HAND_CLEAN_ML] = 5000;//Ĭ����ϴ���5ml
	model_1006.start_product[PUMP_SPEED_SET] = 2000;//Ĭ��������
//	model_1006.calibration_num = 0;//
	
}

void Dugs_send_config()
{
	model_1006.new_data = 0;
	model_1006.start_product[START_PRODUCT_COMMEND] = 0;//ָ������
	model_1006.calibration[CALIBRATION_COMMEND] = 0;//ָ������
	model_1006.start_product[START_INJECTION_COMMEND] = 0;//ָ������
	model_1006.home_page[HOME_WORK_STATUS] = 0;//��ҳ��ʾ״̬����
	model_1006.start_product[FLUID_FLOW] = 0;//Һ����������
	model_1006.start_product[HAND_CLEAN_STATUS] = 0x00; //
	model_1006.start_product[AUTO_CLEAN_STATUS] = 0x00; //��ϴҳ��ʾ���
	for(int i=0;i<=WARNING_ALL;i++)
	{
		model_1006.warning[i] = 0;
	}
	
}

//����ˢ����ʾ
void Dugs(void *p_arg)
{
	(void) p_arg;
	char i=0;
	Dugs_init();
	//SPI3_Configuration();
	while(1)
	{
		///*******************���µ�ŷ���״̬
		if(GPIO_ReadOutputDataBit(GPIOC , GPIO_Pin_0))
			model_1006.status[RADIOTUBE_STATUS] = 1;
		else
			model_1006.status[RADIOTUBE_STATUS] = 0;
		
		if(GPIO_ReadOutputDataBit(GPIOC , GPIO_Pin_1))
			model_1006.status[ATOMIZER_SWITCH] = 1;
		else
			model_1006.status[ATOMIZER_SWITCH] = 0;
		//**************ע���״̬***************
		if(levi_2043e.command_status==0)
			model_1006.start_product[INJECTION_PUMP_STATUS] = 0;//ֹͣ
//		else	
//		{
////			if(levi_2043e.value_num == 0x02)
////				model_1006.start_product[INJECTION_PUMP_STATUS] = 2;//��Һ
////			else
////				model_1006.start_product[INJECTION_PUMP_STATUS] = 1;//��Һ
//			model_1006.start_product[INJECTION_PUMP_STATUS] = levi_2043e.value_num;
//		}
		
		//*************������ʾ����*******
		if(model_1006.status[FLUID_FLOW_STATUS]>200)//����0.2ml/min����
			model_1006.warning[FLUID_FLOW_STATUS] = 1;
		else 
			model_1006.warning[FLUID_FLOW_STATUS] = 0;
		if(model_1006.start_product[AIR_FLOW_RATE_SET]>0)
		{
			if(model_1006.status[SC_AIR_FLOW_RATE]*100000/(model_1006.start_product[AIR_FLOW_RATE_SET]*1000)>105)//����5%����
				model_1006.warning[SC_AIR_FLOW_RATE] = 1;
			else if(model_1006.status[SC_AIR_FLOW_RATE]*100000/(model_1006.start_product[AIR_FLOW_RATE_SET]*1000)<95)//С��5%����
				model_1006.warning[SC_AIR_FLOW_RATE] = 2;
			else
				model_1006.warning[SC_AIR_FLOW_RATE] = 0;
		}
		if(model_1006.start_product[SC_CONCENTRATION]>0)
		{
			if(model_1006.status[SC_START_CONCENTRATION]*100/model_1006.start_product[SC_CONCENTRATION]>105)//����5%����
			{
				model_1006.warning[SC_START_CONCENTRATION] = 1;//����͹�����ͬ
				model_1006.warning[WC_START_CONCENTRATION] = 1;
			}
			else if(model_1006.status[SC_START_CONCENTRATION]*100/model_1006.start_product[SC_CONCENTRATION]<95)//С��5%����
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
		if(model_1006.status[MODEL_1006_TEMP]>4500)//����45�汨��
			model_1006.warning[MODEL_1006_TEMP] = 1;
		else if(model_1006.status[MODEL_1006_TEMP]<500)//С��5�汨��
			model_1006.warning[MODEL_1006_TEMP] = 2;
		else
			model_1006.warning[MODEL_1006_TEMP] = 0;
		
		if(model_1006.status[MODEL_1006_RH]>5000)//ʪ�ȴ���50%����
			model_1006.warning[MODEL_1006_RH] = 1;
		else
			model_1006.warning[MODEL_1006_RH] = 0;
		
		if(model_1006.status[AIR_PRESSURE]>11000)//����110KPa����
			model_1006.warning[AIR_PRESSURE] = 1;
		else if(model_1006.status[AIR_PRESSURE]<9000)//С��90KPa����
			model_1006.warning[AIR_PRESSURE] = 2;
		else
			model_1006.warning[AIR_PRESSURE] = 0;
			
		model_1006.warning[WARNING_ALL] = 0;
		for(i=0;i<WARNING_ALL;i++)
		{
			if(model_1006.home_page[HOME_WORK_STATUS] == 0x02)//ֻ�ڷ���״̬ʱ��ⱨ����������ʱ������б��� 2021125
				model_1006.warning[WARNING_ALL] += model_1006.warning[i];
			else 
				model_1006.warning[i] = 0;
			OSTimeDlyHMSM(0,0,0,10);
		}
		if(model_1006.warning[WARNING_ALL]>0)
			model_1006.warning[WARNING_ALL] = 1;
		//********************��ʾ����**************
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
		for(i=0;i<=CALI_CON_COMMEND1;i++)  //���Ӷ��У׼
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
		///����Ũ��
		if(model_1006.status[SC_AIR_FLOW_RATE]!=0)
			model_1006.status[SC_START_CONCENTRATION] = model_1006.start_product[FLUID_CONCENTRATION]*model_1006.status[FLUID_FLOW_STATUS]
		*1000/model_1006.status[SC_AIR_FLOW_RATE]/linearCalibration(FLUID_FLOW_B);//��/100/1000/1000��/��100��//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
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
		
		model_1006.status[SC_AIR_FLOW_RATE] = Alic1.quality*model_1006.calibration[ACTUAL_FLOW_RATE];//50L��������
		if(model_1006.status[AIR_PRESSURE]!=0)
		model_1006.status[WC_AIR_FLOW_RATE] = model_1006.status[SC_AIR_FLOW_RATE]*101.325f*(model_1006.status[MODEL_1006_TEMP]*1.0f+27315)/
		(model_1006.status[AIR_PRESSURE]*273.15f);
		
		if(model_1006.status[MODEL_1006_TEMP]!=0)
		model_1006.start_product[WC_CONCENTRATION] =  model_1006.start_product[SC_CONCENTRATION]*model_1006.status[AIR_PRESSURE]*273.15f/
		(101.325f*(model_1006.status[MODEL_1006_TEMP]*1.0f+27315));
		
		model_1006.data_demand[DATA_EXPORT_EN] = USB_OTG_Core.host.ConnSts;//U��״̬
		//model_1006.status[SC_START_CONCENTRATION] = model_1006.start_product[SC_CONCENTRATION];//
		comSendBuf(COM1,gettime, 6);//��ȡʱ��
		OSTimeDlyHMSM(0,0,0,200);
	}
}
//ִ�в���
void auto_hand_action(void *p_arg)
{
	(void)p_arg;
	IO_init();
	//u32 time=0;//,time_length=0;
	uint8_t alicat_init[10]={'A',0x36,0x34,0x30,0x30,0x30,0x0d,0x0a,0x0d,'\0'};//��ʼ�趨ֵ64000 1L�������
//	int calinum_tem = 0;
	int flow_rate;
//	u8 auto_injection_count;//�Զ���ϴ����
// 	char next_start_hourtime=0;//�´η�����ʱ�䣨ϴҺʱ�䣩
	RADIOTUBE_OFF;//�رյ�ŷ�
	OSTimeDlyHMSM(0,0,2,0);//������ʱ����ȴ����ò���
	while(1)
	{

		//*****************************���ӷ���*************************************
		if(model_1006.start_product[START_PRODUCT_COMMEND]&0x08)//��ʼ����
		{
			if(model_1006.start_product[FLUID_CONCENTRATION]!=0)
			FLUID_FLOW_B = model_1006.start_product[AIR_FLOW_RATE_SET]*model_1006.start_product[SC_CONCENTRATION]/
				(model_1006.start_product[FLUID_CONCENTRATION]*1000);//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
			model_1006.start_product[FLUID_FLOW] = FLUID_FLOW_B* linearCalibration(FLUID_FLOW_B); 
			ATOMIZER_ON;//�����ϵ�
			OSTimeDlyHMSM(0,0,2,0);
			flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*6.4f*1000/5/model_1006.calibration[ACTUAL_FLOW_RATE];//20201010�����趨������У׼���� 20201228����б��ϵ��
			//flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//����������
			LED1_OFF;
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//��ʼ����
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x08);//��־λ����
			
			model_1006.home_page[HOME_WORK_STATUS] = 0x02;//��ҳ��ʾ������
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��
		}
		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x10)//ֹͣ����
		{
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x10);
			model_1006.start_product[FLUID_FLOW] = 0;//Һ����������
			//RADIOTUBE_OFF;
			ATOMIZER_OFF;//�ر�����
			alicat_init[1] = 0x30;
			alicat_init[2] = 0x30;
			alicat_init[3] = 0x30;
			alicat_init[4] = 0x30;
			alicat_init[5] = 0x30;
			comSendBuf(COM6, alicat_init, 9);//�ر���������
			
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x04;//�ر���Һ
			model_1006.home_page[HOME_WORK_STATUS] = 0x00;//��ҳ��ʾ����
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��
		}
//		else if(model_1006.home_page[HOME_WORK_STATUS] == 0x02&&model_1006.start_product[START_PRODUCT_COMMEND]==0)
//		{
//			if(levi_2043e.command_status==0)
//			{
//				if(levi_2043e.value_num == 0x02)
//				{
//					model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//��Һ
//					OSTimeDlyHMSM(0,0,0,200);
//					RADIOTUBE_OFF;//�رյ�ŷ�
//					levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��
//				}
//				else
//				{
//					model_1006.start_product[START_PRODUCT_COMMEND] |= 0x0100;//��Һ
//					RADIOTUBE_OFF;//�رյ�ŷ�
//					
//					//******************20201013�����Զ���Һ����
//					//���㲢����´���Һʱ�� ��СʱΪ��λ�ƣ� time = ����ǰ������+ ��ע���������/60����
//					next_start_hourtime = (model_1006.time[TIME_MIN] + 5000/model_1006.start_product[FLUID_FLOW])/60;
//					
//					OSTimeDlyHMSM(0,0,0,200);
//					levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��  
//					
//				}
//			}
//			//***********************20201013�����Զ���Һ���� next_start_hourtime ******************
//			else if(model_1006.time[TIME_MIN] ==0 && model_1006.time[TIME_SEC]==0)
//			{
//				
//				if(next_start_hourtime<=1)//���ڵ���ϴҺ���ʱ����ʼ��Һ
//				{
//					model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//��Һ
//					OSTimeDlyHMSM(0,0,0,200);
//					RADIOTUBE_OFF;//�رյ�ŷ�
//					levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��
//				}
//				else
//				{
//					next_start_hourtime--;
//					OSTimeDlyHMSM(0,0,1,100);//��ʱһ��+����ֹ�ظ���ʱ��
//				}
//			}
//			//******************************************************************************************
//		}
		if(Alic1.set_value == 0)
			LED1_ON;
		else
			LED1_OFF;
		//**************************************Ũ��У׼*******************************
		//10
		if(model_1006.set[CALI_CON_COMMEND]&0x01)//0x04������У׼����������
		{
			model_1006.set[CALI_CON_COMMEND]&= ~0x01;//���ָ��
			//20201010�����趨������У׼����
			if(model_1006.set[CALI_CON_FLUID]!=0)
			FLUID_FLOW_B = model_1006.set[CAIL_CON_SETFLOW]*model_1006.set[SET_CON1]/
				(model_1006.set[CALI_CON_FLUID]*1000);//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
			model_1006.start_product[FLUID_FLOW] = FLUID_FLOW_B* linearCalibration(FLUID_FLOW_B); 
			//��ֵ�����ӷ�������,�Ա���ʾ�ͱ����ж�
			model_1006.start_product[AIR_FLOW_RATE_SET] = model_1006.set[CAIL_CON_SETFLOW];
			model_1006.start_product[FLUID_CONCENTRATION] = model_1006.set[CALI_CON_FLUID];
			model_1006.start_product[SC_CONCENTRATION] = model_1006.set[SET_CON1];
			
			ATOMIZER_ON;//�����ϵ�
			OSTimeDlyHMSM(0,0,2,0);
			flow_rate = model_1006.set[CAIL_CON_SETFLOW]*6.4f*1000/5/model_1006.calibration[ACTUAL_FLOW_RATE];//20201010�����趨������У׼���� 20201228����б��ϵ��
			//flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//����������
			LED1_OFF;
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//��Һ
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x08);//��־λ����
			
			model_1006.home_page[HOME_WORK_STATUS] = 0x02;//��ҳ��ʾ������
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��
		}
		else if(model_1006.set[CALI_CON_COMMEND1]&0x01)//10L����У׼��������
		{
			model_1006.set[CALI_CON_COMMEND1]&= ~0x01;//���ָ��
			model_1006.fluidflow_tem[0] = model_1006.start_product[FLUID_FLOW];
			model_1006.ratio_tem[0] = (float)model_1006.set[ACTUAL_CON1]/(float)model_1006.status[SC_START_CONCENTRATION];
		}
		//20
		if(model_1006.set[CALI_CON_COMMEND]&0x02)//0x04������У׼����������
		{
			model_1006.set[CALI_CON_COMMEND]&= ~0x02;//���ָ��
			//20201010�����趨������У׼����
			if(model_1006.set[CALI_CON_FLUID]!=0)
			FLUID_FLOW_B = model_1006.set[CAIL_CON_SETFLOW]*model_1006.set[SET_CON2]/
				(model_1006.set[CALI_CON_FLUID]*1000);//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
			model_1006.start_product[FLUID_FLOW] = FLUID_FLOW_B* linearCalibration(FLUID_FLOW_B);
			//��ֵ�����ӷ�������,�Ա���ʾ�ͱ����ж�
			model_1006.start_product[AIR_FLOW_RATE_SET] = model_1006.set[CAIL_CON_SETFLOW];
			model_1006.start_product[FLUID_CONCENTRATION] = model_1006.set[CALI_CON_FLUID];
			model_1006.start_product[SC_CONCENTRATION] = model_1006.set[SET_CON2];			
			ATOMIZER_ON;//�����ϵ�
			OSTimeDlyHMSM(0,0,2,0);
			//flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
			flow_rate = model_1006.set[CAIL_CON_SETFLOW]*6.4f*1000/5/model_1006.calibration[ACTUAL_FLOW_RATE];//20201010�����趨������У׼���� 20201228����б��ϵ��
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//����������
			LED1_OFF;
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//��Һ
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x08);//��־λ����
			
			model_1006.home_page[HOME_WORK_STATUS] = 0x02;//��ҳ��ʾ������
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��
		}
		else if(model_1006.set[CALI_CON_COMMEND1]&0x02)//10L����У׼��������
		{
			model_1006.set[CALI_CON_COMMEND1]&= ~0x02;//���ָ��
			model_1006.fluidflow_tem[1] = model_1006.start_product[FLUID_FLOW];
			model_1006.ratio_tem[1] = (float)model_1006.set[ACTUAL_CON2]/(float)model_1006.status[SC_START_CONCENTRATION];
		}
		//30
		if(model_1006.set[CALI_CON_COMMEND]&0x04)//0x04������У׼����������
		{
			model_1006.set[CALI_CON_COMMEND]&= ~0x04;//���ָ��
			//20201010�����趨������У׼����
			if(model_1006.set[CALI_CON_FLUID]!=0)
			FLUID_FLOW_B = model_1006.set[CAIL_CON_SETFLOW]*model_1006.set[SET_CON3]/
				(model_1006.set[CALI_CON_FLUID]*1000);//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
			model_1006.start_product[FLUID_FLOW] = FLUID_FLOW_B* linearCalibration(FLUID_FLOW_B); 
			//��ֵ�����ӷ�������,�Ա���ʾ�ͱ����ж�
			model_1006.start_product[AIR_FLOW_RATE_SET] = model_1006.set[CAIL_CON_SETFLOW];
			model_1006.start_product[FLUID_CONCENTRATION] = model_1006.set[CALI_CON_FLUID];
			model_1006.start_product[SC_CONCENTRATION] = model_1006.set[SET_CON3];
			ATOMIZER_ON;//�����ϵ�
			OSTimeDlyHMSM(0,0,2,0);
			//flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
			flow_rate = model_1006.set[CAIL_CON_SETFLOW]*6.4f*1000/5/model_1006.calibration[ACTUAL_FLOW_RATE];//20201010�����趨������У׼���� 20201228����б��ϵ��
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//����������
			LED1_OFF;
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//��Һ
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x08);//��־λ����
			
			model_1006.home_page[HOME_WORK_STATUS] = 0x02;//��ҳ��ʾ������
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��
		}
		else if(model_1006.set[CALI_CON_COMMEND1]&0x04)//10L����У׼��������
		{
			model_1006.set[CALI_CON_COMMEND1]&= ~0x04;//���ָ��
			model_1006.fluidflow_tem[2] = model_1006.start_product[FLUID_FLOW];
			model_1006.ratio_tem[2] = (float)model_1006.set[ACTUAL_CON3]/(float)model_1006.status[SC_START_CONCENTRATION];
		}
		//40
		if(model_1006.set[CALI_CON_COMMEND]&0x08)//0x04������У׼����������
		{
			model_1006.set[CALI_CON_COMMEND]&= ~0x08;//���ָ��
			//20201010�����趨������У׼����
			if(model_1006.set[CALI_CON_FLUID]!=0)
			FLUID_FLOW_B = model_1006.set[CAIL_CON_SETFLOW]*model_1006.set[SET_CON4]/
				(model_1006.set[CALI_CON_FLUID]*1000);//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
			model_1006.start_product[FLUID_FLOW] = FLUID_FLOW_B* linearCalibration(FLUID_FLOW_B); 
			//��ֵ�����ӷ�������,�Ա���ʾ�ͱ����ж�
			model_1006.start_product[AIR_FLOW_RATE_SET] = model_1006.set[CAIL_CON_SETFLOW];
			model_1006.start_product[FLUID_CONCENTRATION] = model_1006.set[CALI_CON_FLUID];
			model_1006.start_product[SC_CONCENTRATION] = model_1006.set[SET_CON4];
			ATOMIZER_ON;//�����ϵ�
			OSTimeDlyHMSM(0,0,2,0);
			//flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
			flow_rate = model_1006.set[CAIL_CON_SETFLOW]*6.4f*1000/5/model_1006.calibration[ACTUAL_FLOW_RATE];//20201010�����趨������У׼���� 20201228����б��ϵ��
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//����������
			LED1_OFF;
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//��Һ
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x08);//��־λ����
			
			model_1006.home_page[HOME_WORK_STATUS] = 0x02;//��ҳ��ʾ������
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��
		}
		else if(model_1006.set[CALI_CON_COMMEND1]&0x08)//10L����У׼��������
		{
			model_1006.set[CALI_CON_COMMEND1]&= ~0x08;//���ָ��
			model_1006.fluidflow_tem[3] = model_1006.start_product[FLUID_FLOW];
			model_1006.ratio_tem[3] = (float)model_1006.set[ACTUAL_CON4]/(float)model_1006.status[SC_START_CONCENTRATION];
		}
		//50
		if(model_1006.set[CALI_CON_COMMEND]&0x10)//0x04������У׼����������
		{
			model_1006.set[CALI_CON_COMMEND]&= ~0x10;//���ָ��
			//20201010�����趨������У׼����
			if(model_1006.set[CALI_CON_FLUID]!=0)
			FLUID_FLOW_B = model_1006.set[CAIL_CON_SETFLOW]*model_1006.set[SET_CON5]/
				(model_1006.set[CALI_CON_FLUID]*1000);//*model_1006.calibration[PUMP_CALIBRATION_FACTOR]
			model_1006.start_product[FLUID_FLOW] = FLUID_FLOW_B* linearCalibration(FLUID_FLOW_B);
			//��ֵ�����ӷ�������,�Ա���ʾ�ͱ����ж�
			model_1006.start_product[AIR_FLOW_RATE_SET] = model_1006.set[CAIL_CON_SETFLOW];
			model_1006.start_product[FLUID_CONCENTRATION] = model_1006.set[CALI_CON_FLUID];
			model_1006.start_product[SC_CONCENTRATION] = model_1006.set[SET_CON5];			
			ATOMIZER_ON;//�����ϵ�
			OSTimeDlyHMSM(0,0,2,0);
			//flow_rate = model_1006.start_product[AIR_FLOW_RATE_SET]*64000/5000;
			flow_rate = model_1006.set[CAIL_CON_SETFLOW]*6.4f*1000/5/model_1006.calibration[ACTUAL_FLOW_RATE];//20201010�����趨������У׼���� 20201228����б��ϵ��
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//����������
			LED1_OFF;
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//��Һ
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~(0x08);//��־λ����
			
			model_1006.home_page[HOME_WORK_STATUS] = 0x02;//��ҳ��ʾ������
			OSTimeDlyHMSM(0,0,0,200);
			levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��
		}
		else if(model_1006.set[CALI_CON_COMMEND1]&0x10)//10L����У׼��������
		{
			model_1006.set[CALI_CON_COMMEND1]&= ~0x10;//���ָ��
			model_1006.fluidflow_tem[4] = model_1006.start_product[FLUID_FLOW];
			model_1006.ratio_tem[4] = (float)model_1006.set[ACTUAL_CON5]/(float)model_1006.status[SC_START_CONCENTRATION];
		}
		//��ʼУ׼
		if(model_1006.set[CALI_CON_COMMEND]&0x20)//0x08������У׼��ʼУ׼
		{
			model_1006.set[CALI_CON_COMMEND]&= ~0x20;//���ָ��
//			if(calinum_tem) //������0˵���Ϸ���ȷ������
//			{
//				model_1006.calibration_num = calinum_tem; //���������ж��Ƿ�У׼

//			}
//			calinum_tem = 0;
			model_1006.calibration_num = 0; //������������
			for(int j = 0; j<5; j++)  //��������ŵ�������
			{
				if((model_1006.set[SET_CON1+j] != 0)&&(model_1006.set[ACTUAL_CON1+j] != 0))
				{
						model_1006.fluidflow_tem[model_1006.calibration_num] = model_1006.fluidflow_tem[j];
						model_1006.ratio_tem[model_1006.calibration_num] = model_1006.ratio_tem[j];
						model_1006.calibration_num++;
				}
			}
			GETKB(model_1006.fluidflow_tem, model_1006.ratio_tem, model_1006.calibration_num);
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x10;//У׼��ɺ�ֹͣ����

//			//model_1006.calibration[ACTUAL_FLOW_RATE] = model_1006.calibration[ACTUAL_FLOW]*model_1006.calibration[ACTUAL_FLOW_RATE]/model_1006.status[SC_AIR_FLOW_RATE];
//			model_1006.calibration[CALIBRATION_FLOW_K1] = model_1006.calibration[ACTUAL_FLOW1]/flow_tem[0];//20201010���޸�������У׼����
//			if(model_1006.calibration[CALIBRATION_FLOW_K1]<0)
//				model_1006.calibration[CALIBRATION_FLOW_K1] = 1000;
		}
		//**************************************������У׼*******************************
		if(model_1006.calibration[CALIBRATION_COMMEND]&0x04)//0x04������У׼����������
		{
			model_1006.calibration[CALIBRATION_COMMEND]&= ~0x04;//���ָ��
			//20201010�����趨������У׼����
			flow_rate = model_1006.calibration[SET_FLOW]*6400/5/model_1006.calibration[ACTUAL_FLOW_RATE];//*64000*1000/5000/model_1006.calibration[ACTUAL_FLOW_RATE];
			alicat_init[1] = flow_rate/10000+0x30;
			alicat_init[2] = flow_rate/1000%10+0x30;
			alicat_init[3] = flow_rate/100%10+0x30;
			alicat_init[4] = flow_rate/10%10+0x30;
			alicat_init[5] = flow_rate%10+0x30;
			comSendBuf(COM6, alicat_init, 9);//����������
		}
		else if(model_1006.calibration[CALIBRATION_COMMEND]&0x08)//0x08������У׼��ʼУ׼
		{
			model_1006.calibration[CALIBRATION_COMMEND]&= ~0x08;//���ָ��
			//model_1006.calibration[ACTUAL_FLOW_RATE] = model_1006.calibration[ACTUAL_FLOW]*model_1006.calibration[ACTUAL_FLOW_RATE]/model_1006.status[SC_AIR_FLOW_RATE];
			model_1006.calibration[ACTUAL_FLOW_RATE] = model_1006.calibration[ACTUAL_FLOW]/Alic1.quality;//20201010���޸�������У׼����
			if(model_1006.calibration[ACTUAL_FLOW_RATE]<0)
				model_1006.calibration[ACTUAL_FLOW_RATE] = 1000;
		}
		//*****************************ע���У׼************************************
		if(model_1006.calibration[CALIBRATION_COMMEND]&0x01)//0x01ע���У׼����ע���
		{
			model_1006.calibration[CALIBRATION_COMMEND]&= ~0x01;//���ָ��
			model_1006.home_page[HOME_WORK_STATUS] = 0x01;//��ҳ��ʾ�ȴ�����
			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x02;//��Һ
			levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��
		}
		else if(model_1006.home_page[HOME_WORK_STATUS]==0x01&&model_1006.start_product[START_PRODUCT_COMMEND]==0)//0x01ע���У׼У׼���̼�ʱ
		{
			if(levi_2043e.command_status==0)
			{
				model_1006.start_product[START_PRODUCT_COMMEND] |= 0x0200;//��Һ
				RADIOTUBE_OFF;//�رյ�ŷ�
				OSTimeDlyHMSM(0,0,0,200);
				levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ�� 
				model_1006.home_page[HOME_WORK_STATUS] &=0x00;//	�����ҳ��ʾ�ȴ�����		
			}
		}
		else if(model_1006.calibration[CALIBRATION_COMMEND]&0x02)//0x08ע���У׼��ʼУ׼
		{
			model_1006.calibration[CALIBRATION_COMMEND]&= ~0x02;//���ָ��
			model_1006.calibration[PUMP_CALIBRATION_FACTOR] = model_1006.calibration[PUMP_BULK_ML]*1000/model_1006.calibration[FLOW_WEIGHT];
		}
		OSTimeDlyHMSM(0,0,0,10);
	}
}

