#ifndef __SERIAL_COMMAND_H 
#define __SERIAL_COMMAND_H 
#include "stdint.h"


typedef struct
{
	char addr;
	float pressure;
	float temp;
	float cubage;
	float quality;
	float set_value;
	char gas_name[8];
	char new_data;
}Alicat;

#define SERIAL_COMMAND_MAX_LENGTH         60
#define USART2_SERIAL_COMMAND_MAX_LENGTH  50 

#define USART3_SERIAL_COMMAND_MAX_LENGTH  50 
#define USART4_SERIAL_COMMAND_MAX_LENGTH  50 
#define USART6_SERIAL_COMMAND_MAX_LENGTH  50 
#define SERIAL_COMMAND_HOLD_LENGTH        20
#define AGC_DATA_MAX_LENGHT               90
extern Alicat Alic1;
extern uint8_t AGC_data_word[];
extern uint8_t master_serial_command_word[];
void Serial_MasterCommandSend(uint8_t command_code,int command_data);
//void Serial_MasterCommandProcess(void);
void Serial_MasterCommandProcess_NEW(void);
void BluetoothCommandProcess(void);
void DealUsart3_Commend(void);
void DealSerial_Command(void *p_arg);
void Get_AGC_data(void);
#endif

