#ifndef _APP_H_
#define _APP_H_

#include "app_cfg.h"
#include "ucos_ii.h" 
#include "stm32f4xx.h" 
#include "usart.h"
#include "led.h"
#include "serial_command.h"
#include "filter.h"
#include "moter.h"
#include "dgus.h"
#include "fatfs.h"
#include "ff.h"	


extern void Task_Start(void *p_arg);


void GetSerial_Command(void *p_arg);
void GetUsart1_Commend(void);
void GetUsart2_Commend(void);
void GetUsart4_Commend(void);
void GetUsart3_Commend(void);
void GetUsart6_Commend(void);
void GetUsart5_Commend(void);//上位机
//**************************继电器控制部分***************************
void Set_relay_onoff(u8 addr,u8 data);
//******************************************************************

//**************************udp_task.c***************************
//void Task_UDP_Server(void *pdata);
//******************************************************************
//**************************tcp_task.c***************************
//void Task_TCP_Client(void *pdata);
//******************************************************************
//**************************app.c***************************
void ad_task(void *p_arg);
//******************************************************************
//**************************Temperature.c***************************
typedef struct
{
    float Temp;
    float Last1_Temp;
    float Last2_Temp;
    float Temp_speed;
    float Temp_Integral;
    float Temp_Contr_P;
    float Temp_Contr_I;
    float Temp_Contr_D;
    float PWM_Out_P;
    float PWM_Out_I;
    float PWM_Out_D;
    float PWM_out;
    float PWM_MAX;
}TEMPERATURE_CONTROL;
extern float TEMP_25,TEMP_70,TEMP_10,TEMP_Indev;
void Get_Temperature(void *p_arg);
void Temperature_contr(void *p_arg);
//**************************************************************


void test_task(void *p_arg);
#endif
