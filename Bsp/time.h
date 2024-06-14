
#include "bsp.h"

#ifndef _PWM_H_
#define _PWM_H_

#define TIME2_ENABLE    1
#define TIME3_ENABLE    1
#define TIME4_ENABLE    1

#define PWM1_PIN             GPIO_Pin_1
#define PWM1_GPIO            GPIOB

void bsp_StartHardTimer(uint8_t _CC, uint32_t _uiTimeOut, void * _pCallBack);
void PWM_GPIO_INIT(void);
void PWM_TIME4_OCH_INIT(void);
void PWM_TIME11_OC1_INIT(void);
void PWM_TIME10_OC1_INIT(void);
void Time2_freq(int freq);
void Time2_Init(void);
void Time3_Init(void);
#endif
