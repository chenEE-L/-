#include "main.h"  
#include "app.h"
//UDP�������շ�����
extern volatile int COM1_command_data_length;
extern struct ip_addr destAddr ; /* ��ȡԶ������ IP��ַ */
extern u16_t  destPORT;
void Task_UDP_Server(void *pdata)
{
	__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
	/* configure ethernet (GPIOs, clocks, MAC, DMA) */ 
	ETH_BSP_Config();
	LwIP_Init();

	/* UDP_server Init */
	UDP_server_init();
	//UDP_client_init();
	OSTimeDlyHMSM(0, 0, 15, 5);
	while(1)
	{  
		if(Uart1_command[0])
			{
				
				if(COM1_command_data_length!=0)
				udp_p = pbuf_alloc(PBUF_RAW,COM1_command_data_length,PBUF_RAM);
				udp_p -> payload = (void *)&Uart1_command[1];
				//udp_send(udp_pcb,udp_p);//��������
				udp_sendto(udp_pcb,udp_p,&destAddr,destPORT); /* ���յ��������ٷ��ͳ�ȥ */
				pbuf_free(udp_p);
				COM1_command_data_length = 0;
				Uart1_command[0] = 0;
			}
		LocalTime += 2;
		LwIP_Periodic_Handle(LocalTime);		
		OSTimeDlyHMSM(0, 0, 0, 5);//����5ms���Ա������߳�����
	}
}

