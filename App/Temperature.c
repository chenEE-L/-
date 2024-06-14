#include "app.h"
#include "bsp.h"
#include "usbh_bsp_msc.h"
TEMPERATURE_CONTROL Temp_70,Temp_25,Temp_10;
extern uint16_t ADC_buf[8];
void Max31865Init()
{
	//1：VBIAS有效 1:自动转换 0:未启动转换 0:两线\四线模式 00:未检测 1：清楚故障 1：50HZ抑制
	
	SPI2_Write_Max31865_1(0x80,0x83);//10000011b
	//SPI2_Write_Max31865();
	//故障上线0xfffe
	SPI2_Write_Max31865_1(0x83,0xFF);
	SPI2_Write_Max31865_1(0x84,0xFF);
	//故障下线0x0001
	SPI2_Write_Max31865_1(0x85,0x00);
	SPI2_Write_Max31865_1(0x86,0x00);
	//初始化第二个MAX31865
	SPI2_Write_Max31865_2(0x80,0x83);//10000011b
	//SPI2_Write_Max31865();
	//故障上线0xfffe
	SPI2_Write_Max31865_2(0x83,0xFF);
	SPI2_Write_Max31865_2(0x84,0xFF);
	//故障下线0x0001
	SPI2_Write_Max31865_2(0x85,0x00);
	SPI2_Write_Max31865_2(0x86,0x00);
	
	SPI2_Write_Max31865_3(0x80,0x83);//10000011b
	//SPI2_Write_Max31865();
	//故障上线0xfffe
	SPI2_Write_Max31865_3(0x83,0xFF);
	SPI2_Write_Max31865_3(0x84,0xFF);
	//故障下线0x0001
	SPI2_Write_Max31865_3(0x85,0x00);
	SPI2_Write_Max31865_3(0x86,0x00);
}
//*******************************************
//*该任务三路温度采集温度
 uint16_t R_data=0;
float TEMP_25=0,TEMP_70=0,TEMP_10,TEMP_Indev;
float TEMP=0;
//uint16_t data=0;
uint16_t Fault = 0;
void Get_Temperature(void *p_arg)
{
    (void)p_arg;
    
    SPI2_Configuration();
    Max31865Init();
    Time3_Init();
    filter_init();
    //GPIO_SetBits(GPIOE,GPIO_Pin_8);
    SPI2_Write_Max31865_1(0x80,0x01);//00000001
    OSTimeDlyHMSM(0,0,0,800);
    SPI2_Write_Max31865_1(0x80,0xE3);//11100011
	//初始化第二个MAX31865
    SPI2_Write_Max31865_2(0x80,0x01);//00000001
    OSTimeDlyHMSM(0,0,0,800);
    SPI2_Write_Max31865_2(0x80,0xE3);//11100011
	
	//初始化第三个MAX31865
    SPI2_Write_Max31865_3(0x80,0x01);//00000001
    OSTimeDlyHMSM(0,0,0,800);
    SPI2_Write_Max31865_3(0x80,0xE3);//11100011

	//初始化第四个MAX31865
    SPI2_Write_Max31865_4(0x80,0x01);//00000001
    OSTimeDlyHMSM(0,0,0,800);
    SPI2_Write_Max31865_4(0x80,0xE3);//11100011
    while(1)
    {
		
        if(GPIO_ReadInputDataBit(GPIOE,DRDY1)==0)
        {
            
            //data = 0;
            R_data = SPI2_Read_Max31865_1(0x01);
            R_data<<=8;
            R_data |= SPI2_Read_Max31865_1(0x02);
            //data = R_data&0x0001;
            R_data>>=1;
            TEMP = R_data/32.0f-256;
			if(TEMP<100)
			{
				TEMP_70 = Temperature_Filter(&Temperature_filter1,TEMP);
				if(model4001.warning[WARNING_REACTION_TEMP]==3)//如果当前状态为温度异常状态，则清除异常状态。否则不处理
					model4001.warning[WARNING_REACTION_TEMP]=0;
			}
			else
			{//温度超标，显示异常
				if(model4001.warning[WARNING_REACTION_TEMP]!=3)
					model4001.new_log |= LOG_REACTION_TEMP_ERROR;//第一次检测到异常记录日志
				model4001.warning[WARNING_REACTION_TEMP]=3;
			}
            //OSTimeDlyHMSM(0,0,0,5);
            
        }
		
		if(GPIO_ReadInputDataBit(GPIOE,DRDY2)==0)
        {
            
            //data = 0;
            R_data = SPI2_Read_Max31865_2(0x01);
            R_data<<=8;
            R_data |= SPI2_Read_Max31865_2(0x02);
            //data = R_data&0x0001;
            R_data>>=1;
            TEMP = R_data/32.0f-256;
			if(TEMP<100)
			{
				TEMP_25 = Temperature_Filter(&Temperature_filter2,TEMP);
				if(model4001.warning[WARNING_LED_TEMP]==3)//如果当前状态为温度异常状态，则清除异常状态。否则不处理
					model4001.warning[WARNING_LED_TEMP]=0;
			}
			else
			{//温度超标，显示异常
				if(model4001.warning[WARNING_LED_TEMP]!=3)
					model4001.new_log |= LOG_LED_TEMP_ERROR;//第一次检测到异常记录日志
				model4001.warning[WARNING_LED_TEMP]=3;
			}
            //OSTimeDlyHMSM(0,0,0,5);
            
        }
		if(GPIO_ReadInputDataBit(GPIOC,DRDY3)==0)
        {
            
            //data = 0;
            R_data = SPI2_Read_Max31865_3(0x01);
            R_data<<=8;
            R_data |= SPI2_Read_Max31865_3(0x02);
            //data = R_data&0x0001;
            R_data>>=1;
            TEMP = R_data/32.0f-256;
			if(TEMP<100)
			{
				TEMP_10 = Temperature_Filter(&Temperature_filter3,TEMP); 
				if(model4001.warning[WARNING_ABSORB_TEMP]==3)//如果当前状态为温度异常状态，则清除异常状态。否则不处理
					model4001.warning[WARNING_ABSORB_TEMP]=0;
			}
			else
			{	//温度超标，显示异常
				if(model4001.warning[WARNING_ABSORB_TEMP]!=3)
					model4001.new_log|=LOG_ABSORB_TEMP_ERROR;//第一次检测到异常记录日志
				model4001.warning[WARNING_ABSORB_TEMP]=3;
			}
            //OSTimeDlyHMSM(0,0,0,5);
            
        }
		if(GPIO_ReadInputDataBit(GPIOC,DRDY4)==0)
        {
            
            //data = 0;
            R_data = SPI2_Read_Max31865_4(0x01);
            R_data<<=8;
            R_data |= SPI2_Read_Max31865_4(0x02);
            //data = R_data&0x0001;
            R_data>>=1;
            TEMP = R_data/32.0f-256;
			if(TEMP<100)
			{
				TEMP_Indev = Temperature_Filter(&Temperature_filter4,TEMP); 
//				if(model4001.warning[WARNING_ABSORB_TEMP]==3)//如果当前状态为温度异常状态，则清除异常状态。否则不处理
//					model4001.warning[WARNING_ABSORB_TEMP]=0;
			}
			else
			{	//温度超标，显示异常
//				if(model4001.warning[WARNING_ABSORB_TEMP]!=3)
//					model4001.new_log|=LOG_ABSORB_TEMP_ERROR;//第一次检测到异常记录日志
//				model4001.warning[WARNING_ABSORB_TEMP]=3;
			}
            //OSTimeDlyHMSM(0,0,0,5);
            
        }
        //data++;
//        if(data>=10)
//        {
//            Fault = SPI2_Read_Max31865_1(0x07);
//            //data = 0;
//        }
        //data%=400;
        OSTimeDlyHMSM(0,0,0,5);
    }
}
//*************************************
//*温控参数初始化
void Temp_contr_init()
{
	TEMPERATURE_CONTROL *Temp;
	Temp = &Temp_70;
    Temp->Last1_Temp = 0;
    Temp->Last2_Temp = 0;
    Temp->Temp = 0;
    Temp->PWM_MAX = 2000;
    Temp->PWM_out = 0;
    Temp->Temp_Contr_P = 600;
    Temp->Temp_Contr_I = 1;
    Temp->Temp_Contr_D = 0.1;
    Temp->Temp_speed = 0;
    Temp->PWM_Out_P = 0;
    Temp->PWM_Out_I = 0;
    Temp->PWM_Out_D = 0;
	
	Temp = &Temp_25;
    Temp->Last1_Temp = 0;
    Temp->Last2_Temp = 0;
    Temp->Temp = 0;
    Temp->PWM_MAX = 2000;
    Temp->PWM_out = 0;
    Temp->Temp_Contr_P = 600;
    Temp->Temp_Contr_I = 1;
    Temp->Temp_Contr_D = 0.1;
    Temp->Temp_speed = 0;
    Temp->PWM_Out_P = 0;
    Temp->PWM_Out_I = 0;
    Temp->PWM_Out_D = 0;
		
		Temp = &Temp_10;
    Temp->Last1_Temp = 0;
    Temp->Last2_Temp = 0;
    Temp->Temp = 0;
    Temp->PWM_MAX = 2000;
    Temp->PWM_out = 0;
    Temp->Temp_Contr_P = 600;
    Temp->Temp_Contr_I = 1;
    Temp->Temp_Contr_D = 0.1;
    Temp->Temp_speed = 0;
    Temp->PWM_Out_P = 0;
    Temp->PWM_Out_I = 0;
    Temp->PWM_Out_D = 0;
}
//****************************************************
//*Get_Temperature_contrPWM:控温PID计算PWM输出函数
//*Temp:被控温度结构体参数
//*****************************************************
void Get_Temperature_contrPWM(TEMPERATURE_CONTROL *Temp)
{
	Temp->PWM_Out_P = Temp->Temp_Contr_P*Temp->Temp;//比例项
			
	if(Temp->Temp>-3&Temp->Temp<3) //启动积分

	Temp->Temp_Integral += Temp->Temp;
	if(Temp->Temp_Integral>Temp->PWM_MAX)
		Temp->Temp_Integral=Temp->PWM_MAX;
	else if(Temp->Temp_Integral<-Temp->PWM_MAX)
		Temp->Temp_Integral=-Temp->PWM_MAX;
	
	Temp->PWM_Out_I = Temp->Temp_Contr_I*Temp->Temp_Integral;//积分项
			
	if(Temp->PWM_Out_I > Temp->PWM_MAX) Temp->PWM_Out_I = Temp->PWM_MAX; //积分限幅。
			else if(Temp->PWM_Out_I < -Temp->PWM_MAX) Temp->PWM_Out_I = -Temp->PWM_MAX;
			
	if(Temp->Last1_Temp <= 0.001f&Temp->Last1_Temp>=-0.001f)//计算速率
		Temp->Temp_speed = 0;
	Temp->Temp_speed = (Temp->Temp-Temp->Last1_Temp)/100.0f;
	Temp->Last1_Temp = Temp->Temp;
	
	Temp->PWM_Out_D = Temp->Temp_Contr_I*Temp->Temp_speed;//微分项
	Temp->PWM_out = Temp->PWM_Out_P+Temp->PWM_Out_I-Temp->PWM_Out_D;
}


//**********************************************
//*控温
//*吸收室，反应室，LED荧光三路PID控温
//*TIM4,OC2、3、4三路PWM输出
OS_EVENT *Temp_Contr_time;
void Temperature_contr(void *p_arg)
{
    (void)p_arg;
	//Time2_Init();
	//GPIO_ResetBits(GPIOE,GPIO_Pin_3);//电机上电后先找0位
	//model_1006.motor_data =0;
	//Time2_freq(200);//1转每分
    while(1)
    {	
         OSTimeDlyHMSM(0,0,0,5);
    }
}
