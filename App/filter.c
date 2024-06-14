#include "filter.h"
#include <stdlib.h>
#include "math.h"


TEMPERATURE_FILTER_t Temperature_filter1,Temperature_filter2,Temperature_filter3,Temperature_filter4;

float Temperature_filter1_data[TEMPERATURE_FILTER_LENGTH];
float Temperature_filter2_data[TEMPERATURE_FILTER_LENGTH];
float Temperature_filter3_data[TEMPERATURE_FILTER_LENGTH];
float Temperature_filter4_data[TEMPERATURE_FILTER_LENGTH];

ALICAT_FILTER_t Alicat_filter1;
float Alicat_filter1_data[ALICAT_FILTER_LENGTH];

FILTER_FLOAT_t AD_filter6,AD_filter7;
float AD_filter6_data[AD_FILTER_LENGTH];
float AD_filter7_data[AD_FILTER7_LENGTH];
//#define MPU9150_COMPASS_FILTER_LENGTH  	5
//#define MPU9150_ACCL_FILTER_LENGTH  	10
//#define MPU9150_GYRO_FILTER_LENGTH  	10


//float MPU9150_GyroX_filter_data[MPU9150_GYRO_FILTER_LENGTH];
//float MPU9150_GyroY_filter_data[MPU9150_GYRO_FILTER_LENGTH];
//float MPU9150_GyroZ_filter_data[MPU9150_GYRO_FILTER_LENGTH];

//float MPU9150_GyroX_filter_data_D2[MPU9150_GYRO_FILTER_LENGTH];
//float MPU9150_GyroY_filter_data_D2[MPU9150_GYRO_FILTER_LENGTH];
//float MPU9150_GyroZ_filter_data_D2[MPU9150_GYRO_FILTER_LENGTH];

//float MPU9150_AcclX_filter_data[MPU9150_ACCL_FILTER_LENGTH];
//float MPU9150_AcclY_filter_data[MPU9150_ACCL_FILTER_LENGTH];
//float MPU9150_AcclZ_filter_data[MPU9150_ACCL_FILTER_LENGTH];

//float MPU9150_AcclX_filter_data_D2[MPU9150_ACCL_FILTER_LENGTH];
//float MPU9150_AcclY_filter_data_D2[MPU9150_ACCL_FILTER_LENGTH];
//float MPU9150_AcclZ_filter_data_D2[MPU9150_ACCL_FILTER_LENGTH];

//MPU9150_SENSOR_FILTER_t 	MPU9150_CmpsX_filter;
//MPU9150_SENSOR_FILTER_t 	MPU9150_CmpsY_filter;
//MPU9150_SENSOR_FILTER_t 	MPU9150_CmpsZ_filter;



//MPU9150_SENSOR_FILTER_t 	MPU9150_GyroX_filter;
//MPU9150_SENSOR_FILTER_t 	MPU9150_GyroY_filter;
//MPU9150_SENSOR_FILTER_t 	MPU9150_GyroZ_filter;

//MPU9150_SENSOR_FILTER_t 	MPU9150_GyroX_filter_D2;
//MPU9150_SENSOR_FILTER_t 	MPU9150_GyroY_filter_D2;
//MPU9150_SENSOR_FILTER_t 	MPU9150_GyroZ_filter_D2;


//MPU9150_SENSOR_FILTER_t 	MPU9150_AcclX_filter;
//MPU9150_SENSOR_FILTER_t 	MPU9150_AcclY_filter;
//MPU9150_SENSOR_FILTER_t 	MPU9150_AcclZ_filter;

//MPU9150_SENSOR_FILTER_t 	MPU9150_AcclX_filter_D2;
//MPU9150_SENSOR_FILTER_t 	MPU9150_AcclY_filter_D2;
//MPU9150_SENSOR_FILTER_t 	MPU9150_AcclZ_filter_D2;


//FILTER_Comple_FLOAT_t MPU9150_Accl_X_filter;
//FILTER_Comple_FLOAT_t MPU9150_Accl_Y_filter;
//FILTER_Comple_FLOAT_t MPU9150_Accl_Z_filter;

//FILTER_Comple_FLOAT_t MPU9150_Accl_X_filter_D2;
//FILTER_Comple_FLOAT_t MPU9150_Accl_Y_filter_D2;
//FILTER_Comple_FLOAT_t MPU9150_Accl_Z_filter_D2;



//FILTER_FLOAT_t Motor_Azimuth_Track_filter;
//FILTER_INT_t Agc_ms_filter;
//FILTER_INT_t Agc_s_filter;



void filter_Comple_Float_Init(FILTER_Comple_FLOAT_t *pfilter)
{
		pfilter->angle = 0.0l;
		pfilter->devation_angle =0.0l;
		pfilter->last_angle = 0.0l;
		pfilter->flag = 1;
}

void filterFloat_Init(FILTER_FLOAT_t *pfilter,float pdata[],int filter_length)
{
		int i;
  	pfilter->pos_pointer = 0;
  	pfilter->init_flag = filter_length;
  	pfilter->max_long = filter_length;
		pfilter->sum = 0.0l;
		pfilter->average = 0.0l;	
  	pfilter->pdata = pdata;		
		for(i=0;i<pfilter->max_long;i++)
		{
			pfilter->pdata[i] = 0.0l;
		}	
}

void filterInt_Init(FILTER_INT_t *pfilter,int pdata[], int filter_length)
{
	int i;
  	pfilter->pos_pointer = 0;
  	pfilter->init_flag = filter_length;
  	pfilter->max_long = filter_length;
	pfilter->sum = 0;
	pfilter->average = 0;	
  	pfilter->pdata = pdata;			
	for(i=0;i<pfilter->max_long;i++)
	{
		pfilter->pdata[i] = 0;
	}		
		
}


//互补滤波
float comple_filter_float(FILTER_Comple_FLOAT_t *pfilter, float newAngle, float newRate, float dt)
{
	if(pfilter->flag > 0)
	{
		pfilter->flag--;
		pfilter->angle = newAngle;
	}
	else
	{
		pfilter->last_angle = pfilter->angle;
	  pfilter->devation_angle = (newAngle - pfilter->last_angle)/3;
		pfilter->angle += (pfilter->devation_angle + newRate)*dt;
	}	
	return pfilter->angle;
}



////方位互补滤波
//float comple_filter_float_Azimuth(FILTER_Comple_FLOAT_t *pfilter, float newAngle, float newRate, float dt)
//{
//	if(pfilter->flag > 0)
//	{
//		pfilter->flag--;
//		pfilter->angle = newAngle;
//	}
//	else
//	{
//		pfilter->last_angle = pfilter->angle;
//		newAngle = fmod(newAngle+360,360.0);
//	  pfilter->devation_angle = newAngle - pfilter->last_angle;
//		//0点附近处理
//		if(pfilter->devation_angle>180)
//			pfilter->devation_angle -= 360;
//		else if(pfilter->devation_angle<-180)
//			pfilter->devation_angle += 360;
//		
//		pfilter->devation_angle /= 2;
//		pfilter->angle += (pfilter->devation_angle + newRate)*dt;
//		//对360取正余
//		pfilter->angle = fmod(pfilter->angle+360,360.0);
//	}	
//	return pfilter->angle;
//}

////平均值滤波法
int even_filter_int(FILTER_INT_t *pfilter,int new_data)
{
//		int i;
		
		if(pfilter->init_flag > 0)
		{
			pfilter->init_flag--;
			{
				pfilter->pdata[pfilter->pos_pointer] = new_data;
				pfilter->sum += new_data;
				pfilter->pos_pointer++;
			}
			pfilter->average = (pfilter->sum + pfilter->pos_pointer/2)/pfilter->pos_pointer;
			
		}
		else
		{
			pfilter->pos_pointer++;
			if(pfilter->pos_pointer >= pfilter->max_long) pfilter->pos_pointer = 0;
			pfilter->sum = pfilter->sum + new_data - pfilter->pdata[pfilter->pos_pointer];
			
			pfilter->average = (pfilter->sum + pfilter->max_long/2)/pfilter->max_long;
			
			//pfilter->pdata[pfilter->pos_pointer] = pfilter->average;
			
			pfilter->pdata[pfilter->pos_pointer] = new_data;
						
		}
		return pfilter->average;
}

float even_filter_float(FILTER_FLOAT_t *pfilter,float new_data)
{
//	int i;	
	if(pfilter->init_flag > 0)
	{
		pfilter->init_flag--;
		{			
			{
				pfilter->pdata[pfilter->pos_pointer] = new_data;
				pfilter->sum += new_data;
				pfilter->pos_pointer++;
			}
			pfilter->rms = 0;
			//pfilter->average = (pfilter->sum)/pfilter->max_long;
			pfilter->average = (pfilter->sum)/pfilter->pos_pointer;
		}
		
	}
	else
	{
		pfilter->pos_pointer++;
		if(pfilter->pos_pointer >= pfilter->max_long) pfilter->pos_pointer = 0;
		pfilter->sum = pfilter->sum + new_data - pfilter->pdata[pfilter->pos_pointer];		
		
		pfilter->average = (pfilter->sum)/pfilter->max_long;
		
		pfilter->pdata[pfilter->pos_pointer] = new_data;
		
		
		//pfilter->pdata[pfilter->pos_pointer] = pfilter->average;
		
		//pfilter->sum = pfilter->sum - new_data + pfilter->average;	
		
	} 
	return pfilter->average;
}

//float Motor_track_filter(FILTER_FLOAT_t* p_filter,float filter_data)
//{
//	return(even_filter_float(p_filter, filter_data));
//}

//float MPU9150_sensor_filter(MPU9150_SENSOR_FILTER_t* p_filter, float filter_data)
//{
//	return(even_filter_float(p_filter, filter_data));
//}


//int AGC_filter(AGC_FILTER_t *pFilter,int filter_data)
//{
//		return(even_filter_int(pFilter,filter_data));
//}


float Temperature_Filter(FILTER_FLOAT_t*pFilter, float filter_data)
{
		return(even_filter_float(pFilter,filter_data));
}
void filter_init()
{
//    filter_Comple_Float_Init(&MPU9150_Accl_X_filter);
//    filter_Comple_Float_Init(&MPU9150_Accl_Y_filter);
//    filter_Comple_Float_Init(&MPU9150_Accl_Z_filter);

//    filter_Comple_Float_Init(&MPU9150_Accl_X_filter_D2);
//    filter_Comple_Float_Init(&MPU9150_Accl_Y_filter_D2);
//    filter_Comple_Float_Init(&MPU9150_Accl_Z_filter_D2);
      filterFloat_Init(&Temperature_filter1,Temperature_filter1_data,TEMPERATURE_FILTER_LENGTH);
		filterFloat_Init(&Temperature_filter2,Temperature_filter2_data,TEMPERATURE_FILTER_LENGTH);
		filterFloat_Init(&Temperature_filter3,Temperature_filter3_data,TEMPERATURE_FILTER_LENGTH);
		filterFloat_Init(&Temperature_filter4,Temperature_filter4_data,TEMPERATURE_FILTER_LENGTH);
			filterFloat_Init(&Alicat_filter1,Alicat_filter1_data,ALICAT_FILTER_LENGTH);
			filterFloat_Init(&AD_filter6,AD_filter6_data,AD_FILTER_LENGTH);
			filterFloat_Init(&AD_filter7,AD_filter7_data,AD_FILTER7_LENGTH);
	
//		filterFloat_Init(&MPU9150_GyroX_filter, MPU9150_GyroX_filter_data,MPU9150_GYRO_FILTER_LENGTH);
//		filterFloat_Init(&MPU9150_GyroY_filter, MPU9150_GyroY_filter_data,MPU9150_GYRO_FILTER_LENGTH);
//		filterFloat_Init(&MPU9150_GyroZ_filter, MPU9150_GyroZ_filter_data,MPU9150_GYRO_FILTER_LENGTH);
//	
//		filterFloat_Init(&MPU9150_GyroX_filter_D2, MPU9150_GyroX_filter_data_D2,MPU9150_GYRO_FILTER_LENGTH);
//		filterFloat_Init(&MPU9150_GyroY_filter_D2, MPU9150_GyroY_filter_data_D2,MPU9150_GYRO_FILTER_LENGTH);
//		filterFloat_Init(&MPU9150_GyroZ_filter_D2, MPU9150_GyroZ_filter_data_D2,MPU9150_GYRO_FILTER_LENGTH);
//	
//		
//		filterFloat_Init(&MPU9150_AcclX_filter, MPU9150_AcclX_filter_data,MPU9150_ACCL_FILTER_LENGTH);
//		filterFloat_Init(&MPU9150_AcclY_filter, MPU9150_AcclY_filter_data,MPU9150_ACCL_FILTER_LENGTH);
//		filterFloat_Init(&MPU9150_AcclZ_filter, MPU9150_AcclZ_filter_data,MPU9150_ACCL_FILTER_LENGTH);
//		
//		filterFloat_Init(&MPU9150_AcclX_filter_D2, MPU9150_AcclX_filter_data_D2,MPU9150_ACCL_FILTER_LENGTH);
//		filterFloat_Init(&MPU9150_AcclY_filter_D2, MPU9150_AcclY_filter_data_D2,MPU9150_ACCL_FILTER_LENGTH);
//		filterFloat_Init(&MPU9150_AcclZ_filter_D2, MPU9150_AcclZ_filter_data_D2,MPU9150_ACCL_FILTER_LENGTH);


}








