#include "bsp.h"
#include "app.h"

uint16_t ADC_buf[8];
float voltage_buf[8];
void AD7606_Gpio_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//cs CLK RESET CONVST
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//busy data
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//busy data
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void AD7606_Init()
{
    AD7606_Gpio_Init();
    AD7606_CLK_H;
    AD7606_Reset_OFF;
    AD7606_Reset_OFF;
    AD7606_Reset_OFF;
    AD7606_Reset_ON;
    AD7606_Reset_ON;
    AD7606_Reset_ON;
    //__NOP;
    //__NOP;
    AD7606_Reset_OFF;
    AD7606_Reset_OFF;
    AD7606_Reset_OFF;
    //__NOP;
    //__NOP;
}
extern float TEMP_OUT1;
void AD7606_Read(void)
{
   u8 i,ii;
   u16 data;

   AD7606_CS_H;
   AD7606_CLK_H;
   AD7606_CONVT_ON;
   AD7606_CONVT_OFF;
   AD7606_CONVT_OFF;
   AD7606_CONVT_OFF;
   AD7606_CONVT_ON;

   //OSTimeDlyHMSM(0,0,0,1);
    //��ʱ
    for(i=0;i<100;i++)
    ii++;
    
   data=0;

   while((GPIO_ReadInputDataBit(AD7606_GPIO, AD7606_BUSY)) == 0) // �ȴ� BUSY �źű��
       {	
            AD7606_CS_L;
     for(i=0;i<8;i++)
       {		 // �ɼ�8��ͨ�����ݣ�ÿͨ��16������
        for(ii=16;ii>0;ii--)
         {
            AD7606_CLK_L;
            data <<= 1;
            data |= (GPIO_ReadInputDataBit(GPIOC, AD7606_DATA));
             //OSTimeDlyHMSM(0,0,0,1);
            AD7606_CLK_H;
          }    
	     ADC_buf[i]=data;
	    }
       AD7606_CLK_H;
   break;
	}
	
	AD7606_CS_H;
	
	AD_Voltage();
	//printf("Temperature=%0.3f,AD=%f,%d\n",TEMP_OUT1,ADC_buf[7]>32768? 5*(ADC_buf[7]-65535)/32768.0:5*(ADC_buf[7]/32768.0),ADC_buf[7]);
}

void AD_Voltage(void)
{
	float voltage;
	static char Zero_count=0;
	voltage = (ADC_buf[1]>32768? AD7606_VOLTAGE*(ADC_buf[1]-65535)/32768.0l:AD7606_VOLTAGE*(ADC_buf[1]/32768.0l));
	
	if(voltage>4.9f&&voltage<5.1f)//��ѹΪ5Vʱ����Ϊ��
	{
		if(Zero_count<101)
		Zero_count++;
	}
	else
		Zero_count = 0;
	if(Zero_count<3||Zero_count>100)//100msһ�����ݣ����ʱ�䣩��С�����飨0.3�룩���ݿ������䶯����������ݲ�����100��10��С��10����Ϊ������������ʮ������Ϊ��
	voltage_buf[6] = even_filter_float(&AD_filter6,voltage);
	
	voltage = (ADC_buf[3]>32768? AD7606_VOLTAGE*(ADC_buf[3]-65535)/32768.0l:AD7606_VOLTAGE*(ADC_buf[3]/32768.0l));
	voltage_buf[7] = even_filter_float(&AD_filter7,voltage);
}

