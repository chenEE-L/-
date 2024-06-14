#include "bsp.h"

void PWM_GPIO_INIT()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //Õ∆ÕÏ ‰
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3); 
}

void PWM_TIME3_OC4_INIT()
{
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    TIM_TimeBaseInitStruct.TIM_Period = 1000;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;//1MHZ
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIME1~5£¨8
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x0000;  //TIME1£¨8
    
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
    
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    //TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;//TIME1,8
    TIM_OCInitStruct.TIM_Pulse = 500;
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
    //TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;//TIME1,8
    //TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;//TIME1,8
    //TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//TIME1,8
    TIM_OC4Init(TIM3,&TIM_OCInitStruct);        
    TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3,ENABLE);
    
    TIM_Cmd(TIM3,ENABLE);
    //TIM_CCxCmd(TIM3,TIM_Channel_4,TIM_CCx_Enable);
}
