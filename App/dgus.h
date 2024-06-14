#ifndef _DGUS_H_
#define _DGUS_H_

#define HOME_PAGE_LINE_LENGTH						600
#define TEMP_THRESHOLD								5
#define RAY_VOLTAGE_THRESHOLD_UP					9000
#define RAY_VOLTAGE_THRESHOLD_DOWN					-9000
#define PMT_VOLTAGE_THRESHOLD_UP					900
#define PMT_VOLTAGE_THRESHOLD_DOWN					200
#define GAS_FLOW_THRESHOLD_UP						1100
#define GAS_FLOW_THRESHOLD_DOWN						900
#define LIQUID_FLOW_THRESHOLD_UP					800
#define LIQUID_FLOW_THRESHOLD_DOWN					300


#define AUTO_STATUS								0x001B//�Զ�״̬
#define METHANAL_U								0x001C//��ȩ��λ
#define METHANAL_CONCENTRATION					0x001D//��ȩŨ��H
#define METHANAL_CONCENTRATION_L				0x001E//��ȩŨ��L

#define AUTO_ZERO								0x03//�Զ����
#define AUTO_ZERO_ON_OFF						0x0//�Զ���㿪��
#define AUTO_ZERO_STATUS						0x1//�Զ����״̬
#define AUTO_ZERO_TIME_LENGTH					0x2//�Զ����ʱ��
#define AUTO_ZERO_TIME_HOUR						0x3//�Զ����ʱ
#define AUTO_ZERO_TIME_MIN						0x4//�Զ�����
#define AUTO_ZERO_TIME_INTERVAL					0x5//�Զ����ʱ����

#define AUTO_FLUID								0x04//�Զ���Һ
#define AUTO_FLUID_ON_OFF						0x0//�Զ���Һ����
#define AUTO_FLUID_STATUS						0x1//�Զ���Һ״̬
#define AUTO_FLUID_CONCENTRATION				0x2//�Զ���ҺŨ��
#define AUTO_FLUID_ZERO_TIME_LENGTH				0x3//�Զ���Һʱ��
#define AUTO_FLUID_TIME_LENGTH					0x4//�Զ���Һʱ��
#define AUTO_FLUID_TIME_HOUR					0x5//�Զ���Һʱ
#define AUTO_FLUID_TIME_MIN						0x6//�Զ���Һ��
#define AUTO_FLUID_TIME_INTERVAL				0x7//�Զ���Һʱ����

#define AUTO_GAS								0x05//�Զ�����
#define AUTO_GAS_ON_OFF							0x0//�Զ���������
#define AUTO_GAS_STATUS							0x1//�Զ�����״̬
#define AUTO_GAS_CONCENTRATION					0x2//�Զ�����Ũ��
#define AUTO_GAS_ZERO_TIME_LENGTH				0x3//�Զ�����ʱ��
#define AUTO_GAS_TIME_LENGTH					0x4//�Զ�����ʱ��
#define AUTO_GAS_TIME_HOUR						0x5//�Զ�����ʱ
#define AUTO_GAS_TIME_MIN						0x6//�Զ�������
#define AUTO_GAS_TIME_INTERVAL					0x7//�Զ�����ʱ����

#define HAND_GAS								0x07//�ֶ���������
#define HAND_GAS_ON_OFF							0x0//�ֶ���������
#define HAND_GAS_ZERO_TIME_LENGTH				0x1//�ֶ�����ʱ��
#define HAND_GAS_CONCENTRATION_H				0x2//�ֶ�����Ũ��
#define HAND_GAS_CONCENTRATION_L				0x3//�ֶ�����Ũ��
#define HAND_GAS_METHANAL_CONCENTRATION_H		0x4//�ֶ�������ȩŨ��
#define HAND_GAS_METHANAL_CONCENTRATION_L		0x5//�ֶ�������ȩŨ��
#define HAND_GAS_RAY_VOLTAGE					0x6//�ֶ�����ӫ���ѹ
#define HAND_GAS_CALIBRATION					0x7//�ֶ���ҺУ׼
#define HAND_GAS_ABSORPTION_EFFI			0x8//�ֶ���������Ч��У׼

#define HAND_FLUID								0x08//�ֶ���Һ����
#define HAND_FLUID_ON_OFF						0x0//�ֶ���Һ����
#define HAND_FLUID_ZERO_TIME_LENGTH				0x1//�ֶ���Һʱ��
#define HAND_FLUID_CONCENTRATION_H				0x2//�ֶ���ҺŨ��
#define HAND_FLUID_CONCENTRATION_L				0x3//�ֶ���ҺŨ��
#define HAND_FLUID_METHANAL_CONCENTRATION_H		0x4//�ֶ���Һ��ȩŨ��
#define HAND_FLUID_METHANAL_CONCENTRATION_L		0x5//�ֶ���Һ��ȩŨ��
#define HAND_FLUID_RAY_VOLTAGE					0x6//�ֶ���Һӫ���ѹ
#define HAND_FLUID_CALIBRATION					0x7//�ֶ���ҺУ׼

//#define STATUS_PAGE								0x09//״̬ҳ
#define ABSORB_TEMP_STATUS						0x0//�������¶�
#define REACTION_TEMP_STATUS					0x1//��Ӧ���¶�
#define LED_TEMP_STATUS							0x2//LED�����¶�
#define RAY_VOLTAGE_STATUS						0x3//ӫ���ź�ֵ
#define ZERO_VOLTAGE_STATUS						0x4//����ѹֵ
#define PMT_VOLTAGE_STATUS						0x5//PMT��ѹ
#define GAS_FLOW_STATUS							0x6//��������
#define LIQUID_FLOW_STATUS						0x7//Һ������
#define FLUID_ON_OFF_STATUS						0x8//��Һ��
#define GAS_ON_OFF_STATUS						0x9//������
#define ZERO_GAS_ON_OFF_STATUS					0xA//������
#define PUMP_SPEED_STATUS						0xB//�䶯��ת��
#define RATE_STATUS								0xC//������ ����
#define DEV_TEMP_STATUS							0xD//�豸�¶�




#define WARNING_ABSORB_TEMP							1
#define WARNING_REACTION_TEMP						2
#define WARNING_LED_TEMP							3
#define WARNING_RAY_VOLTAGE							4
#define WARNING_ZERO_VOLTAGE						5
#define WARNING_PMT_VOLTAGE							6
#define WARNING_GAS_FLOW							7
#define WARNING_LIQUID_FLOW							8
#define WARNING_AUTO								9
#define WARNING_AUTO_FLUID							10
#define WARNING_AUTO_GAS							11





#define COM_PAGE								0x0D//�˿�ҳ
#define COMID									0x0//�˿ں�
#define COM_BAUD								0x1//������

#define DATA_SAVE_PAGE							0x0F//���ݵ�������ҳ
#define DATA_SAVE_SET							0x0//���ݵ�������
//���ݵ���λ����
#define FLUID_ON_OFF_BIT						0x0001//��Һ��λ
#define GAS_ON_OFF_BIT							0x0002//������λ
#define ZERO_GAS_ON_OFF_BIT						0x0004//������λ
#define RAY_VOLTAGE_BIT							0x0008//ӫ���ź�ֵλ
#define ZERO_VOLTAGE_BIT						0x0010//����ѹֵλ
#define SENSITIVITY_BIT							0x0020//������λ
#define FLUID_FLOW_BIT							0x0040//����Һ����λ
#define PUMP_SPEED_BIT							0x0080//�䶯��ת��λ
#define METHANAL_CONCENTRATION_BIT				0x0100//��ȩŨ��λ
#define ABSORB_TEMP_BIT_						0x0200//�������¶�λ
#define REACTION_TEMP_BIT						0x0400//��Ӧ���¶�λ
#define GAS_FLOW_BIT							0x0800//��������λ
#define TIME_MIN_BIT							0x1000//ʱ���λ
#define TIME_TENMIN_BIT							0x2000//ʱ��10��λ
#define TIME_HOUR_BIT							0x4000//ʱ��Сʱλ



#define VERSIONS_PAGE							0x11//�汾
#define VERSIONS								0x0//�汾

#define PASSWORD_PAGE							0x12//����
#define OLD_PASSWORD							0x0//������
#define NEW_PASSWORD1							0x1//������
#define NEW_PASSWORD2							0x2//ȷ������
#define PASSWORD_VERIFY							0x3//ȷ����֤
#define PASSWORD_SET							0x4//ȷ���޸�
#define PASSWORD								0x5//��������
#define PASSWORD_SAVE							0x6//��������

#define TIME_YEAR           	   					0
#define TIME_MONTH									1
#define TIME_DAY									2
#define TIME_WEAK									3
#define TIME_HOUR									4
#define TIME_MIN									5
#define TIME_SEC									6


#define FLOW_CALIBRATION					0x26
#define FLUID_FLOW_RATE						0x0
#define GAS_FLOW_RATE							0x1

#define LOG0											0x30
#define LOG1											0x32
#define LOG2											0x34
#define LOG3											0x36
#define LOG4											0x38
#define LOG												0x2D
#define LOG_NEW										0
#define LOG_NEXT									1
#define LOG_LAST									2
#define LOG_FPTR_START								0//��־��ʾ�ļ�ָ�뿪ʼλ��
#define LOG_FPTR_END								1//��־��ʾ�ļ�ָ�����λ��
#define LOG_ABSORB_TEMP_UP							(1<<0)//�������¶�
#define LOG_ABSORB_TEMP_DOWN						(1<<1)//�������¶�
#define LOG_ABSORB_TEMP_ERROR						(1<<2)//�������¶�
#define LOG_REACTION_TEMP_UP						(1<<3)//��Ӧ���¶�
#define LOG_REACTION_TEMP_DOWN						(1<<4)//��Ӧ���¶�
#define LOG_REACTION_TEMP_ERROR						(1<<5)//��Ӧ���¶�
#define LOG_LED_TEMP_UP								(1<<6)//LED�����¶�
#define LOG_LED_TEMP_DOWN							(1<<7)//LED�����¶�
#define LOG_LED_TEMP_ERROR							(1<<8)//LED�����¶�
#define LOG_RAY_VOLTAGE_UP							(1<<9)//ӫ���ź�ֵ
#define LOG_RAY_VOLTAGE_DOWN						(1<<10)//ӫ���ź�ֵ
#define LOG_ZERO_VOLTAGE_UP							(1<<11)//����ѹֵ
#define LOG_ZERO_VOLTAGE_DOWN						(1<<12)//����ѹֵ
#define LOG_PMT_VOLTAGE_UP							(1<<13)//PMT��ѹ
#define LOG_PMT_VOLTAGE_DOWN						(1<<14)//PMT��ѹ
#define LOG_GAS_FLOW_UP								(1<<15)//��������
#define LOG_GAS_FLOW_DOWN							(1<<16)//��������
#define LOG_LIQUID_FLOW_UP							(1<<17)//Һ������
#define LOG_LIQUID_FLOW_DOWN						(1<<18)//Һ������
#define LOG_AUTO_ZERO								(1<<19)//�Զ������־
#define LOG_AUTO_FLUID								(1<<20)//�Զ���Һ��־
#define LOG_AUTO_GAS								(1<<21)//�Զ�������־
#define LOG_HAND_FLUID								(1<<22)//�ֶ���Һ��־
#define LOG_HAND_GAS								(1<<23)//�ֶ�������־
#define LOG_FLUID_FLOW_RATE							(1<<24)//��Һ��������ϵ��
#define LOG_GAS_FLOW_RATE							(1<<25)//������������ϵ��
#define LOG_PUMP_SPEED								(1<<26)//�䶯��ת����־
#define LOG_COM_BAUD								(1<<27)//���ڲ�������־
#define LOG_AUTO_FLUID_ERROR						(1<<28)//�Զ���Һ������־
#define LOG_AUTO_GAS_ERROR							(1<<29)//�Զ�����������־

typedef enum {
	NORMEL_VALUE=0,
	AUTO_VALUE,
	HAND_VALUE,
	AUTO_ZERO_VALUE,
	AUTO_ERROR,
	AUTO_ZERO_ERROR
}WORK_STATUS;

typedef enum {
	AUTO_WAIT_CALIBRATION=0,
	AUTO_CALIBRATION=1,
	AUTO_CALIBRATION_ERROR=2,
}AUTO_CALIBRATION_STATUS;

typedef struct
{
	int fluid_flow_status;
	int fluid_concentration;
	int sc_air_flow_rate;
	int wc_air_flow_rate;
	int sc_start_concentration;
	int wc_start_concentration;
	int air_temp;
	int air_rh;
	int air_pressure;
	int slope_coefficients;
	char radiotube_status;
	char atomizer_switch;	
}FileSive;

typedef struct
{
	 short home_page[4];
	 short auto_zero[6];
	 short auto_fluid[8];
	 short auto_gas[8];
	 unsigned short hand_gas[9];
	 short hand_fluid[8];
	 short status[14];
	 short set[8];
	 short com[2];
	 short data_save_set;
	 short data_demand[13];
	 char versions[10];
	 short password[7];
	unsigned char time[7];
	short flow_calib[2];
	char log0[30];
	char log1[30];
	char log2[30];
	char log3[30];
	char log4[30];
	char log[3];
	short warning[12];
	unsigned char new_data;
	volatile unsigned int new_log;
	int log_pftr[2];
	FileSive file;
}MODEL_4001;



#define MODEL_1006_STATUS					0x06//״̬��ʶ


#define HOME_PAGE                 				1//��ҳ
#define HOME_WORK_STATUS						0x0

#define Actual_operating_concentration			0x08

#define START_PRODUCT_PAGE					0x03
#define PUMP_SPEED_SET						0x00 //�䶯��ת�� 
#define HAND_CLEAN_ML					0x01 //�䶯��ת�� 
#define SC_CONCENTRATION					0x03 //���Ũ��
#define AIR_FLOW_RATE_SET					0x04 //��������
#define FLUID_CONCENTRATION					0x05 //Һ��Ũ��
#define FLUID_FLOW							0x06 //Һ������
#define WC_CONCENTRATION					0x07 //����Ũ��
#define START_PRODUCT_COMMEND				0x08 //����ָ����
#define INJECTION_PUMP_STATUS				0x09 //�Զ���ϴ״̬
#define START_INJECTION_COMMEND				0x0A //�Զ���ϴָ����
#define HAND_CLEAN_STATUS             0x0B //�ֶ���ϴ״̬
#define AUTO_CLEAN_STATUS				0x0C //�Զ���ϴ״̬

#define WARNING								0x07//�����ʶ
#define STATUS_PAGE							0x06
#define FLUID_FLOW_STATUS					0x00//Һ������
#define SC_AIR_FLOW_RATE					0x01//�����������
#define WC_AIR_FLOW_RATE					0x02//������������
#define SC_START_CONCENTRATION				0x03//�������Ũ��
#define WC_START_CONCENTRATION				0x04//��������Ũ��
#define RADIOTUBE_STATUS					0x05//��ŷ�״̬
#define ATOMIZER_SWITCH						0x06//����״̬
#define MODEL_1006_TEMP 					0x07//�¶�
#define MODEL_1006_RH 						0x08//ʪ��
#define AIR_PRESSURE		 				0x09//ѹ��
#define WARNING_ALL							0x0A//���еľ�����ʾ

#define SET_PAGE							0x09//Ũ��У׼ҳ��
//#define SLOPE_COEFFICIENTS					0x00//б��ϵ��
//#define SC_TEMP_SET							0x1 //����¶�
#define ACTUAL_CON1						0x00
#define ACTUAL_CON2					  0x01
#define ACTUAL_CON3						0x02
#define ACTUAL_CON4						0x03
#define ACTUAL_CON5						0x04
#define SET_CON1						  0x05
#define SET_CON2					  	0x06
#define SET_CON3						  0x07
#define SET_CON4						  0x08
#define SET_CON5						  0x09
#define CALI_CON_FLUID        0x0A
#define CAIL_CON_SETFLOW			0x0B
//#define CAIL_CON_FLUIDFLOW		0x0C //Һ�����������ӷ���ҳ��ͬ��
#define CAIL_SC_CONCENTRATION 0x0D
#define CALI_CON_COMMEND   		0x0E
#define CALI_CON_COMMEND1  		0x0F

#define CALIBRATION_CON_a    0x00
#define CALIBRATION_CON_b    0x01
#define CALIBRATION_CON_c    0x02


#define CALIBRATION_PAGE					0x0D
#define PUMP_FLOW							0x00
#define PUMP_BULK_ML						0x01//ע�����
#define FLOW_WEIGHT							0x02
#define PUMP_CALIBRATION_FACTOR				0x03
#define SET_FLOW							0x04
#define ACTUAL_FLOW_RATE					0x05
#define ACTUAL_FLOW							0x06
#define TEMP_K								0x07
#define TEMP_B								0x08
#define HUMIDITY_K							0x09
#define HUMIDITY_B							0x0A
#define PRESSURE_K							0x0B
#define PRESSURE_B							0x0C
#define CALIBRATION_COMMEND					0x0D
#define CALIBRATION_PASSWORD				0x0E

//#define TIME_PAGE							0x03

#define DEMAND_PAGE								0x0B//���ݲ�ѯҳ
#define STAR_YEAR								0x0//���ݲ�ѯ��ʼ��
#define STAR_MONTH								0x1//���ݲ�ѯ��ʼ��
#define STAR_DAY								0x2//���ݲ�ѯ��ʼ��
#define DATA_EXPORT								0x3//���ݵ���
#define DATA_EXPORT_EN							0x4//����ʹ��
#define DATA_EXPORT_PERCENRT					0x5//�����ٷֱ�
typedef struct
{
	short home_page[9];
	short start_product[13];
	short status[10];
	short set[20];  //���Ӷ��У׼
	short calibration[16];
	float fluidflow_tem[5];//����Ũ�ȶ��궨����
	float ratio_tem[5];//
	char calibration_num;//
	float CONCALI_K[5];//
	float CONCALI_B[5];//
	unsigned char time[7];
	short data_demand[14];
	unsigned char new_data;
	//unsigned short motor_data;
	short warning[11];
	FileSive file;
}MODEL_1006;


extern MODEL_1006 model_1006;



extern MODEL_4001 model4001;
extern short home_page_time[6];
extern short home_page_lin_length;
void DugsCommandSend(unsigned short addr,unsigned short command_data);
void Dugs(void *p_arg);
void auto_hand_action(void *p_arg);
void Dugs_send_config(void);
void DugsText_Display(unsigned short addr,char text[],unsigned short length);
#endif
