#include "app.h"
#include "AD7606.h"
#include "beionfluid_s60.h"
__align(8)
static OS_STK DealSerial_Commend_task_stk[DEALSERIAL_COMMAND_TASK_STK_SIZE]; //定义栈
__align(8)
static OS_STK GetSerial_Command_task_stk[GETSERIAL_COMMAND_TASK_STK_SIZE]; //定义栈
__align(8)
//OS_STK Stk_Task_Ad[TASK_AD_STK_SIZE];
//__align(8)
OS_STK Stk_Task_Temp[TASK_TEMP_STK_SIZE];
__align(8)
//OS_STK Stk_Task_Temp_Contr[TASK_TEMP_CONTR_STK_SIZE];
//__align(8)
OS_STK Stk_Task_Dugs[TASK_DUGS_STK_SIZE];
__align(8)
OS_STK Stk_Task_Auto_Hand[TASK_ATUO_HAND_STK_SIZE];
__align(8)
OS_STK Stk_Task_Fatfs_Test[TASK_FATFS_TEST_STK_SIZE];
__align(8)
OS_STK Stk_Task_Test[TASK_TEST_STK_SIZE];
void Task_Start(void *p_arg)
{
	(void)p_arg;
 
	//SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC - 1);
	//OS_CPU_SysTickInit();  
	
	//USART_Configuration();
	OSStatInit();
	
	OSTaskCreateExt(DealSerial_Command,
								(void *)0,
								&DealSerial_Commend_task_stk[DEALSERIAL_COMMAND_TASK_STK_SIZE-1], 
								DEALSERIAL_COMMAND_TASK_PRIO,
								DEALSERIAL_COMMAND_TASK_PRIO,
								DealSerial_Commend_task_stk,
								DEALSERIAL_COMMAND_TASK_STK_SIZE,
								(void *)0,
								OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
							);
	OSTaskCreateExt(GetSerial_Command,
								(void *)0,
								&GetSerial_Command_task_stk[GETSERIAL_COMMAND_TASK_STK_SIZE-1], 
								GETSERIAL_COMMAND_TASK_PRIO,
								GETSERIAL_COMMAND_TASK_PRIO,
								GetSerial_Command_task_stk,
								GETSERIAL_COMMAND_TASK_STK_SIZE,
								(void *)0,
								OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
							);
    OSTaskCreateExt(Dugs,
								(void *)0,
								&Stk_Task_Dugs[TASK_DUGS_STK_SIZE-1], 
								DUGS_TASK_PRIO,
								DUGS_TASK_PRIO,
								Stk_Task_Dugs,
								TASK_DUGS_STK_SIZE,
								(void *)0,
								OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
							);
//	OSTaskCreateExt(Temperature_contr,
//								(void *)0,
//								&Stk_Task_Temp_Contr[TASK_TEMP_CONTR_STK_SIZE-1], 
//								TASK_TEMP_CONTR_STK_PRIO,
//								TASK_TEMP_CONTR_STK_PRIO,
//								Stk_Task_Temp_Contr,
//								TASK_TEMP_CONTR_STK_SIZE,
//								(void *)0,
//								OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
//							);
    OSTaskCreateExt(Levi2043e,
								(void *)0,
								&Stk_Task_Temp[TASK_TEMP_STK_SIZE-1], 
								TASK_TEMP_STK_PRIO,
								TASK_TEMP_STK_PRIO,
								Stk_Task_Temp,
								TASK_TEMP_STK_SIZE,
								(void *)0,
								OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
							);
//	OSTaskCreateExt(ad_task,
//								(void *)0,
//								&Stk_Task_Ad[TASK_AD_STK_SIZE-1], 
//								AD_TASK_PRIO,
//								AD_TASK_PRIO,
//								Stk_Task_Ad,
//								TASK_AD_STK_SIZE,
//								(void *)0,
//								OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
//							);
    
	OSTaskCreateExt(auto_hand_action,
								(void *)0,
								&Stk_Task_Auto_Hand[TASK_ATUO_HAND_STK_SIZE-1], 
								AUTO_HAND_TASK_PRIO,
								AUTO_HAND_TASK_PRIO,
								Stk_Task_Auto_Hand,
								TASK_ATUO_HAND_STK_SIZE,
								(void *)0,
								OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
							);

	OSTaskCreateExt(fatfs_test,
								(void *)0,
								&Stk_Task_Fatfs_Test[TASK_FATFS_TEST_STK_SIZE-1], 
								FATFS_TEST_TASK_PRIO,
								FATFS_TEST_TASK_PRIO,
								Stk_Task_Fatfs_Test,
								TASK_FATFS_TEST_STK_SIZE,
								(void *)0,
								OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
							);
	OSTaskCreate(test_task,
								(void *)0,
								&Stk_Task_Test[TASK_TEST_STK_SIZE-1], 
								TEST_TASK_PRIO
							);
	OSTaskDel(STARTUP_TASK_PRIO);
	while(1)
	{
		OSTimeDlyHMSM(1,0,0,500);
	}
}

void ad_task(void *p_arg)
{
	LED_Configuration(); //LED 端口初始化 
    //AD7606_Init();
	OSTimeDlyHMSM(0,0,1,0);
	//LED1_OFF;
	while (1)
	{
						  // 亮
		OSTimeDlyHMSM(0,0,0,33);
		//LED1_ON;	    // 灭
  //RCC_GetClocksFreq(&RCC_Clocks);
		//LED2_OFF;
		OSTimeDlyHMSM(0,0,0,33);
		//LED2_ON;
		//LED3_OFF;
		OSTimeDlyHMSM(0,0,0,33);
		//LED3_ON;
        //AD7606_Read();
	}
}

OS_STK_DATA  StackBytes[8];
void test_task(void *p_arg)
{
	//OS_STK_DATA  StackBytes[8];
	while(1)
	{
		OSTimeDlyHMSM(0,0,0,33);
		OSTaskStkChk(DEALSERIAL_COMMAND_TASK_PRIO, &StackBytes[0]);
		OSTimeDlyHMSM(0,0,0,33);
		OSTaskStkChk(GETSERIAL_COMMAND_TASK_PRIO, &StackBytes[1]);
		OSTimeDlyHMSM(0,0,0,33);
		OSTaskStkChk(TASK_TEMP_STK_PRIO, &StackBytes[2]);
		OSTimeDlyHMSM(0,0,0,33);
//		OSTaskStkChk(AD_TASK_PRIO, &StackBytes[3]);
//		OSTimeDlyHMSM(0,0,0,33);
//		OSTaskStkChk(TASK_TEMP_CONTR_STK_PRIO, &StackBytes[4]);
		OSTimeDlyHMSM(0,0,0,33);
		OSTaskStkChk(AUTO_HAND_TASK_PRIO, &StackBytes[5]);
		OSTimeDlyHMSM(0,0,0,33);
		OSTaskStkChk(DUGS_TASK_PRIO, &StackBytes[6]);
		OSTimeDlyHMSM(0,0,0,33);
		OSTaskStkChk(FATFS_TEST_TASK_PRIO, &StackBytes[7]);
	}
}
