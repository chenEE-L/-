/***********************************************************************
�ļ����ƣ�
��    �ܣ�
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "bsp.h"

/*
 * ���ܣ�
 *       �弶��ʼ��
 * ������
 *       ��
 * ���أ�
 *       ��
 * ˵����
 *       ������ģ���г�ʼ���ľ���ʹ��ģ���ʼ��
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

