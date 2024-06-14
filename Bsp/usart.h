#ifndef _USART_H_
#define _USART_H_

#include <stdio.h>
#include "stm32f4xx.h"
#include <stdio.h>

#define  CPU_IntDis()           { __set_PRIMASK(1); }	/* ���ж� */
#define  CPU_IntEn()            { __set_PRIMASK(0); }	/* ���ж� */

#define	UART1_FIFO_EN	1
#define	UART2_FIFO_EN	1

#define	UART3_FIFO_EN	1

#define	UART4_FIFO_EN	1
#define	UART5_FIFO_EN	0
#define	UART6_FIFO_EN	1

#if UART1_FIFO_EN == 1
	#define UART1_BAUD			115200
	#define UART1_TX_BUF_SIZE	1*512
	#define UART1_RX_BUF_SIZE	1*512
	#define UART1_COMMEND_SIZE 200
#endif

#if UART2_FIFO_EN == 1
	#define UART2_BAUD			115200
	#define UART2_TX_BUF_SIZE	1*512
	#define UART2_RX_BUF_SIZE	1*512
	#define UART2_COMMEND_SIZE 100
#endif

#if UART3_FIFO_EN == 1
	#define UART3_BAUD			 9600
	#define UART3_TX_BUF_SIZE	1*512
	#define UART3_RX_BUF_SIZE	1*512
	#define UART3_COMMEND_SIZE 11
#endif

#if UART4_FIFO_EN == 1
	#define UART4_BAUD			9600
	#define UART4_TX_BUF_SIZE	1*512
	#define UART4_RX_BUF_SIZE	1*512
	#define UART4_COMMEND_SIZE  50
#endif

#if UART5_FIFO_EN == 1
	#define UART5_BAUD			9600
	#define UART5_TX_BUF_SIZE	1*200
	#define UART5_RX_BUF_SIZE	1*200
	#define UART5_COMMEND_SIZE  60
#endif

#if UART6_FIFO_EN == 1
	#define UART6_BAUD			19200
	#define UART6_TX_BUF_SIZE	1*512
	#define UART6_RX_BUF_SIZE	1*512
	#define UART6_COMMEND_SIZE  100
#endif

typedef enum
{
	COM1 = 0,	/* USART1  PA9, PA10 */
	COM2 = 1,	/* USART2, PA2, PA3 */
	COM3 = 2,	/* USART3, PB10, PB11 */
	COM4 = 3,	/* UART4, PC10, PC11 */
	COM5 = 4,	/* UART5, PC12,PD2 */
	COM6 = 5	/* UART5, PC12,PD2 */
}COM_PORT_E;

/* �����豸�ṹ�� */
typedef struct
{
	USART_TypeDef *uart;		/* STM32�ڲ������豸ָ�� */
	uint8_t *pCommandData;
	uint8_t *pTxBuf;			/* ���ͻ����� */
	uint8_t *pRxBuf;			/* ���ջ����� */
	uint16_t usTxBufSize;		/* ���ͻ�������С */
	uint16_t usRxBufSize;		/* ���ջ�������С */
	uint16_t usTxWrite;			/* ���ͻ�����дָ�� */
	uint16_t usTxRead;			/* ���ͻ�������ָ�� */
	uint16_t usRxWrite;			/* ���ջ�����дָ�� */
	uint16_t usRxRead;			/* ���ջ�������ָ�� */
	void (*SendBefor)(void); 	/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	void (*ReciveNew)(void);	/* �����յ����ݵĻص�����ָ�� */
	
}UART_T;

typedef enum
{
	WAITING_COMMAND_SYNC_FLAG = 0,
	FIND_COMMAND_HEAD,

 	WAITING_COMMAND_DATA,
	WAITING_WRITE_COMMAND_DATA,
	WAITING_READ_COMMAND_DATA,
	WAITING_COMMAND_END		
}COMMAND_RECEIVE_STATUS_t;


void bsp_InitUart(void);
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(uint8_t _ucPort, uint8_t *_pByte);
void process_Uart1_command_data(void);
void process_Uart2_command_data(void);
void process_Uart2_command_data_NEW(void);
void process_Uart3_command_data(void);
int usar_3(int x);


/* ����ÿ�����ڽṹ����� */
#if UART1_FIFO_EN == 1
	extern UART_T g_tUart1;
	extern  uint8_t g_TxBuf1[UART1_TX_BUF_SIZE];		/* ���ͻ����� */
	extern uint8_t g_RxBuf1[UART1_RX_BUF_SIZE];		/* ���ջ����� */
	extern uint8_t  Uart1_command[UART1_COMMEND_SIZE+1];
#endif

#if UART2_FIFO_EN == 1
	extern UART_T g_tUart2;
	extern uint8_t g_TxBuf2[UART2_TX_BUF_SIZE];		/* ���ͻ����� */
	extern uint8_t g_RxBuf2[UART2_RX_BUF_SIZE];		/* ���ջ����� */
	extern uint8_t Uart2_command[UART2_COMMEND_SIZE+1];
#endif

#if UART3_FIFO_EN == 1
	extern UART_T g_tUart3;
	extern uint8_t g_TxBuf3[UART3_TX_BUF_SIZE];		/* ���ͻ����� */
	extern uint8_t g_RxBuf3[UART3_RX_BUF_SIZE];		/* ���ջ����� */
	extern uint8_t Uart3_command[UART3_COMMEND_SIZE+1];
#endif

#if UART4_FIFO_EN == 1
	extern UART_T g_tUart4;
	extern uint8_t g_TxBuf4[UART4_TX_BUF_SIZE];		/* ���ͻ����� */
	extern uint8_t g_RxBuf4[UART4_RX_BUF_SIZE];		/* ���ջ����� */
	extern uint8_t Uart4_command[UART4_COMMEND_SIZE+1];
#endif

#if UART5_FIFO_EN == 1
	extern UART_T g_tUart5;
	extern uint8_t g_TxBuf5[UART5_TX_BUF_SIZE];		/* ���ͻ����� */
	extern uint8_t g_RxBuf5[UART5_RX_BUF_SIZE];		/* ���ջ����� */
	extern uint8_t Uart5_command[UART5_COMMEND_SIZE+1];
#endif

#if UART6_FIFO_EN == 1
	extern UART_T g_tUart6;
	extern uint8_t g_TxBuf6[UART6_TX_BUF_SIZE];		/* ���ͻ����� */
	extern uint8_t g_RxBuf6[UART6_RX_BUF_SIZE];		/* ���ջ����� */
	extern uint8_t Uart6_command[UART6_COMMEND_SIZE+1];
#endif

#endif /*_USART_H*/

