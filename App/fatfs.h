#ifndef _FATFS_H_
#define _FATFS_H_


#define TEST_FILE_LEN			(2*1024*1024)	/* 用于测试的文件长度 */
#define BUF_SIZE				(4*1024)		/* 每次读写SD卡的最大数据长度 */



void fatfs_test(void *p_arg);
void Data_display(short *time);
int referfile_time(unsigned int start_fptr,unsigned int end_fptr,short time[]);
void log_display(void);
#endif
