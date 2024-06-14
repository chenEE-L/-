#ifndef _LED_H_
#define _LED_H_

#define LED1 		GPIOA, GPIO_Pin_0
#define LED2 		GPIOA , GPIO_Pin_1
#define LED3 		GPIOA , GPIO_Pin_2

#define LED1_ON 		GPIO_ResetBits(GPIOE , GPIO_Pin_8)
#define LED2_ON 		GPIO_ResetBits(GPIOE , GPIO_Pin_9)
#define LED3_ON 		GPIO_ResetBits(GPIOE , GPIO_Pin_10)



#define LED1_OFF 		GPIO_SetBits(GPIOE , GPIO_Pin_8)
#define LED2_OFF 		GPIO_SetBits(GPIOE , GPIO_Pin_9)
#define LED3_OFF 		GPIO_SetBits(GPIOE , GPIO_Pin_10)
extern void LED_Configuration(void);

#endif
