#include "App.h"
#include "Usart.h"
#include "serial_command.h"
/*
void GetSerial_Command()
�ӻ��������ȡָ��
*/
OS_EVENT *Usart4Serial_Command;
//extern OS_EVENT *Usart3Serial_Command;
volatile int COM1_command_data_length = 0;
//�Ӵ��ڻ�������ȡ���ݣ����/�������
void GetSerial_Command(void *p_arg)
{
	(void)p_arg;
	int i;
	OSTimeDlyHMSM(0,0,1,1);
	Usart4Serial_Command = OSSemCreate(0);
	Time2_Init(); // modbus��ʱ����ʼ��
	//OS_TaskStat(p_arg);
	while(1)
	{
		GetUsart2_Commend();
		GetUsart1_Commend();
		GetUsart6_Commend();
		GetUsart4_Commend();
		GetUsart3_Commend();	
		OSTimeDlyHMSM(0,0,0,5);
		i++;
		if(i>200)
		{
			i=0;
		}
	}
}




/*�Ӵ���1���ȡָ��*/
void GetUsart1_Commend()
{
	UART_T *pUart = &g_tUart1;
  static COMMAND_RECEIVE_STATUS_t command_status = WAITING_COMMAND_SYNC_FLAG;
  uint8_t receive_data;
  static uint32_t command_data_length=0;
	while(Uart1_command[0]==0)//ֱ���õ�һ��ָ��
	{
		if(pUart->usRxRead != pUart->usRxWrite) /////����������
		{
			receive_data = pUart->pRxBuf[pUart->usRxRead++];
			if (pUart->usRxRead >= pUart->usRxBufSize) pUart->usRxRead = 0;  
			
 			switch(command_status)
 			{
 				case WAITING_COMMAND_SYNC_FLAG:
 				{
 						
 					if(receive_data == 0x5A)
 					{
 						command_status = FIND_COMMAND_HEAD;
 						command_data_length = 1;
 						Uart1_command[1] = receive_data;
 					}
 				}
 				break;
 				case FIND_COMMAND_HEAD:
 				{
 					command_data_length++;
 					if(receive_data == 0xA5)
 					{
 						command_status = WAITING_READ_COMMAND_DATA;
 						Uart1_command[command_data_length] = receive_data;
 					}
 					else if(receive_data == 0x10)
 					{
 						command_status = WAITING_WRITE_COMMAND_DATA;
 						Uart1_command[command_data_length] = receive_data;
 					}
 					else  command_status = WAITING_COMMAND_SYNC_FLAG;
 				}
 				break;
 				case WAITING_READ_COMMAND_DATA:
 				{
 					command_data_length++;
					command_status = WAITING_COMMAND_END;
 					Uart1_command[command_data_length] = receive_data;
 					
 				}
 				break;
 				case WAITING_COMMAND_END:
 				{
 					command_data_length++;
 					if(command_data_length >=UART1_COMMEND_SIZE) /////�������ݿ��ܴ��ˣ�������ͬ��ͷ
 					{
 						command_status = WAITING_COMMAND_SYNC_FLAG; 
 					}
 					else ////У����
 					{
						
 						Uart1_command[command_data_length] = receive_data;	
 						//ָʾ���յ�һ���µ�����
						if(command_data_length>=(Uart1_command[3]+3))//00 5A A5 +�ֽ���+ָ��+��ַ+����
						{
							Uart1_command[0] = 1;		
							//�˴�post�ź���
							
							
							command_status = WAITING_COMMAND_SYNC_FLAG; /////���յ�һ��������������¿�ʼ׼�������һ������ͬ��ͷ
						}
 					}
 				}
 				break;
 				default: break;
 			} 
		}
		else
			//**û���յ�����������ѭ��
			break;
	}
}
///*�Ӵ���2���ȡָ��*/
//void GetUsart2_Commend()
//{
//	#if UART2_FIFO_EN == 1
//	 UART_T *pUart = &g_tUart2;
//   static COMMAND_RECEIVE_STATUS_t command_status = WAITING_COMMAND_SYNC_FLAG;
//   uint8_t receive_data;
//   static uint32_t command_data_length=0;
//  while(Uart2_command[0]==0)
//  if(pUart->usRxRead != pUart->usRxWrite) /////����������
//  {
//		
//  	receive_data = pUart->pRxBuf[pUart->usRxRead++];
//		if (pUart->usRxRead >= pUart->usRxBufSize) pUart->usRxRead = 0;  
//		switch(command_status)
//		{
//			case WAITING_COMMAND_SYNC_FLAG:
//			{
//				if(receive_data=='A')
//				{
//					command_status = WAITING_COMMAND_DATA;
//					command_data_length = 1;
//					Uart2_command[command_data_length] = receive_data;
//				}
//			}
//			break;
//			case FIND_COMMAND_HEAD:
//			{
//				
//			}
//			break;
//			case WAITING_COMMAND_DATA:
//			{
//				command_data_length++;
//				if(command_data_length >= USART2_SERIAL_COMMAND_MAX_LENGTH) ////�������ݿ��ܴ��ˣ�������ͬ��ͷ
//				{
//					command_status = WAITING_COMMAND_SYNC_FLAG; 
//				}				
//				else 
//				{
//					if((receive_data ==0x0a)||(receive_data ==0x0d)) ////�����ַ���������
//					{					
//						Uart2_command[command_data_length] = receive_data;
//							
//						////ָʾ���յ�һ���µ�����
//						Uart2_command[0] = 1;		
//						//OSSemPost(Usart2Serial_Command);
//						command_status = WAITING_COMMAND_SYNC_FLAG; /////���յ�һ��������������¿�ʼ׼�������һ������ͬ��ͷ
//					}
//					else Uart2_command[command_data_length] = receive_data;
//				}
//			}
//			break;
//			default: break;
//		}
//		 
//	}
//	else 
//		break;
//	#endif
//}
//*
//* 
//*
//*
//*
void GetUsart4_Commend(void)
{
	#if UART4_FIFO_EN == 1
	 UART_T *pUart = &g_tUart4;
   static COMMAND_RECEIVE_STATUS_t command_status = WAITING_COMMAND_SYNC_FLAG;
   uint8_t receive_data;
   static uint32_t command_data_length=0;
  while(Uart4_command[0]==0)
  if(pUart->usRxRead != pUart->usRxWrite) /////����������
  {
		
  	receive_data = pUart->pRxBuf[pUart->usRxRead++];
		if (pUart->usRxRead >= pUart->usRxBufSize) pUart->usRxRead = 0;  
		switch(command_status)
		{
			case WAITING_COMMAND_SYNC_FLAG:
			{
				if(receive_data==0x01)
				{
					command_status = FIND_COMMAND_HEAD;
					command_data_length = 1;
					Uart4_command[command_data_length] = receive_data;
				}
			}
			break;
			case FIND_COMMAND_HEAD:
			{
				command_data_length++;
				if(receive_data==0x03||receive_data==0x06||receive_data==0x10)//��/д�����Ĵ���
				{
					command_status = WAITING_COMMAND_DATA;
					Uart4_command[command_data_length] = receive_data;
				}
				else 
					command_status = WAITING_COMMAND_SYNC_FLAG;
			}
			break;
			case WAITING_COMMAND_DATA:
			{
				command_data_length++;
				Uart4_command[command_data_length] = receive_data;
				if(command_data_length >= 8) ////�����ַ���������
				{
					if(Uart4_command[2]==0x06)
					{
						////ָʾ���յ�һ���µ�����
						Uart4_command[0] = 1;		
						command_status = WAITING_COMMAND_SYNC_FLAG; /////���յ�һ��������������¿�ʼ׼�������һ������ͬ��ͷ
					}
					else if(Uart4_command[2]==0x03)
					{
						if(command_data_length>=(Uart4_command[3]+4))
						{
							////ָʾ���յ�һ���µ�����
							Uart4_command[0] = 1;		
							command_status = WAITING_COMMAND_SYNC_FLAG; /////���յ�һ��������������¿�ʼ׼�������һ������ͬ��ͷ
						}
					}
					else if(Uart4_command[2]==0x010)
					{
						//if(command_data_length>=Uart4_command[7]+8)
						{
							////ָʾ���յ�һ���µ�����
							Uart4_command[0] = 1;		
							command_status = WAITING_COMMAND_SYNC_FLAG; /////���յ�һ��������������¿�ʼ׼�������һ������ͬ��ͷ
						}
					}
					else if(command_data_length>=UART4_COMMEND_SIZE)
					{
						command_status = WAITING_COMMAND_SYNC_FLAG; /////δ���յ�һ��������������¿�ʼ׼�������һ������ͬ��ͷ
					}
						
					
				}
			}
			break;
			default: break;
		}
		 
	}
	else 
		break;
	#endif
}

void GetUsart3_Commend()
{
	//static uint8_t  Uart4_command_line[10];
#if UART3_FIFO_EN == 1
	 UART_T *pUart = &g_tUart3;
   static COMMAND_RECEIVE_STATUS_t command_status = WAITING_COMMAND_SYNC_FLAG;
   uint8_t receive_data;
   static uint32_t command_data_length=0;
  while(Uart3_command[0]==0)
	{
		if(pUart->usRxRead != pUart->usRxWrite) /////����������
		{
			
			receive_data = pUart->pRxBuf[pUart->usRxRead++];
				
			
			if (pUart->usRxRead >= pUart->usRxBufSize) pUart->usRxRead = 0;  
		 
			switch(command_status)
			{
				case WAITING_COMMAND_SYNC_FLAG:
				{
					//comSendChar(COM1,receive_data);	
					if(receive_data == 0xff)
					{
						command_status = FIND_COMMAND_HEAD;
						command_data_length = 1;
						Uart3_command[command_data_length] = receive_data;
					}
				}
				break;
				case FIND_COMMAND_HEAD:
				{
					if(receive_data == 0x03)
					{
						command_status = WAITING_COMMAND_DATA;
						command_data_length ++;
						Uart3_command[command_data_length] = receive_data;
					}
					else
						command_status = WAITING_COMMAND_SYNC_FLAG; 
				}
				break;
				case WAITING_COMMAND_DATA:
				{
					command_data_length++;
					if(command_data_length >= UART3_COMMEND_SIZE) ////�������ݿ��ܴ��ˣ�������ͬ��ͷ
					{
											
							//Uart4_command[command_data_length] = receive_data;
								
							////ָʾ���յ�һ���µ�����
							Uart3_command[0] = 1;		
							//OSSemPost(Usart4Serial_Command);
							command_status = WAITING_COMMAND_SYNC_FLAG; /////���յ�һ��������������¿�ʼ׼�������һ������ͬ��ͷ
						
					}
					else Uart3_command[command_data_length] = receive_data;

					
				}
				break;
				default: break;
			}
		}
		else
			break;
	}
	#endif
}

/*�Ӵ���2���ȡָ��*/
void GetUsart6_Commend()
{
	#if UART6_FIFO_EN == 1
	 UART_T *pUart = &g_tUart6;
   static COMMAND_RECEIVE_STATUS_t command_status = WAITING_COMMAND_SYNC_FLAG;
   uint8_t receive_data;
   static uint32_t command_data_length=0;
  while(Uart6_command[0]==0)
  if(pUart->usRxRead != pUart->usRxWrite) /////����������
  {
		
  	receive_data = pUart->pRxBuf[pUart->usRxRead++];
		if (pUart->usRxRead >= pUart->usRxBufSize) pUart->usRxRead = 0;  
		switch(command_status)
		{
			case WAITING_COMMAND_SYNC_FLAG:
			{
				if(receive_data=='A')
				{
					command_status = WAITING_COMMAND_DATA;
					command_data_length = 1;
					Uart6_command[command_data_length] = receive_data;
				}
			}
			break;
			case FIND_COMMAND_HEAD:
			{
				
			}
			break;
			case WAITING_COMMAND_DATA:
			{
				command_data_length++;
				if(command_data_length >= USART6_SERIAL_COMMAND_MAX_LENGTH) ////�������ݿ��ܴ��ˣ�������ͬ��ͷ
				{
					command_status = WAITING_COMMAND_SYNC_FLAG; 
				}				
				else 
				{
					if((receive_data ==0x0a)||(receive_data ==0x0d)) ////�����ַ���������
					{					
						Uart6_command[command_data_length] = receive_data;
							
						////ָʾ���յ�һ���µ�����
						Uart6_command[0] = 1;		
						//OSSemPost(Usart2Serial_Command);
						command_status = WAITING_COMMAND_SYNC_FLAG; /////���յ�һ��������������¿�ʼ׼�������һ������ͬ��ͷ
					}
					else Uart6_command[command_data_length] = receive_data;
				}
			}
			break;
			default: break;
		}
		 
	}
	else 
		break;
	#endif
}
/*�Ӵ���5���ȡָ��*/
void GetUsart2_Commend()
{
	#if UART2_FIFO_EN == 1
	 UART_T *pUart = &g_tUart2;
//   static COMMAND_RECEIVE_STATUS_t command_status = WAITING_COMMAND_SYNC_FLAG;
//   uint8_t receive_data;
//   static uint32_t command_data_length=0;
  while(Uart2_command[0]==0)
  if(pUart->usRxRead != pUart->usRxWrite) /////����������
  {
		MODS_ReciveNew( pUart->pRxBuf[pUart->usRxRead++]);
		if (pUart->usRxRead >= pUart->usRxBufSize) pUart->usRxRead = 0;  
	}
	else 
		break;
	#endif
}

