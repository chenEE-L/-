#ifndef _AD_7606_
#define _AD_7606_

#define AD7606_CS_H             GPIO_SetBits(GPIOE , GPIO_Pin_5)    // ÷√ °∞1°±
#define AD7606_CS_L             GPIO_ResetBits(GPIOE , GPIO_Pin_5)  // ÷√ °∞0°±

#define AD7606_CLK_H            GPIO_SetBits(GPIOE , GPIO_Pin_4)
#define AD7606_CLK_L            GPIO_ResetBits(GPIOE , GPIO_Pin_4)

#define AD7606_Reset_ON         GPIO_SetBits(GPIOE , GPIO_Pin_3)
#define AD7606_Reset_OFF        GPIO_ResetBits(GPIOE , GPIO_Pin_3)

#define AD7606_CONVT_ON         GPIO_SetBits(GPIOE , GPIO_Pin_2)
#define AD7606_CONVT_OFF        GPIO_ResetBits(GPIOE , GPIO_Pin_2)

#define AD7606_BUSY             GPIO_Pin_6
#define AD7606_DATA             GPIO_Pin_13
#define AD7606_GPIO             GPIOE
#define AD7606_VOLTAGE          10

extern unsigned short ADC_buf[8];
extern float voltage_buf[8];

void AD7606_Init(void);
void AD7606_Read(void);
void AD_Voltage(void);
#endif
