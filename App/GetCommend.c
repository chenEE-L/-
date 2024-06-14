#include "App.h"
#include "Usart.h"
#include "serial_command.h"
/*
void GetSerial_Command()
从缓冲区里获取指令
*/
OS_EVENT *Usart4Serial_Command;
//extern OS_EVENT *Usart3Serial_Command;
volatile int COM1_command_data_length = 0;
//从串口缓冲器里取数据，组包/打包数据
void GetSerial_Command(void *p_arg)
{
	(void)p_arg;
	int i;
	OSTimeDlyHMSM(0,0,1,1);
	Usart4Serial_Command = OSSemCreate(0);
	Time2_Init(); // modbus定时器初始化
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




/*从串口1里获取指令*/
void GetUsart1_Commend()
{
	UART_T *pUart = &g_tUart1;
  static COMMAND_RECEIVE_STATUS_t command_status = WAITING_COMMAND_SYNC_FLAG;
  uint8_t receive_data;
  static uint32_t command_data_length=0;
	while(Uart1_command[0]==0)//直到得到一组指令
	{
		if(pUart->usRxRead != pUart->usRxWrite) /////缓冲区不空
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
 					if(command_data_length >=UART1_COMMEND_SIZE) /////接收数据可能错了，重新找同步头
 					{
 						command_status = WAITING_COMMAND_SYNC_FLAG; 
 					}
 					else ////校验字
 					{
						
 						Uart1_command[command_data_length] = receive_data;	
 						//指示接收到一个新的命令
						if(command_data_length>=(Uart1_command[3]+3))//00 5A A5 +字节数+指令+地址+数据
						{
							Uart1_command[0] = 1;		
							//此处post信号量
							
							
							command_status = WAITING_COMMAND_SYNC_FLAG; /////接收到一个完整的命令，重新开始准备检测下一个命令同步头
						}
 					}
 				}
 				break;
 				default: break;
 			} 
		}
		else
			//**没有收到数据则跳出循环
			break;
	}
}
///*从串口2里获取指令*/
//void GetUsart2_Commend()
//{
//	#if UART2_FIFO_EN == 1
//	 UART_T *pUart = &g_tUart2;
//   static COMMAND_RECEIVE_STATUS_t command_status = WAITING_COMMAND_SYNC_FLAG;
//   uint8_t receive_data;
//   static uint32_t command_data_length=0;
//  while(Uart2_command[0]==0)
//  if(pUart->usRxRead != pUart->usRxWrite) /////缓冲区不空
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
//				if(command_data_length >= USART2_SERIAL_COMMAND_MAX_LENGTH) ////接收数据可能错了，重新找同步头
//				{
//					command_status = WAITING_COMMAND_SYNC_FLAG; 
//				}				
//				else 
//				{
//					if((receive_data ==0x0a)||(receive_data ==0x0d)) ////命令字符串结束符
//					{					
//						Uart2_command[command_data_length] = receive_data;
//							
//						////指示接收到一个新的命令
//						Uart2_command[0] = 1;		
//						//OSSemPost(Usart2Serial_Command);
//						command_status = WAITING_COMMAND_SYNC_FLAG; /////接收到一个完整的命令，重新开始准备检测下一个命令同步头
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
  if(pUart->usRxRead != pUart->usRxWrite) /////缓冲区不空
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
				if(receive_data==0x03||receive_data==0x06||receive_data==0x10)//读/写单个寄存器
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
				if(command_data_length >= 8) ////命令字符串结束符
				{
					if(Uart4_command[2]==0x06)
					{
						////指示接收到一个新的命令
						Uart4_command[0] = 1;		
						command_status = WAITING_COMMAND_SYNC_FLAG; /////接收到一个完整的命令，重新开始准备检测下一个命令同步头
					}
					else if(Uart4_command[2]==0x03)
					{
						if(command_data_length>=(Uart4_command[3]+4))
						{
							////指示接收到一个新的命令
							Uart4_command[0] = 1;		
							command_status = WAITING_COMMAND_SYNC_FLAG; /////接收到一个完整的命令，重新开始准备检测下一个命令同步头
						}
					}
					else if(Uart4_command[2]==0x010)
					{
						//if(command_data_length>=Uart4_command[7]+8)
						{
							////指示接收到一个新的命令
							Uart4_command[0] = 1;		
							command_status = WAITING_COMMAND_SYNC_FLAG; /////接收到一个完整的命令，重新开始准备检测下一个命令同步头
						}
					}
					else if(command_data_length>=UART4_COMMEND_SIZE)
					{
						command_status = WAITING_COMMAND_SYNC_FLAG; /////未接收到一个完整的命令，重新开始准备检测下一个命令同步头
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
		if(pUart->usRxRead != pUart->usRxWrite) /////缓冲区不空
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
					if(command_data_length >= UART3_COMMEND_SIZE) ////接收数据可能错了，重新找同步头
					{
											
							//Uart4_command[command_data_length] = receive_data;
								
							////指示接收到一个新的命令
							Uart3_command[0] = 1;		
							//OSSemPost(Usart4Serial_Command);
							command_status = WAITING_COMMAND_SYNC_FLAG; /////接收到一个完整的命令，重新开始准备检测下一个命令同步头
						
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

/*从串口2里获取指令*/
void GetUsart6_Commend()
{
	#if UART6_FIFO_EN == 1
	 UART_T *pUart = &g_tUart6;
   static COMMAND_RECEIVE_STATUS_t command_status = WAITING_COMMAND_SYNC_FLAG;
   uint8_t receive_data;
   static uint32_t command_data_length=0;
  while(Uart6_command[0]==0)
  if(pUart->usRxRead != pUart->usRxWrite) /////缓冲区不空
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
				if(command_data_length >= USART6_SERIAL_COMMAND_MAX_LENGTH) ////接收数据可能错了，重新找同步头
				{
					command_status = WAITING_COMMAND_SYNC_FLAG; 
				}				
				else 
				{
					if((receive_data ==0x0a)||(receive_data ==0x0d)) ////命令字符串结束符
					{					
						Uart6_command[command_data_length] = receive_data;
							
						////指示接收到一个新的命令
						Uart6_command[0] = 1;		
						//OSSemPost(Usart2Serial_Command);
						command_status = WAITING_COMMAND_SYNC_FLAG; /////接收到一个完整的命令，重新开始准备检测下一个命令同步头
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
/*从串口5里获取指令*/
void GetUsart2_Commend()
{
	#if UART2_FIFO_EN == 1
	 UART_T *pUart = &g_tUart2;
//   static COMMAND_RECEIVE_STATUS_t command_status = WAITING_COMMAND_SYNC_FLAG;
//   uint8_t receive_data;
//   static uint32_t command_data_length=0;
  while(Uart2_command[0]==0)
  if(pUart->usRxRead != pUart->usRxWrite) /////缓冲区不空
  {
		MODS_ReciveNew( pUart->pRxBuf[pUart->usRxRead++]);
		if (pUart->usRxRead >= pUart->usRxBufSize) pUart->usRxRead = 0;  
	}
	else 
		break;
	#endif
}

