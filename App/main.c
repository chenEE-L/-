/***********************************************************************
�ļ����ƣ�main.C
��    �ܣ��źŴ�����Ƴ��򣬻���UC0S II����ϵͳ
ʵ��ƽ̨������STM32F407VET6 ������
��汾  ��V1.0.0 
***********************************************************************/

//#include "ucos_ii.h"  //uC/OS-II ϵͳ����ͷ�ļ�
#include "bsp.h"
#include "app.h" 

OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE]; //����ջ
int main()
{
	//���ʼ��
 	BSP_Init();
	//��ʼ��ϵͳ
	OSInit();
	//��������
 
	OSTaskCreate(Task_Start,(void *)0,(OS_STK *)&startup_task_stk[STARTUP_TASK_STK_SIZE-1], (INT8U)STARTUP_TASK_PRIO);
	//���ļ�������0  
	OSTimeSet(0);
	//����UCOS-II�ں�
	OSStart();
	while(1)
	{
		
	}
	//return 0;
}

