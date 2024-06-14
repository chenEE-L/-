/***********************************************************************
文件名称：main.C
功    能：信号处理控制程序，基于UC0S II操作系统
实验平台：基于STM32F407VET6 开发板
库版本  ：V1.0.0 
***********************************************************************/

//#include "ucos_ii.h"  //uC/OS-II 系统函数头文件
#include "bsp.h"
#include "app.h" 

OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE]; //定义栈
int main()
{
	//板初始化
 	BSP_Init();
	//初始化系统
	OSInit();
	//创建任务
 
	OSTaskCreate(Task_Start,(void *)0,(OS_STK *)&startup_task_stk[STARTUP_TASK_STK_SIZE-1], (INT8U)STARTUP_TASK_PRIO);
	//节拍计数器清0  
	OSTimeSet(0);
	//启动UCOS-II内核
	OSStart();
	while(1)
	{
		
	}
	//return 0;
}

