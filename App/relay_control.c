#include "app.h"
#include "CRC.h"

void Set_relay_onoff(u8 addr,u8 data)
{
	u8 commd_data[10];
	commd_data[0] = addr;
	commd_data[1] = 0x0f;//功能码：写多个线圈
	commd_data[2] = 0x00;
	commd_data[3] = 0x00;
	commd_data[4] = 0x00;
	commd_data[5] = 0x08;//控制八路
	commd_data[6] = 0x01;//数据字节数
	commd_data[7] = data;
	//commd_data[8] = 0x0f;//CRC校验位H
	//commd_data[9] = 0x0f;//CRC校验位L
	Get_crc(commd_data,8);
	comSendBuf(COM2,commd_data,10);//发送指令
}
