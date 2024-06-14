#include "App.h"
#include "beionfluid_s60.h"
#include "CRC.h"
#include "Bsp.h"

//u8 Valve_ON[8] = {0x50,0x05,0x00,0x08,0xFF,0x00,0xFF,0xFF};//开阀 步距往大调需要开发，往小调关阀。即泵往外注射开阀，往泵里抽需要关阀
//u8 Valve_OFF[8] = {0x50,0x05,0x00,0x08,0x00,0x00,0xFF,0xFF};//关阀
//u8 Inquire[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//查询，可获取设备号和工作模式  
//u8 Get_Motor_DATA[8] = {0x50,0x03,0x00,0x09,0x00,0x00,0xFF,0xFF};//50 03 00 09 00 00 FF FF  读电机位置
////u8 Goto_coordinate[8] = {0x50,0x06,0x00,0x09,0x00,0x00,0xFF,0xFF}; //到达某个坐标
//u8 Get_coordinate[8]= {0x50,0x03,0x00,0x09,0x00,0x00,0xFF,0xFF};//得到电机当前位置
//u8 PUL_DIR[8] = {0x50,0x06,0x00,0x02,0x00,0x3C,0x00,0x00};//脉冲控制模式

LEVI2043E levi_2043e;
//u8 Action[8]={0x01,0x06,0x9E,0x34,0x00,0x03,0xA7,0xED};
u8 Action[8]={0x01,0x06,0x9C,0x71,0x00,0x02,0x76,0x40};  //40049  

u8 Stop[8] = {0x01,0x06,0x9C,0x71,0x00,0x08,0xF6,0x47};// 停止01 06 9E 34 00 64 E6 07 
u8 empty_L[8] = {0x01,0x06,0x9C,0x71,0x00,0x09,0x37,0x87};
u8 empty_R[8] = {0x01,0x06,0x9C,0x71,0x00,0x0A,0x77,0x86};

//u8 valve[8] = {0x01,0x06,0x9E,0x34,0x00,0x61,0x26,0x04};//01 06 9E 34 00 61 26 04

//u8 Parameter_set[27] = {0x01,0x10,0x9C,0x69,0x00,0x09,0x12,0x00,0x01,0x00,0x01,0x13,0x88,0x00,0x00,0x00,0x00,0x07,0xD0,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00};//5000ml量程 单位ml,时间min
// 40040 寄存器开始                                           模式       容量2寄存器         单位       速度2寄存器           单位2寄存器       排充循环    CRC
u8 Parameter_set[27] = {0x01,0x10,0x9C,0x68,0x00,0x09,0x12,0x00,0x01,0x1F,0x40,0x00,0x00,0x00,0x00,0x07,0xD0,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00};//5000ml量程 2速度 单位ml,时间min
//********************设备号（0x01）+指令码(0x10)+寄存器数量（0x0009）+字节数（0x12）+数据+校验
void Goto_coordinate()
{
	Get_crc(Parameter_set,25);
	comSendBuf(COM4,Parameter_set,27);//设定参数
	OSTimeDlyHMSM(0,0,0,100);
	comSendBuf(COM4,Parameter_set,27);//设定参数
//	OSTimeDlyHMSM(0,0,0,100);
//	comSendBuf(COM4,valve, 8);//切阀
//	OSTimeDlyHMSM(0,0,0,100);
//	comSendBuf(COM4,valve, 8);//切阀
	OSTimeDlyHMSM(0,0,0,500);
	comSendBuf(COM4,Action, 8);//执行动作
	OSTimeDlyHMSM(0,0,0,200);
	comSendBuf(COM4,Action, 8);//执行动作
	OSTimeDlyHMSM(0,0,0,200);
//	u8 command[8] = {0x50,0x06,0x00,0x09,0x00,0x00,0xFF,0xFF}; //到达某个坐标
//	if(coordinate>25000)//先调整阀的状态再调整位置
//	{
//		//comSendBuf(COM4,Valve_ON, 8);
//		OSTimeDlyHMSM(0,0,0,5);
//	}
//	else
//	{
//		//comSendBuf(COM4,Valve_OFF, 8);
//		OSTimeDlyHMSM(0,0,0,5);
//	}
//	OSTimeDlyHMSM(0,0,0,10);
//	command[4] = (coordinate>>8)&0xff;
//	command[5] = coordinate&0xff;
//	comSendBuf(COM4,command, 8);
}
//注射泵动作
void Levi2043e(void *p_arg)
{
	(void)p_arg;
	u8 Search[8] ={0x01,0x03,0x9C,0x72,0x00,0x01,0x0A,0x41};//查询工作状态
//	u8 Search[8] ={0x01,0x03,0x9E,0x98,0x00,0x08,0xEA,0x0B};//查询工作状态
//	u8 Inside[8] ={0x01,0x03,0x9C,0x48,0x00,0x02,0x6A,0x4D};//01 03 9C 48 00 02 6A 4D //查询内径
	u16 pump_speed;
	int i=0;
	while(1)
	{
		if(i++&0x01)
			comSendBuf(COM4,Search, 8);
//		else
//			comSendBuf(COM4,Inside, 8);
		OSTimeDlyHMSM(0,0,0,200);
		
		if(model_1006.start_product[AUTO_CLEAN_STATUS] == 0x02)  //自动清洗判定
		{
			if(levi_2043e.command_status==0)
			{
					RADIOTUBE_OFF;//关闭电磁阀
					OSTimeDlyHMSM(0,0,0,200);
//					model_1006.home_page[HOME_WORK_STATUS] = 0;//关闭自动清洗
					model_1006.start_product[AUTO_CLEAN_STATUS] = 0x01; //清洗页显示完成
					comSendBuf(COM4,Stop, 8);//停机
					levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新

			}
		}
		if(model_1006.start_product[HAND_CLEAN_STATUS] == 0x02) //手动清洗判定
		{
			if(levi_2043e.command_status==0)
			{
					RADIOTUBE_OFF;//关闭电磁阀
					OSTimeDlyHMSM(0,0,0,200);
					model_1006.start_product[HAND_CLEAN_STATUS] = 0x01; //清洗页显示完成
					comSendBuf(COM4,Stop, 8);//停机
				 levi_2043e.command_status = 1;//强制改变指令状态等待状态刷新

			}
		}
				//******************************自动管路清洗********************************
			//******************************自动管路清洗********************************
		if(model_1006.start_product[START_INJECTION_COMMEND]&0x01)//0x01自动清洗启动指令
		{
//			model_1006.home_page[HOME_WORK_STATUS] = 0x03;//首页显示自动清洗
			model_1006.start_product[AUTO_CLEAN_STATUS] = 0x02; //清洗页显示运行
//			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x0400;//执行自动清洗
						RADIOTUBE_ON;//打开电磁阀
//			if(levi_2043e.motor_status)
//			{
//				comSendBuf(COM4,Stop, 8);//停机
//				OSTimeDlyHMSM(0,0,0,500);
//			}
			Parameter_set[8]  = 0x02;//设定容量
			Parameter_set[9]  = 0x1F;//注射体积高位  
			Parameter_set[10]  = 0x40;//注射体积低位  // 8ml
//			Parameter_set[8]  = 0x01;//吸 切1号阀
//			Parameter_set[10]  = 0x01;//拉
			Parameter_set[15]  = 0x13;//流速高 默认5000ml/min
			Parameter_set[16]  = 0x88;//流速低
			Goto_coordinate();
//			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x0400;
			model_1006.start_product[START_INJECTION_COMMEND] &= ~(0x01);//标志位清零
//			auto_injection_count = 0;
			levi_2043e.command_status=1;//强制改变指令状态等待状态刷新
		}

		
		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x01)//手动管路清晰排液 对流量精确度要求不高
		{
			model_1006.start_product[HAND_CLEAN_STATUS] = 0x02; //清洗页显示运行
			RADIOTUBE_ON;//打开电磁阀
//			if(levi_2043e.motor_status)
//			{
//				comSendBuf(COM4,Stop, 8);//停机
//				OSTimeDlyHMSM(0,0,0,500);
//			}
			pump_speed = model_1006.start_product[PUMP_SPEED_SET]*model_1006.calibration[PUMP_CALIBRATION_FACTOR]/1000;
			Parameter_set[8]  = 0x02;//设定容量模式
			Parameter_set[9]  = model_1006.start_product[HAND_CLEAN_ML]>>8;//注射体积高位  
			Parameter_set[10]  = model_1006.start_product[HAND_CLEAN_ML]&0xFF;//注射体积低位 
			Parameter_set[15]  = pump_speed>>8;//model_1006.start_product[PUMP_SPEED_SET_PULL]>>8;//流速高
			Parameter_set[16]  = pump_speed&0xff;//model_1006.start_product[PUMP_SPEED_SET_PULL]&0xFF;//流速低
			Goto_coordinate();
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x01;
			levi_2043e.command_status=1;//强制改变指令状态等待状态刷新
		}
		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x80)//手动清洗排充
		{
			comSendBuf(COM4,empty_L, 8);//排空
			OSTimeDlyHMSM(0,0,0,500);
			comSendBuf(COM4,empty_R, 8);//
  		OSTimeDlyHMSM(0,0,0,500);
			model_1006.start_product[HAND_CLEAN_STATUS] = 0x01; //清洗页显示完成
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x80;
		}
		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x0200)//注射泵校准
		{
			RADIOTUBE_OFF;//关闭电磁阀
//			if(levi_2043e.motor_status)
//			{
//				comSendBuf(COM4,Stop, 8);//停机
//				OSTimeDlyHMSM(0,0,0,500);
//			}
			Parameter_set[8]  = 0x02;//设置容量模式
//			Parameter_set[10]  = 0x00;//推
			Parameter_set[9]  = model_1006.calibration[PUMP_BULK_ML]>>8;//注射体积高位  
			Parameter_set[10]  = model_1006.calibration[PUMP_BULK_ML]&0xFF;//注射体积低位 
			Parameter_set[15]  = model_1006.calibration[PUMP_FLOW]>>8;//流速高
			Parameter_set[16]  = model_1006.calibration[PUMP_FLOW]&0xFF;//流速低
			Goto_coordinate();
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x0200;
			//*************恢复8000ml的默认体积
			Parameter_set[9]  = 0x1F;//注射体积高位  
			Parameter_set[10]  = 0x40;//注射体积低位
		}
//		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x0400)//注射泵自动清洗
//		{
//			RADIOTUBE_ON;//打开电磁阀
////			if(levi_2043e.motor_status)
////			{
////				comSendBuf(COM4,Stop, 8);//停机
////				OSTimeDlyHMSM(0,0,0,500);
////			}
//			Parameter_set[8]  = 0x02;//设定容量
//			Parameter_set[9]  = 0x1F;//注射体积高位  
//			Parameter_set[10]  = 0x40;//注射体积低位  // 8ml
////			Parameter_set[8]  = 0x01;//吸 切1号阀
////			Parameter_set[10]  = 0x01;//拉
//			Parameter_set[15]  = 0x13;//流速高 默认5000ml/min
//			Parameter_set[16]  = 0x88;//流速低
//			Goto_coordinate();
//			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x0400;
//		}
		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x02)//粒子发生
		{
			RADIOTUBE_OFF;//关闭电磁阀
//			if(levi_2043e.motor_status)
//			{
//				comSendBuf(COM4,Stop, 8);//停机
//				OSTimeDlyHMSM(0,0,0,500);
//			}
//			pump_speed = model_1006.start_product[PUMP_SPEED_SET_PULL]*model_1006.calibration[PUMP_CALIBRATION_FACTOR]/1000;
			pump_speed = model_1006.start_product[FLUID_FLOW]*model_1006.calibration[PUMP_CALIBRATION_FACTOR]/1000;//*model_1006.set[SLOPE_COEFFICIENTS]/1000000;
			Parameter_set[8]  = 0x01;//连续模式
//			Parameter_set[8]  = 0x01;//吸 切1号阀
//			Parameter_set[10]  = 0x01;//拉
			Parameter_set[15]  = pump_speed>>8;//model_1006.start_product[PUMP_SPEED_SET_PULL]>>8;//流速高
			Parameter_set[16]  = pump_speed&0xff;//model_1006.start_product[PUMP_SPEED_SET_PULL]&0xFF;//流速低
			Goto_coordinate();
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x02;
		}
//		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x1000)//注射泵自动清洗吸液
//		{
//			RADIOTUBE_OFF;//关闭电磁阀
//			if(levi_2043e.motor_status)
//			{
//				comSendBuf(COM4,Stop, 8);//停机
//				OSTimeDlyHMSM(0,0,0,500);
//			}
//			Parameter_set[8]  = 0x01;//吸 切1号阀
//			Parameter_set[10]  = 0x01;//拉
//			Parameter_set[17]  = 0x13;//流速高 默认5000ml/min
//			Parameter_set[18]  = 0x88;//流速低
//			Goto_coordinate();
//			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x1000;
//		}
		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x04)
		{
			RADIOTUBE_OFF;//关闭电磁阀
//			if(levi_2043e.motor_status)
//			{
				comSendBuf(COM4,Stop, 8);//停机
				OSTimeDlyHMSM(0,0,0,200);
//			}
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x04;
			model_1006.start_product[AUTO_CLEAN_STATUS] = 0x00; //清除清洗页显示
			model_1006.start_product[HAND_CLEAN_STATUS] = 0x00; //清除清洗页显示
		}
		else
		{
			OSTimeDlyHMSM(0,0,0,200);
		}
		
	}
}
