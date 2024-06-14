#ifndef _FILTER__H_
#define _FILTER__H_

#define TEMPERATURE_FILTER_LENGTH 5
#define ALICAT_FILTER_LENGTH      100
#define AD_FILTER_LENGTH          1200
#define AD_FILTER7_LENGTH         100

typedef struct 
{
	int pos_pointer;
	int max_long;
	int init_flag;
	float sum;
	float average;
	float rms;
	//int data_type;
	float new_data;
	float *pdata;	
}FILTER_FLOAT_t;

typedef struct 
{
	int pos_pointer;
	int max_long;
	int init_flag;
	int sum;
	int average;
	//int data_type;
	int new_data;
	int *pdata;	
}FILTER_INT_t;

typedef struct 
{
	float angle;
	float last_angle;
	float devation_angle;
	int flag;
}FILTER_Comple_FLOAT_t;

#define TEMPERATURE_FILTER_t  FILTER_FLOAT_t
#define ALICAT_FILTER_t       FILTER_FLOAT_t

extern TEMPERATURE_FILTER_t Temperature_filter1,Temperature_filter2,Temperature_filter3,Temperature_filter4;

extern float Temperature_filter1_data[TEMPERATURE_FILTER_LENGTH];
extern float Temperature_filter2_data[TEMPERATURE_FILTER_LENGTH];

extern ALICAT_FILTER_t Alicat_filter1;
extern float Alicat_filter1_data[ALICAT_FILTER_LENGTH];

extern FILTER_FLOAT_t AD_filter6 ,AD_filter7;
extern float AD_filter6_data[AD_FILTER_LENGTH];
extern float AD_filter7_data[AD_FILTER7_LENGTH];
//#define ADXRS450_GYRO_FILTER_t  FILTER_FLOAT_t

//#define MPU9150_SENSOR_FILTER_t FILTER_FLOAT_t

////#define GYRO_FILTER_t FILTER_FLOAT_t
//#define AGC_FILTER_t   FILTER_INT_t


//float comple_filter_float(FILTER_Comple_FLOAT_t *pfilter, float newAngle, float newRate, float dt);
//float comple_filter_float_Azimuth(FILTER_Comple_FLOAT_t *pfilter, float newAngle, float newRate, float dt);
//float  MPU9150_sensor_filter(MPU9150_SENSOR_FILTER_t*p_filter, float filter_data);

void filter_init(void);
//int AGC_filter(AGC_FILTER_t *pFilter,int filter_data);
//float MPU9150_CompassFilter(FILTER_FLOAT_t*pFilter, float filter_data);
float Temperature_Filter(FILTER_FLOAT_t* p_filter,float filter_data);
float even_filter_float(FILTER_FLOAT_t *pfilter,float new_data);
#endif

