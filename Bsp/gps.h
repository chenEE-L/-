#ifndef  __GPS_H__
#define  __GPS_H__
#include "stdint.h"

#define PAI                        3.14159

#define  MAGNETIC_DEVIATION_EAST  1
#define  MAGNETIC_DEVIATION_WEST  -1

#define  MAGNETIC_VARIATION_EAST  1
#define  MAGNETIC_VARIATION_WEST  -1

#define  GPS_LONGITUDE_EAST   1
#define  GPS_LONGITUDE_WEST 	-1


#define  GPS_LATITUDE_SOUTH   -1
#define  GPS_LATITUDE_NORTH    1

typedef struct 
{
	uint16_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}GPS_time;



typedef struct
{
/*
	uint16_t lat_d; ////纬度 度数
	char  lat_m[10]; ////纬度 分
	
	uint8_t  lat_dir; /////纬度方向
	
	uint16_t lon_d; ////经度 度数
	char  lon_m[10]; ////经度 分
	
	uint8_t  lon_dir; /////经度方向
	
	char alt[10]; ///高度 m
	char alt_uint[10];
*/
	int   compass_available;
	float magnetic_heading;
	float true_heading;
	float speed;
	float gps_heading;
	float gps_heading_TTL;//**GPS航行角有效生存时间（周期）
	float gps_latitude;
	float gps_latitude_dir;
	float gps_longitude;
	float gps_longitude_dir;
	float gps_latitude_old;
	float gps_longitude_old;
	
}GPS_GEO_POS;


//#define 	GPS_ANTENNA_POWER_ON_PIN	 PIN0_12    ////GPS天线电源供电控制引脚
#define		GPS_ANTENNA_POWER_ON	8				// BIT 3
#define 	GPS_ANTENNA_SHORT_CIRCUIT_PIN  PIN0_13

#define 	SWITCH_TO_GPS	4			//BIT 2
#define 	MCU_CTRL_CPLD_ADDRESS		2



#define 	GPS_UNLOCK  	0   ////GPS 信号未锁定
#define 	GPS_LOCKED  	1   /////GPS 信号锁定  
#define 	GPS_UNSETTLE 	2   //////GPS信号锁定后失锁，但是时间未超过5分钟，或GPS锁定稳定时间不到1分钟	

#define 	CLOCK_LOCKED 	2

#define MAX_GPS_FIELD_NUMBER_ONE_COMMAND 30
#define GPS_COMMAND_MAX_LENGTH       100

#define GPS_CHANNEL_NUMBER_MAX 12


extern GPS_GEO_POS gps_position;

int gps_1pps_locked(void);

void process_nmea(char gps_command[]);
int gps_getOneCommandString(int uart_no,char *cmd_line, unsigned char n);
int gps_extract_field(char *buff,char *field[],char delimiter,short field_no_max);
void gps_init(void);
void process_Gps_command(void);

void uart0_switch_to_gps(void);
void uart0_switch_to_backpanel(void);

extern int azimuth(int lg_, int lt_, int loc);		//方位角计算(azimuth*10)
int polarization(int lg_, int lt_, int loc);		/*极化角计算(*10)*/
unsigned int elevation(int lg_, int lt_, int loc);		/*仰角计算(elevation*10)*/
extern uint8_t  utc_valid;
#endif
