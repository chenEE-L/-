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


#define AUTO_STATUS								0x001B//自动状态
#define METHANAL_U								0x001C//甲醛单位
#define METHANAL_CONCENTRATION					0x001D//甲醛浓度H
#define METHANAL_CONCENTRATION_L				0x001E//甲醛浓度L

#define AUTO_ZERO								0x03//自动零点
#define AUTO_ZERO_ON_OFF						0x0//自动零点开关
#define AUTO_ZERO_STATUS						0x1//自动零点状态
#define AUTO_ZERO_TIME_LENGTH					0x2//自动零点时长
#define AUTO_ZERO_TIME_HOUR						0x3//自动零点时
#define AUTO_ZERO_TIME_MIN						0x4//自动零点分
#define AUTO_ZERO_TIME_INTERVAL					0x5//自动零点时间间隔

#define AUTO_FLUID								0x04//自动标液
#define AUTO_FLUID_ON_OFF						0x0//自动标液开关
#define AUTO_FLUID_STATUS						0x1//自动标液状态
#define AUTO_FLUID_CONCENTRATION				0x2//自动标液浓度
#define AUTO_FLUID_ZERO_TIME_LENGTH				0x3//自动标液时长
#define AUTO_FLUID_TIME_LENGTH					0x4//自动标液时长
#define AUTO_FLUID_TIME_HOUR					0x5//自动标液时
#define AUTO_FLUID_TIME_MIN						0x6//自动标液分
#define AUTO_FLUID_TIME_INTERVAL				0x7//自动标液时间间隔

#define AUTO_GAS								0x05//自动标气
#define AUTO_GAS_ON_OFF							0x0//自动标气开关
#define AUTO_GAS_STATUS							0x1//自动标气状态
#define AUTO_GAS_CONCENTRATION					0x2//自动标气浓度
#define AUTO_GAS_ZERO_TIME_LENGTH				0x3//自动标气时长
#define AUTO_GAS_TIME_LENGTH					0x4//自动标气时长
#define AUTO_GAS_TIME_HOUR						0x5//自动标气时
#define AUTO_GAS_TIME_MIN						0x6//自动标气分
#define AUTO_GAS_TIME_INTERVAL					0x7//自动标气时间间隔

#define HAND_GAS								0x07//手动标气开关
#define HAND_GAS_ON_OFF							0x0//手动标气开关
#define HAND_GAS_ZERO_TIME_LENGTH				0x1//手动标气时长
#define HAND_GAS_CONCENTRATION_H				0x2//手动标气浓度
#define HAND_GAS_CONCENTRATION_L				0x3//手动标气浓度
#define HAND_GAS_METHANAL_CONCENTRATION_H		0x4//手动标气甲醛浓度
#define HAND_GAS_METHANAL_CONCENTRATION_L		0x5//手动标气甲醛浓度
#define HAND_GAS_RAY_VOLTAGE					0x6//手动标气荧光电压
#define HAND_GAS_CALIBRATION					0x7//手动标液校准
#define HAND_GAS_ABSORPTION_EFFI			0x8//手动标气吸收效率校准

#define HAND_FLUID								0x08//手动标液开关
#define HAND_FLUID_ON_OFF						0x0//手动标液开关
#define HAND_FLUID_ZERO_TIME_LENGTH				0x1//手动标液时长
#define HAND_FLUID_CONCENTRATION_H				0x2//手动标液浓度
#define HAND_FLUID_CONCENTRATION_L				0x3//手动标液浓度
#define HAND_FLUID_METHANAL_CONCENTRATION_H		0x4//手动标液甲醛浓度
#define HAND_FLUID_METHANAL_CONCENTRATION_L		0x5//手动标液甲醛浓度
#define HAND_FLUID_RAY_VOLTAGE					0x6//手动标液荧光电压
#define HAND_FLUID_CALIBRATION					0x7//手动标液校准

//#define STATUS_PAGE								0x09//状态页
#define ABSORB_TEMP_STATUS						0x0//吸收室温度
#define REACTION_TEMP_STATUS					0x1//反应室温度
#define LED_TEMP_STATUS							0x2//LED底座温度
#define RAY_VOLTAGE_STATUS						0x3//荧光信号值
#define ZERO_VOLTAGE_STATUS						0x4//零点电压值
#define PMT_VOLTAGE_STATUS						0x5//PMT高压
#define GAS_FLOW_STATUS							0x6//气体流量
#define LIQUID_FLOW_STATUS						0x7//液体流量
#define FLUID_ON_OFF_STATUS						0x8//标液阀
#define GAS_ON_OFF_STATUS						0x9//标气阀
#define ZERO_GAS_ON_OFF_STATUS					0xA//零气阀
#define PUMP_SPEED_STATUS						0xB//蠕动泵转速
#define RATE_STATUS								0xC//灵敏度 比例
#define DEV_TEMP_STATUS							0xD//设备温度




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





#define COM_PAGE								0x0D//端口页
#define COMID									0x0//端口号
#define COM_BAUD								0x1//波特率

#define DATA_SAVE_PAGE							0x0F//数据导出配置页
#define DATA_SAVE_SET							0x0//数据导出设置
//数据导出位设置
#define FLUID_ON_OFF_BIT						0x0001//标液阀位
#define GAS_ON_OFF_BIT							0x0002//标气阀位
#define ZERO_GAS_ON_OFF_BIT						0x0004//零气阀位
#define RAY_VOLTAGE_BIT							0x0008//荧光信号值位
#define ZERO_VOLTAGE_BIT						0x0010//零点电压值位
#define SENSITIVITY_BIT							0x0020//灵敏度位
#define FLUID_FLOW_BIT							0x0040//吸收液流量位
#define PUMP_SPEED_BIT							0x0080//蠕动泵转速位
#define METHANAL_CONCENTRATION_BIT				0x0100//甲醛浓度位
#define ABSORB_TEMP_BIT_						0x0200//吸收室温度位
#define REACTION_TEMP_BIT						0x0400//反应室温度位
#define GAS_FLOW_BIT							0x0800//气体流量位
#define TIME_MIN_BIT							0x1000//时间分位
#define TIME_TENMIN_BIT							0x2000//时间10分位
#define TIME_HOUR_BIT							0x4000//时间小时位



#define VERSIONS_PAGE							0x11//版本
#define VERSIONS								0x0//版本

#define PASSWORD_PAGE							0x12//密码
#define OLD_PASSWORD							0x0//旧密码
#define NEW_PASSWORD1							0x1//新密码
#define NEW_PASSWORD2							0x2//确认密码
#define PASSWORD_VERIFY							0x3//确认验证
#define PASSWORD_SET							0x4//确认修改
#define PASSWORD								0x5//密码输入
#define PASSWORD_SAVE							0x6//密码输入

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
#define LOG_FPTR_START								0//日志显示文件指针开始位置
#define LOG_FPTR_END								1//日志显示文件指针结束位置
#define LOG_ABSORB_TEMP_UP							(1<<0)//吸收室温度
#define LOG_ABSORB_TEMP_DOWN						(1<<1)//吸收室温度
#define LOG_ABSORB_TEMP_ERROR						(1<<2)//吸收室温度
#define LOG_REACTION_TEMP_UP						(1<<3)//反应室温度
#define LOG_REACTION_TEMP_DOWN						(1<<4)//反应室温度
#define LOG_REACTION_TEMP_ERROR						(1<<5)//反应室温度
#define LOG_LED_TEMP_UP								(1<<6)//LED底座温度
#define LOG_LED_TEMP_DOWN							(1<<7)//LED底座温度
#define LOG_LED_TEMP_ERROR							(1<<8)//LED底座温度
#define LOG_RAY_VOLTAGE_UP							(1<<9)//荧光信号值
#define LOG_RAY_VOLTAGE_DOWN						(1<<10)//荧光信号值
#define LOG_ZERO_VOLTAGE_UP							(1<<11)//零点电压值
#define LOG_ZERO_VOLTAGE_DOWN						(1<<12)//零点电压值
#define LOG_PMT_VOLTAGE_UP							(1<<13)//PMT高压
#define LOG_PMT_VOLTAGE_DOWN						(1<<14)//PMT高压
#define LOG_GAS_FLOW_UP								(1<<15)//气体流量
#define LOG_GAS_FLOW_DOWN							(1<<16)//气体流量
#define LOG_LIQUID_FLOW_UP							(1<<17)//液体流量
#define LOG_LIQUID_FLOW_DOWN						(1<<18)//液体流量
#define LOG_AUTO_ZERO								(1<<19)//自动零点日志
#define LOG_AUTO_FLUID								(1<<20)//自动标液日志
#define LOG_AUTO_GAS								(1<<21)//自动标气日志
#define LOG_HAND_FLUID								(1<<22)//手动标液日志
#define LOG_HAND_GAS								(1<<23)//手动标气日志
#define LOG_FLUID_FLOW_RATE							(1<<24)//标液流量比例系数
#define LOG_GAS_FLOW_RATE							(1<<25)//标气流量比例系数
#define LOG_PUMP_SPEED								(1<<26)//蠕动泵转速日志
#define LOG_COM_BAUD								(1<<27)//串口波特率日志
#define LOG_AUTO_FLUID_ERROR						(1<<28)//自动标液错误日志
#define LOG_AUTO_GAS_ERROR							(1<<29)//自动标气错误日志

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



#define MODEL_1006_STATUS					0x06//状态标识


#define HOME_PAGE                 				1//首页
#define HOME_WORK_STATUS						0x0

#define Actual_operating_concentration			0x08

#define START_PRODUCT_PAGE					0x03
#define PUMP_SPEED_SET						0x00 //蠕动泵转速 
#define HAND_CLEAN_ML					0x01 //蠕动泵转速 
#define SC_CONCENTRATION					0x03 //标况浓度
#define AIR_FLOW_RATE_SET					0x04 //气体流量
#define FLUID_CONCENTRATION					0x05 //液体浓度
#define FLUID_FLOW							0x06 //液体流量
#define WC_CONCENTRATION					0x07 //工况浓度
#define START_PRODUCT_COMMEND				0x08 //发生指令码
#define INJECTION_PUMP_STATUS				0x09 //自动清洗状态
#define START_INJECTION_COMMEND				0x0A //自动清洗指令码
#define HAND_CLEAN_STATUS             0x0B //手动清洗状态
#define AUTO_CLEAN_STATUS				0x0C //自动清洗状态

#define WARNING								0x07//警告标识
#define STATUS_PAGE							0x06
#define FLUID_FLOW_STATUS					0x00//液体流量
#define SC_AIR_FLOW_RATE					0x01//标况气体流量
#define WC_AIR_FLOW_RATE					0x02//工况气体流量
#define SC_START_CONCENTRATION				0x03//标况发生浓度
#define WC_START_CONCENTRATION				0x04//工况发生浓度
#define RADIOTUBE_STATUS					0x05//电磁阀状态
#define ATOMIZER_SWITCH						0x06//雾化器状态
#define MODEL_1006_TEMP 					0x07//温度
#define MODEL_1006_RH 						0x08//湿度
#define AIR_PRESSURE		 				0x09//压力
#define WARNING_ALL							0x0A//所有的警告提示

#define SET_PAGE							0x09//浓度校准页面
//#define SLOPE_COEFFICIENTS					0x00//斜率系数
//#define SC_TEMP_SET							0x1 //标况温度
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
//#define CAIL_CON_FLUIDFLOW		0x0C //液体流量和粒子发生页面同步
#define CAIL_SC_CONCENTRATION 0x0D
#define CALI_CON_COMMEND   		0x0E
#define CALI_CON_COMMEND1  		0x0F

#define CALIBRATION_CON_a    0x00
#define CALIBRATION_CON_b    0x01
#define CALIBRATION_CON_c    0x02


#define CALIBRATION_PAGE					0x0D
#define PUMP_FLOW							0x00
#define PUMP_BULK_ML						0x01//注射体积
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

#define DEMAND_PAGE								0x0B//数据查询页
#define STAR_YEAR								0x0//数据查询起始年
#define STAR_MONTH								0x1//数据查询开始月
#define STAR_DAY								0x2//数据查询开始日
#define DATA_EXPORT								0x3//数据导出
#define DATA_EXPORT_EN							0x4//导出使能
#define DATA_EXPORT_PERCENRT					0x5//导出百分比
typedef struct
{
	short home_page[9];
	short start_product[13];
	short status[10];
	short set[20];  //增加多点校准
	short calibration[16];
	float fluidflow_tem[5];//增加浓度多点标定线性
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
