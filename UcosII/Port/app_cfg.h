#ifndef __APP_CFG_H__
#define __APP_CFG_H__

/*******************设置任务优先级*******************/
//#define GET_ANGLEANDGYRO_TASK_PRIO                 3
//#define TASK_TCP_CLIENT_TASK_PRIO                  4
//#define TASK_UDP_SERVER_TASK_PRIO                  5
#define DEALSERIAL_COMMAND_TASK_PRIO               6
#define GETSERIAL_COMMAND_TASK_PRIO                7
//#define SWING_TASK_PRIO                            8
#define TASK_TEMP_STK_PRIO                         3//9
#define TASK_TEMP_CONTR_STK_PRIO                   4//9
#define AUTO_HAND_TASK_PRIO                         11//8
#define DUGS_TASK_PRIO                              9
#define AD_TASK_PRIO                              	10
#define FATFS_TEST_TASK_PRIO                       5
#define TEST_TASK_PRIO             			       13
//#define LED_OFF_TASK_PRIO                          10
#define STARTUP_TASK_PRIO                          20

/************设置栈大小（单位为 OS_STK ）************/
#define STARTUP_TASK_STK_SIZE                      400
#define DEALSERIAL_COMMAND_TASK_STK_SIZE           320  //sum 480*4  free 1228 use 692
#define GETSERIAL_COMMAND_TASK_STK_SIZE            120	//sum 320*4  free 1032 use 248
#define GET_ANGLEANDGYRO_TASK_STK_SIZE             120	//sum 
#define SWING_TASK_STK_SIZE                        120
#define TASK_AD_STK_SIZE	    				   160	//sum 320*4 free 976 use 304
#define TASK_TEMP_STK_SIZE                         280	//sum 400*4 free 1332 use 268
#define TASK_TEMP_CONTR_STK_SIZE                   120	//sum 400*4 free 1360 use 240
#define TASK_DUGS_STK_SIZE        			       160	//sum 400*4 free 1296 use 304
#define TASK_ATUO_HAND_STK_SIZE        		       160	//sum 400*4 free 1344 use 256
#define TASK_FATFS_TEST_STK_SIZE                   1200  //sum 2000*4 free 6788 use 1212  
#define TASK_TEST_STK_SIZE                  	  200
#endif
