#include "App.h"
#include "beionfluid_s60.h"
#include "CRC.h"
#include "Bsp.h"

//u8 Valve_ON[8] = {0x50,0x05,0x00,0x08,0xFF,0x00,0xFF,0xFF};//���� �����������Ҫ��������С���ط�����������ע�俪�������������Ҫ�ط�
//u8 Valve_OFF[8] = {0x50,0x05,0x00,0x08,0x00,0x00,0xFF,0xFF};//�ط�
//u8 Inquire[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};//��ѯ���ɻ�ȡ�豸�ź͹���ģʽ  
//u8 Get_Motor_DATA[8] = {0x50,0x03,0x00,0x09,0x00,0x00,0xFF,0xFF};//50 03 00 09 00 00 FF FF  �����λ��
////u8 Goto_coordinate[8] = {0x50,0x06,0x00,0x09,0x00,0x00,0xFF,0xFF}; //����ĳ������
//u8 Get_coordinate[8]= {0x50,0x03,0x00,0x09,0x00,0x00,0xFF,0xFF};//�õ������ǰλ��
//u8 PUL_DIR[8] = {0x50,0x06,0x00,0x02,0x00,0x3C,0x00,0x00};//�������ģʽ

LEVI2043E levi_2043e;
//u8 Action[8]={0x01,0x06,0x9E,0x34,0x00,0x03,0xA7,0xED};
u8 Action[8]={0x01,0x06,0x9C,0x71,0x00,0x02,0x76,0x40};  //40049  

u8 Stop[8] = {0x01,0x06,0x9C,0x71,0x00,0x08,0xF6,0x47};// ֹͣ01 06 9E 34 00 64 E6 07 
u8 empty_L[8] = {0x01,0x06,0x9C,0x71,0x00,0x09,0x37,0x87};
u8 empty_R[8] = {0x01,0x06,0x9C,0x71,0x00,0x0A,0x77,0x86};

//u8 valve[8] = {0x01,0x06,0x9E,0x34,0x00,0x61,0x26,0x04};//01 06 9E 34 00 61 26 04

//u8 Parameter_set[27] = {0x01,0x10,0x9C,0x69,0x00,0x09,0x12,0x00,0x01,0x00,0x01,0x13,0x88,0x00,0x00,0x00,0x00,0x07,0xD0,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00};//5000ml���� ��λml,ʱ��min
// 40040 �Ĵ�����ʼ                                           ģʽ       ����2�Ĵ���         ��λ       �ٶ�2�Ĵ���           ��λ2�Ĵ���       �ų�ѭ��    CRC
u8 Parameter_set[27] = {0x01,0x10,0x9C,0x68,0x00,0x09,0x12,0x00,0x01,0x1F,0x40,0x00,0x00,0x00,0x00,0x07,0xD0,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00};//5000ml���� 2�ٶ� ��λml,ʱ��min
//********************�豸�ţ�0x01��+ָ����(0x10)+�Ĵ���������0x0009��+�ֽ�����0x12��+����+У��
void Goto_coordinate()
{
	Get_crc(Parameter_set,25);
	comSendBuf(COM4,Parameter_set,27);//�趨����
	OSTimeDlyHMSM(0,0,0,100);
	comSendBuf(COM4,Parameter_set,27);//�趨����
//	OSTimeDlyHMSM(0,0,0,100);
//	comSendBuf(COM4,valve, 8);//�з�
//	OSTimeDlyHMSM(0,0,0,100);
//	comSendBuf(COM4,valve, 8);//�з�
	OSTimeDlyHMSM(0,0,0,500);
	comSendBuf(COM4,Action, 8);//ִ�ж���
	OSTimeDlyHMSM(0,0,0,200);
	comSendBuf(COM4,Action, 8);//ִ�ж���
	OSTimeDlyHMSM(0,0,0,200);
//	u8 command[8] = {0x50,0x06,0x00,0x09,0x00,0x00,0xFF,0xFF}; //����ĳ������
//	if(coordinate>25000)//�ȵ�������״̬�ٵ���λ��
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
//ע��ö���
void Levi2043e(void *p_arg)
{
	(void)p_arg;
	u8 Search[8] ={0x01,0x03,0x9C,0x72,0x00,0x01,0x0A,0x41};//��ѯ����״̬
//	u8 Search[8] ={0x01,0x03,0x9E,0x98,0x00,0x08,0xEA,0x0B};//��ѯ����״̬
//	u8 Inside[8] ={0x01,0x03,0x9C,0x48,0x00,0x02,0x6A,0x4D};//01 03 9C 48 00 02 6A 4D //��ѯ�ھ�
	u16 pump_speed;
	int i=0;
	while(1)
	{
		if(i++&0x01)
			comSendBuf(COM4,Search, 8);
//		else
//			comSendBuf(COM4,Inside, 8);
		OSTimeDlyHMSM(0,0,0,200);
		
		if(model_1006.start_product[AUTO_CLEAN_STATUS] == 0x02)  //�Զ���ϴ�ж�
		{
			if(levi_2043e.command_status==0)
			{
					RADIOTUBE_OFF;//�رյ�ŷ�
					OSTimeDlyHMSM(0,0,0,200);
//					model_1006.home_page[HOME_WORK_STATUS] = 0;//�ر��Զ���ϴ
					model_1006.start_product[AUTO_CLEAN_STATUS] = 0x01; //��ϴҳ��ʾ���
					comSendBuf(COM4,Stop, 8);//ͣ��
					levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��

			}
		}
		if(model_1006.start_product[HAND_CLEAN_STATUS] == 0x02) //�ֶ���ϴ�ж�
		{
			if(levi_2043e.command_status==0)
			{
					RADIOTUBE_OFF;//�رյ�ŷ�
					OSTimeDlyHMSM(0,0,0,200);
					model_1006.start_product[HAND_CLEAN_STATUS] = 0x01; //��ϴҳ��ʾ���
					comSendBuf(COM4,Stop, 8);//ͣ��
				 levi_2043e.command_status = 1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��

			}
		}
				//******************************�Զ���·��ϴ********************************
			//******************************�Զ���·��ϴ********************************
		if(model_1006.start_product[START_INJECTION_COMMEND]&0x01)//0x01�Զ���ϴ����ָ��
		{
//			model_1006.home_page[HOME_WORK_STATUS] = 0x03;//��ҳ��ʾ�Զ���ϴ
			model_1006.start_product[AUTO_CLEAN_STATUS] = 0x02; //��ϴҳ��ʾ����
//			model_1006.start_product[START_PRODUCT_COMMEND] |= 0x0400;//ִ���Զ���ϴ
						RADIOTUBE_ON;//�򿪵�ŷ�
//			if(levi_2043e.motor_status)
//			{
//				comSendBuf(COM4,Stop, 8);//ͣ��
//				OSTimeDlyHMSM(0,0,0,500);
//			}
			Parameter_set[8]  = 0x02;//�趨����
			Parameter_set[9]  = 0x1F;//ע�������λ  
			Parameter_set[10]  = 0x40;//ע�������λ  // 8ml
//			Parameter_set[8]  = 0x01;//�� ��1�ŷ�
//			Parameter_set[10]  = 0x01;//��
			Parameter_set[15]  = 0x13;//���ٸ� Ĭ��5000ml/min
			Parameter_set[16]  = 0x88;//���ٵ�
			Goto_coordinate();
//			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x0400;
			model_1006.start_product[START_INJECTION_COMMEND] &= ~(0x01);//��־λ����
//			auto_injection_count = 0;
			levi_2043e.command_status=1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��
		}

		
		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x01)//�ֶ���·������Һ ��������ȷ��Ҫ�󲻸�
		{
			model_1006.start_product[HAND_CLEAN_STATUS] = 0x02; //��ϴҳ��ʾ����
			RADIOTUBE_ON;//�򿪵�ŷ�
//			if(levi_2043e.motor_status)
//			{
//				comSendBuf(COM4,Stop, 8);//ͣ��
//				OSTimeDlyHMSM(0,0,0,500);
//			}
			pump_speed = model_1006.start_product[PUMP_SPEED_SET]*model_1006.calibration[PUMP_CALIBRATION_FACTOR]/1000;
			Parameter_set[8]  = 0x02;//�趨����ģʽ
			Parameter_set[9]  = model_1006.start_product[HAND_CLEAN_ML]>>8;//ע�������λ  
			Parameter_set[10]  = model_1006.start_product[HAND_CLEAN_ML]&0xFF;//ע�������λ 
			Parameter_set[15]  = pump_speed>>8;//model_1006.start_product[PUMP_SPEED_SET_PULL]>>8;//���ٸ�
			Parameter_set[16]  = pump_speed&0xff;//model_1006.start_product[PUMP_SPEED_SET_PULL]&0xFF;//���ٵ�
			Goto_coordinate();
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x01;
			levi_2043e.command_status=1;//ǿ�Ƹı�ָ��״̬�ȴ�״̬ˢ��
		}
		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x80)//�ֶ���ϴ�ų�
		{
			comSendBuf(COM4,empty_L, 8);//�ſ�
			OSTimeDlyHMSM(0,0,0,500);
			comSendBuf(COM4,empty_R, 8);//
  		OSTimeDlyHMSM(0,0,0,500);
			model_1006.start_product[HAND_CLEAN_STATUS] = 0x01; //��ϴҳ��ʾ���
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x80;
		}
		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x0200)//ע���У׼
		{
			RADIOTUBE_OFF;//�رյ�ŷ�
//			if(levi_2043e.motor_status)
//			{
//				comSendBuf(COM4,Stop, 8);//ͣ��
//				OSTimeDlyHMSM(0,0,0,500);
//			}
			Parameter_set[8]  = 0x02;//��������ģʽ
//			Parameter_set[10]  = 0x00;//��
			Parameter_set[9]  = model_1006.calibration[PUMP_BULK_ML]>>8;//ע�������λ  
			Parameter_set[10]  = model_1006.calibration[PUMP_BULK_ML]&0xFF;//ע�������λ 
			Parameter_set[15]  = model_1006.calibration[PUMP_FLOW]>>8;//���ٸ�
			Parameter_set[16]  = model_1006.calibration[PUMP_FLOW]&0xFF;//���ٵ�
			Goto_coordinate();
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x0200;
			//*************�ָ�8000ml��Ĭ�����
			Parameter_set[9]  = 0x1F;//ע�������λ  
			Parameter_set[10]  = 0x40;//ע�������λ
		}
//		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x0400)//ע����Զ���ϴ
//		{
//			RADIOTUBE_ON;//�򿪵�ŷ�
////			if(levi_2043e.motor_status)
////			{
////				comSendBuf(COM4,Stop, 8);//ͣ��
////				OSTimeDlyHMSM(0,0,0,500);
////			}
//			Parameter_set[8]  = 0x02;//�趨����
//			Parameter_set[9]  = 0x1F;//ע�������λ  
//			Parameter_set[10]  = 0x40;//ע�������λ  // 8ml
////			Parameter_set[8]  = 0x01;//�� ��1�ŷ�
////			Parameter_set[10]  = 0x01;//��
//			Parameter_set[15]  = 0x13;//���ٸ� Ĭ��5000ml/min
//			Parameter_set[16]  = 0x88;//���ٵ�
//			Goto_coordinate();
//			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x0400;
//		}
		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x02)//���ӷ���
		{
			RADIOTUBE_OFF;//�رյ�ŷ�
//			if(levi_2043e.motor_status)
//			{
//				comSendBuf(COM4,Stop, 8);//ͣ��
//				OSTimeDlyHMSM(0,0,0,500);
//			}
//			pump_speed = model_1006.start_product[PUMP_SPEED_SET_PULL]*model_1006.calibration[PUMP_CALIBRATION_FACTOR]/1000;
			pump_speed = model_1006.start_product[FLUID_FLOW]*model_1006.calibration[PUMP_CALIBRATION_FACTOR]/1000;//*model_1006.set[SLOPE_COEFFICIENTS]/1000000;
			Parameter_set[8]  = 0x01;//����ģʽ
//			Parameter_set[8]  = 0x01;//�� ��1�ŷ�
//			Parameter_set[10]  = 0x01;//��
			Parameter_set[15]  = pump_speed>>8;//model_1006.start_product[PUMP_SPEED_SET_PULL]>>8;//���ٸ�
			Parameter_set[16]  = pump_speed&0xff;//model_1006.start_product[PUMP_SPEED_SET_PULL]&0xFF;//���ٵ�
			Goto_coordinate();
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x02;
		}
//		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x1000)//ע����Զ���ϴ��Һ
//		{
//			RADIOTUBE_OFF;//�رյ�ŷ�
//			if(levi_2043e.motor_status)
//			{
//				comSendBuf(COM4,Stop, 8);//ͣ��
//				OSTimeDlyHMSM(0,0,0,500);
//			}
//			Parameter_set[8]  = 0x01;//�� ��1�ŷ�
//			Parameter_set[10]  = 0x01;//��
//			Parameter_set[17]  = 0x13;//���ٸ� Ĭ��5000ml/min
//			Parameter_set[18]  = 0x88;//���ٵ�
//			Goto_coordinate();
//			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x1000;
//		}
		else if(model_1006.start_product[START_PRODUCT_COMMEND]&0x04)
		{
			RADIOTUBE_OFF;//�رյ�ŷ�
//			if(levi_2043e.motor_status)
//			{
				comSendBuf(COM4,Stop, 8);//ͣ��
				OSTimeDlyHMSM(0,0,0,200);
//			}
			model_1006.start_product[START_PRODUCT_COMMEND] &= ~0x04;
			model_1006.start_product[AUTO_CLEAN_STATUS] = 0x00; //�����ϴҳ��ʾ
			model_1006.start_product[HAND_CLEAN_STATUS] = 0x00; //�����ϴҳ��ʾ
		}
		else
		{
			OSTimeDlyHMSM(0,0,0,200);
		}
		
	}
}
