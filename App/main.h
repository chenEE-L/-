/***********************************************************************
�ļ����ƣ�
��    �ܣ�
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
 
#ifndef _MAIN_H_
#define _MAIN_H_


//��ӱ�Ҫ��ͷ�ļ�
#include "stm32f4xx.h" 
#include "stm32f4x7_eth.h"
#include "stm32f4x7_eth_bsp.h"
#include "../lwip/UDP.h"
#include "netconf.H"
//#include "M_Type_P.h"
//#include "M_CMSIS_P.h" 
//#include "M_Global.h"
//#include "Task_Startup.h"
#include "Bsp_Global.h"
//#include "Task_Test.h"
#include "led.h"   
#include "UDP_SERVER.h"
#include "UDP_CLIENT.h"
#include "TCP_CLIENT.h"
#include "lwip/tcp.h"
#include "NVIC.h"
//ET Module�汾��
#define M_VERSION	100


//1��Ƭ������ 

#define M_DEV_MCU   1
 #define RMII_MODE  1

//ȫ�ֳ�ʼ����������
//void M_Global_init(void);


//////////////////////////////////////////////////////////////////////////
/////////////////////////���°�����ͬģ���ͷ�ļ�/////////////////////////
//////////////////////////////////////////////////////////////////////////

//����ʱ����
//#include "M_Delay_P.h"

//void Delay(uint32_t nCount);


#endif
