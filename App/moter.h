#include "bsp.h"

#ifndef _MOTER_H_
#define _MOTER_H_

void Set_Moter_data(unsigned short addr,unsigned short data);
extern float write_hex_to_float(uint32_t number);
extern uint16_t CRC16_Modbus(uint8_t *_pBuf, uint16_t _usLen);
extern uint16_t BEBufToUint16(uint8_t *_pBuf);

#define SADDR	1


/* 03H 读保持寄存器 */
/* 06H 写保持寄存器 */
/* 10H 写多个保存寄存器 */
#define SLAVE_REG_P01		0x0301
#define SLAVE_REG_P02		0x0302

#define SLAVE_REG_P03		0x0303
#define SLAVE_REG_P04		0x0304

#define SLAVE_REG_P05		0x0305
#define SLAVE_REG_P06		0x0306

#define SLAVE_REG_P07		0x0307
#define SLAVE_REG_P08		0x0308

#define SLAVE_REG_P09		0x0309
#define SLAVE_REG_P10	  0x030A

#define SLAVE_REG_P11	  0x030B

#define SLAVE_REG_P12   0x030C
#define SLAVE_REG_P13	  0x030D

#define SLAVE_REG_P14   0x030E




/* RTU 应答代码 */
#define RSP_OK				0		/* 成功 */
#define RSP_ERR_CMD			0x01	/* 不支持的功能码 */
#define RSP_ERR_REG_ADDR	0x02	/* 寄存器地址错误 */
#define RSP_ERR_VALUE		0x03	/* 数据值域错误 */
#define RSP_ERR_WRITE		0x04	/* 写入失败 */

#define S_RX_BUF_SIZE		30
#define S_TX_BUF_SIZE		128




typedef struct
{
	uint8_t RxBuf[S_RX_BUF_SIZE];
	uint8_t RxCount;
	uint8_t RxStatus;
	uint8_t RxNewFlag;

	uint8_t RspCode;

	uint8_t TxBuf[S_TX_BUF_SIZE];
	uint8_t TxCount;
}MODS_T;

typedef struct
{
	/* 03H 06H 读写保持寄存器 */
	uint16_t flowA;
	uint16_t flowB;
	uint16_t flowC;
	uint16_t flowD;
	uint16_t flowE;
	


	uint16_t frequency;
	uint16_t duty;
	

	/* 04H 读取模拟量寄存器 */
	uint16_t signal;
	uint16_t pt1000;
	/* 01H 05H 读写单个强制线圈 */
	uint8_t D01;
	uint8_t D02;
	uint8_t D03;
	uint8_t D04;
	uint8_t D05;
	uint8_t D06;
	uint8_t D07;
	uint8_t D08;
	uint8_t D09;
	uint8_t D010;
	
}VAR_T;

extern MODS_T g_tModS;
extern VAR_T g_tVar;


void MODS_Poll(void);
void MODS_ReciveNew(uint8_t _byte);
void modsUpdateTask(void const * argument);

#endif
