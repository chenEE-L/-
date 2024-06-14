#ifndef  _BSP_H_
#define  _BSP_H_

#include "ucos_ii.h" 
#include "stm32f4xx.h" 
#include "NVIC.h"
#include "Bsp_Global.h"
#include "usart.h"
#include "led.h"
#include "RS485.h"
#include "spi.h"
#include "time.h"
#include "AD7606.h"
#include "IO.h"
#include "nand.h"
//INT32U  BSP_CPU_ClkFreq (void);
//INT32U  OS_CPU_SysTickClkFreq (void);

#define RADIOTUBE_OFF 		GPIO_ResetBits(GPIOC , GPIO_Pin_0)//µç´Å·§
#define ATOMIZER_OFF 		GPIO_ResetBits(GPIOC , GPIO_Pin_1)//Îí»¯Æ÷
//#define LED3_ON 		GPIO_ResetBits(GPIOD , GPIO_Pin_10)



#define RADIOTUBE_ON 		GPIO_SetBits(GPIOC , GPIO_Pin_0)
#define ATOMIZER_ON 		GPIO_SetBits(GPIOC , GPIO_Pin_1)
//#define LED3_OFF 		GPIO_SetBits(GPIOD , GPIO_Pin_10)

//***************dac7512.c***************************
void se_dac(uint16_t data);
//***************************************************
#endif
