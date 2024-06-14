#include "app.h"
#include "bsp.h"
#include "usbh_bsp_msc.h"
TEMPERATURE_CONTROL Temp_70,Temp_25,Temp_10;
extern uint16_t ADC_buf[8];
void Max31865Init()
{
	//1��VBIAS��Ч 1:�Զ�ת�� 0:δ����ת�� 0:����\����ģʽ 00:δ��� 1��������� 1��50HZ����
	
	SPI2_Write_Max31865_1(0x80,0x83);//10000011b
	//SPI2_Write_Max31865();
	//��������0xfffe
	SPI2_Write_Max31865_1(0x83,0xFF);
	SPI2_Write_Max31865_1(0x84,0xFF);
	//��������0x0001
	SPI2_Write_Max31865_1(0x85,0x00);
	SPI2_Write_Max31865_1(0x86,0x00);
	//��ʼ���ڶ���MAX31865
	SPI2_Write_Max31865_2(0x80,0x83);//10000011b
	//SPI2_Write_Max31865();
	//��������0xfffe
	SPI2_Write_Max31865_2(0x83,0xFF);
	SPI2_Write_Max31865_2(0x84,0xFF);
	//��������0x0001
	SPI2_Write_Max31865_2(0x85,0x00);
	SPI2_Write_Max31865_2(0x86,0x00);
	
	SPI2_Write_Max31865_3(0x80,0x83);//10000011b
	//SPI2_Write_Max31865();
	//��������0xfffe
	SPI2_Write_Max31865_3(0x83,0xFF);
	SPI2_Write_Max31865_3(0x84,0xFF);
	//��������0x0001
	SPI2_Write_Max31865_3(0x85,0x00);
	SPI2_Write_Max31865_3(0x86,0x00);
}
//*******************************************
//*��������·�¶Ȳɼ��¶�
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
	//��ʼ���ڶ���MAX31865
    SPI2_Write_Max31865_2(0x80,0x01);//00000001
    OSTimeDlyHMSM(0,0,0,800);
    SPI2_Write_Max31865_2(0x80,0xE3);//11100011
	
	//��ʼ��������MAX31865
    SPI2_Write_Max31865_3(0x80,0x01);//00000001
    OSTimeDlyHMSM(0,0,0,800);
    SPI2_Write_Max31865_3(0x80,0xE3);//11100011

	//��ʼ�����ĸ�MAX31865
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
				if(model4001.warning[WARNING_REACTION_TEMP]==3)//�����ǰ״̬Ϊ�¶��쳣״̬��������쳣״̬�����򲻴���
					model4001.warning[WARNING_REACTION_TEMP]=0;
			}
			else
			{//�¶ȳ��꣬��ʾ�쳣
				if(model4001.warning[WARNING_REACTION_TEMP]!=3)
					model4001.new_log |= LOG_REACTION_TEMP_ERROR;//��һ�μ�⵽�쳣��¼��־
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
				if(model4001.warning[WARNING_LED_TEMP]==3)//�����ǰ״̬Ϊ�¶��쳣״̬��������쳣״̬�����򲻴���
					model4001.warning[WARNING_LED_TEMP]=0;
			}
			else
			{//�¶ȳ��꣬��ʾ�쳣
				if(model4001.warning[WARNING_LED_TEMP]!=3)
					model4001.new_log |= LOG_LED_TEMP_ERROR;//��һ�μ�⵽�쳣��¼��־
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
				if(model4001.warning[WARNING_ABSORB_TEMP]==3)//�����ǰ״̬Ϊ�¶��쳣״̬��������쳣״̬�����򲻴���
					model4001.warning[WARNING_ABSORB_TEMP]=0;
			}
			else
			{	//�¶ȳ��꣬��ʾ�쳣
				if(model4001.warning[WARNING_ABSORB_TEMP]!=3)
					model4001.new_log|=LOG_ABSORB_TEMP_ERROR;//��һ�μ�⵽�쳣��¼��־
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
//				if(model4001.warning[WARNING_ABSORB_TEMP]==3)//�����ǰ״̬Ϊ�¶��쳣״̬��������쳣״̬�����򲻴���
//					model4001.warning[WARNING_ABSORB_TEMP]=0;
			}
			else
			{	//�¶ȳ��꣬��ʾ�쳣
//				if(model4001.warning[WARNING_ABSORB_TEMP]!=3)
//					model4001.new_log|=LOG_ABSORB_TEMP_ERROR;//��һ�μ�⵽�쳣��¼��־
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
//*�¿ز�����ʼ��
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
//*Get_Temperature_contrPWM:����PID����PWM�������
//*Temp:�����¶Ƚṹ�����
//*****************************************************
void Get_Temperature_contrPWM(TEMPERATURE_CONTROL *Temp)
{
	Temp->PWM_Out_P = Temp->Temp_Contr_P*Temp->Temp;//������
			
	if(Temp->Temp>-3&Temp->Temp<3) //��������

	Temp->Temp_Integral += Temp->Temp;
	if(Temp->Temp_Integral>Temp->PWM_MAX)
		Temp->Temp_Integral=Temp->PWM_MAX;
	else if(Temp->Temp_Integral<-Temp->PWM_MAX)
		Temp->Temp_Integral=-Temp->PWM_MAX;
	
	Temp->PWM_Out_I = Temp->Temp_Contr_I*Temp->Temp_Integral;//������
			
	if(Temp->PWM_Out_I > Temp->PWM_MAX) Temp->PWM_Out_I = Temp->PWM_MAX; //�����޷���
			else if(Temp->PWM_Out_I < -Temp->PWM_MAX) Temp->PWM_Out_I = -Temp->PWM_MAX;
			
	if(Temp->Last1_Temp <= 0.001f&Temp->Last1_Temp>=-0.001f)//��������
		Temp->Temp_speed = 0;
	Temp->Temp_speed = (Temp->Temp-Temp->Last1_Temp)/100.0f;
	Temp->Last1_Temp = Temp->Temp;
	
	Temp->PWM_Out_D = Temp->Temp_Contr_I*Temp->Temp_speed;//΢����
	Temp->PWM_out = Temp->PWM_Out_P+Temp->PWM_Out_I-Temp->PWM_Out_D;
}


//**********************************************
//*����
//*�����ң���Ӧ�ң�LEDӫ����·PID����
//*TIM4,OC2��3��4��·PWM���
OS_EVENT *Temp_Contr_time;
void Temperature_contr(void *p_arg)
{
    (void)p_arg;
	//Time2_Init();
	//GPIO_ResetBits(GPIOE,GPIO_Pin_3);//����ϵ������0λ
	//model_1006.motor_data =0;
	//Time2_freq(200);//1תÿ��
    while(1)
    {	
         OSTimeDlyHMSM(0,0,0,5);
    }
}
