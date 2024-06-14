/***********************************************************************
文件名称：
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
#include "bsp.h"

/*
 * 功能：
 *       板级初始化
 * 参数：
 *       无
 * 返回：
 *       无
 * 说明：
 *       可用与模块中初始化的尽量使用模块初始化
 */
void BSP_Init(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	SystemInit();
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.SYSCLK_Frequency /OS_TICKS_PER_SEC); 
	//LED_Configuration();
	NVIC_Configuration();
	WatchDog_IO_Init();//
}

