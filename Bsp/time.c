#include "bsp.h"
#include "app.h"
//三路PWM输出IO配置　


// modbus上报计时使用
#define TIM_HARD		TIM2
#define TIM_HARD_IRQn	TIM2_IRQn
#define TIM_HARD_RCC	RCC_APB1Periph_TIM2
static void (*s_TIM_CallBack1)(void);
static void (*s_TIM_CallBack2)(void);
static void (*s_TIM_CallBack3)(void);
static void (*s_TIM_CallBack4)(void);

void PWM_GPIO_INIT()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); 
	
	//GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4); 
	
	//GPIO_InitTypeDef GPIO_InitStructure;  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4); 
		 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4); 
}
//*******************************************************
//*TIME4 PWM输出初始化
//*使用TIM4输出三路PWM，OC2、3、4
//*频率：50HZ
void PWM_TIME4_OCH_INIT()
{
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    TIM_TimeBaseInitStruct.TIM_Period = 2000;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 840-1;//100KHZ
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//TIMEx x不等于6，7
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIME1~5，8
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000;  //TIME1，8
    
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    //TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;//TIME1,8
    TIM_OCInitStruct.TIM_Pulse = 500;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    //TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;//TIME1,8
    //TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;//TIME1,8
    //TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//TIME1,8
    //TIM_OC1Init(TIM4,&TIM_OCInitStruct);        
    //TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
    
		
    TIM_OC2Init(TIM4,&TIM_OCInitStruct);        
    TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);
		
		TIM_OC3Init(TIM4,&TIM_OCInitStruct);        
    TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
		
		TIM_OC4Init(TIM4,&TIM_OCInitStruct);        
    TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
		
		TIM_ARRPreloadConfig(TIM4,ENABLE);
    TIM_Cmd(TIM4,ENABLE);
    //TIM_CCxCmd(TIM4,TIM_Channel_4,TIM_CCx_Enable);
}

void PWM_TIME11_OC1_INIT()
{
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
    
    TIM_TimeBaseInitStruct.TIM_Period = 2000;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1680-1;//100KHZ
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//TIMEx x不等于6，7
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIME1~5，8
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000;  //TIME1，8
    
    TIM_TimeBaseInit(TIM11,&TIM_TimeBaseInitStruct);
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    //TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;//TIME1,8
    TIM_OCInitStruct.TIM_Pulse = 100;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    //TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;//TIME1,8
    //TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;//TIME1,8
    //TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//TIME1,8
    TIM_OC1Init(TIM11,&TIM_OCInitStruct);        
    TIM_OC1PreloadConfig(TIM11,TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM11,ENABLE);
    
    TIM_Cmd(TIM11,ENABLE);
    //TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Enable);
}

void PWM_TIME10_OC1_INIT()
{
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
    
    TIM_TimeBaseInitStruct.TIM_Period = 2000;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 1680-1;//1MHZ
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//TIMEx x不等于6，7
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIME1~5，8
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000;  //TIME1，8
    
    TIM_TimeBaseInit(TIM10,&TIM_TimeBaseInitStruct);
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    //TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;//TIME1,8
    TIM_OCInitStruct.TIM_Pulse = 100;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    //TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;//TIME1,8
    //TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;//TIME1,8
    //TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//TIME1,8
    TIM_OC1Init(TIM10,&TIM_OCInitStruct);        
    TIM_OC1PreloadConfig(TIM10,TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM10,ENABLE);
    
    TIM_Cmd(TIM10,ENABLE);
    //TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Enable);
}

void Time2_freq(int freq)
{
//	unsigned short psc;
//	psc = (84000000>>16)/freq+1;//向上取整
//	if(psc<3)
//		psc = 3;
//	
//	TIM2->PSC = psc-1;//分频
//	TIM2->ARR = 84000000.0/psc/freq+0.5;//设置重装值，四舍五入
//	TIM2->CNT = 0;
}
//*******************************
//*TIM2初始化
//*控制蠕动泵电机使用
void Time2_Init()
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	uint32_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTIMxCLK;

  	/* 使能TIM时钟 */
	RCC_APB1PeriphClockCmd(TIM_HARD_RCC, ENABLE);

    /*-----------------------------------------------------------------------
		system_stm32f4xx.c 文件中 void SetSysClock(void) 函数对时钟的配置如下：

		HCLK = SYSCLK / 1     (AHB1Periph)
		PCLK2 = HCLK / 2      (APB2Periph)
		PCLK1 = HCLK / 4      (APB1Periph)

		因为APB1 prescaler != 1, 所以 APB1上的TIMxCLK = PCLK1 x 2 = SystemCoreClock / 2;
		因为APB2 prescaler != 1, 所以 APB2上的TIMxCLK = PCLK2 x 2 = SystemCoreClock;

		APB1 定时器有 TIM2, TIM3 ,TIM4, TIM5, TIM6, TIM7, TIM12, TIM13,TIM14
		APB2 定时器有 TIM1, TIM8 ,TIM9, TIM10, TIM11

	----------------------------------------------------------------------- */
	uiTIMxCLK = SystemCoreClock / 2;

	usPrescaler = uiTIMxCLK / 1000000 ;	/* 分频到周期 1us */
	
#if defined (USE_TIM2) || defined (USE_TIM5) 
	usPeriod = 0xFFFFFFFF;
#else
	usPeriod = 0xFFFF;
#endif
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = usPeriod;
	TIM_TimeBaseStructure.TIM_Prescaler = usPrescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM_HARD, &TIM_TimeBaseStructure);

	//TIM_ARRPreloadConfig(TIMx, ENABLE);

	/* TIMx enable counter */
	TIM_Cmd(TIM_HARD, ENABLE);

	/* 配置TIM定时中断 (Update) */
	{
		NVIC_InitTypeDef NVIC_InitStructure;	/* 中断结构体在 misc.h 中定义 */

		NVIC_InitStructure.NVIC_IRQChannel = TIM_HARD_IRQn;

		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;	/* 比串口优先级低 */
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}
//TIM3固定100ms周期定时器 提供温控准确周期使用
void Time3_Init()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;	
    #if TIME3_ENABLE==1
     
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseInitStruct.TIM_Period = 50000;//Time2 32位  Time4 16位
    TIM_TimeBaseInitStruct.TIM_Prescaler = 168-1;//0.5MHZ
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//TIMEx x不等于6，7
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIME1~5，8
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000;  //TIME1，8
    
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update); //清除溢出中断标志 
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 
    
    TIM_Cmd(TIM3, DISABLE); //关闭时钟
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //通道TIM3  
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//占优先级 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级 
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure);
    
    TIM_Cmd(TIM3, ENABLE);
    #endif
}
//蠕动泵电机输出
//void TIM2_IRQHandler()
//{
    //TIM_ClearFlag(TIM2,TIM_IT_Update);
    ///TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
//    TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
//    if(GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_2))
//	{
//		GPIO_ResetBits(GPIOE,GPIO_Pin_2);
//		if(GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_3))
//			model_1006.motor_data ++;
//		else
//			model_1006.motor_data --;
//	}
//    else 
//    GPIO_SetBits(GPIOE,GPIO_Pin_2);
//	
//	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15)==0)
//	{
//		
//		if(model_1006.motor_data >= 33000)//第一次启动确定电机位置
//		{
//			if(GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_3))
//			{
//				model_1006.motor_data = 32000;
//			}
//			else
//			{
//				model_1006.motor_data = 0;
//			}
//		}
//		if(model_1006.motor_data >=16000)//判断电机到达限位还是离开限位
//		{
//			if(GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_3)==1)
//			{
//				TIM_Cmd(TIM2, DISABLE); //关闭时钟
//				GPIO_SetBits(GPIOE,GPIO_Pin_2);
//				model_1006.motor_data = 32000;
//			}
//		}
//		else
//		{
//			if(GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_3)==0)
//			{
//				TIM_Cmd(TIM2, DISABLE); //关闭时钟
//				GPIO_SetBits(GPIOE,GPIO_Pin_2);
//				model_1006.motor_data = 0;
//			}
//		}
//	}
//}

void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack)
{
    uint32_t cnt_now;
    uint32_t cnt_tar;

    /*
        执行下面这个语句，时长 = 18us (通过逻辑分析仪测量IO翻转)
        bsp_StartTimer2(3, 500, (void *)test1);
    */
    if (_uiTimeOut < 5)
    {
        ;
    }
    else
    {
        _uiTimeOut -= 5;
    }

    cnt_now = TIM_GetCounter(TIM_HARD);    	/* 读取当前的计数器值 */
    cnt_tar = cnt_now + _uiTimeOut;			/* 计算捕获的计数器值 */
    if (_CC == 1)
    {
        s_TIM_CallBack1 = (void (*)(void))_pCallBack;

        TIM_SetCompare1(TIM_HARD, cnt_tar);      	/* 设置捕获比较计数器CC1 */
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC1);
		TIM_ITConfig(TIM_HARD, TIM_IT_CC1, ENABLE);	/* 使能CC1中断 */

    }
    else if (_CC == 2)
    {
		s_TIM_CallBack2 = (void (*)(void))_pCallBack;

        TIM_SetCompare2(TIM_HARD, cnt_tar);      	/* 设置捕获比较计数器CC2 */
		TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC2);
		TIM_ITConfig(TIM_HARD, TIM_IT_CC2, ENABLE);	/* 使能CC2中断 */
    }
    else if (_CC == 3)
    {
        s_TIM_CallBack3 = (void (*)(void))_pCallBack;

        TIM_SetCompare3(TIM_HARD, cnt_tar);      	/* 设置捕获比较计数器CC3 */
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC3);
		TIM_ITConfig(TIM_HARD, TIM_IT_CC3, ENABLE);	/* 使能CC3中断 */
    }
    else if (_CC == 4)
    {
        s_TIM_CallBack4 = (void (*)(void))_pCallBack;

        TIM_SetCompare4(TIM_HARD, cnt_tar);      	/* 设置捕获比较计数器CC4 */
		TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC4);
		TIM_ITConfig(TIM_HARD, TIM_IT_CC4, ENABLE);	/* 使能CC4中断 */
    }
	else
    {
        return;
    }
}

/*
*********************************************************************************************************
*	函 数 名: TIMx_IRQHandler
*	功能说明: TIM 中断服务程序
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/

void TIM2_IRQHandler(void)

{
    if (TIM_GetITStatus(TIM_HARD, TIM_IT_CC1))
    {
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC1);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC1, DISABLE);	/* 禁能CC1中断 */

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack1();
    }

    if (TIM_GetITStatus(TIM_HARD, TIM_IT_CC2))
    {
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC2);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC2, DISABLE);	/* 禁能CC2中断 */

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack2();
    }

    if (TIM_GetITStatus(TIM_HARD, TIM_IT_CC3))
    {
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC3);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC3, DISABLE);	/* 禁能CC3中断 */

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack3();
    }

    if (TIM_GetITStatus(TIM_HARD, TIM_IT_CC4))
    {
        TIM_ClearITPendingBit(TIM_HARD, TIM_IT_CC4);
        TIM_ITConfig(TIM_HARD, TIM_IT_CC4, DISABLE);	/* 禁能CC4中断 */

        /* 先关闭中断，再执行回调函数。因为回调函数可能需要重启定时器 */
        s_TIM_CallBack4();
    }
}
//定时器TIM3 温控10ms周期抛出信号量
extern OS_EVENT *Temp_Contr_time;
void TIM3_IRQHandler()
{
    //TIM_ClearFlag(TIM2,TIM_IT_Update);
    ///TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
    TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
		OSSemPost(Temp_Contr_time);
}

