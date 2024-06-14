#include "main.h"  
#include "app.h"

extern volatile int COM1_command_data_length;

//tcp�ͻ����շ�����
void Task_TCP_Client(void *pdata)
{
	__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
	//unsigned char tcp_data[] = "tcp �ͻ���ʵ��!\n";
	struct tcp_pcb *pcb;
	/* configure ethernet (GPIOs, clocks, MAC, DMA) */ 
	ETH_BSP_Config();
	LwIP_Init();

	/* TCP_Client Init */
	TCP_Client_Init(TCP_LOCAL_PORT,TCP_SERVER_PORT,TCP_SERVER_IP);
	while(1)
	{  
		pcb = Check_TCP_Connect();
		if(pcb != 0)
		{	
			if(Uart1_command[0])
			{
				
				if(COM1_command_data_length!=0)
				TCP_Client_Send_Data(pcb,&Uart1_command[1],COM1_command_data_length);	//�ú���Ϊ��������������ͺ�����
				//comSendBuf(COM1,Uart1_command,COM1_command_data_length+1);
				COM1_command_data_length = 0;
				Uart1_command[0] = 0;
			}
			//else TCP_Client_Send_Data(pcb,tcp_data,sizeof(tcp_data));	//�ú���Ϊ��������������ͺ�����
		}		
		LocalTime += 10;
		LwIP_Periodic_Handle(LocalTime);		
		OSTimeDlyHMSM(0, 0, 0, 10);//����10ms���Ա������߳�����
	}
}

