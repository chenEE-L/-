#ifndef _FATFS_H_
#define _FATFS_H_


#define TEST_FILE_LEN			(2*1024*1024)	/* ���ڲ��Ե��ļ����� */
#define BUF_SIZE				(4*1024)		/* ÿ�ζ�дSD����������ݳ��� */



void fatfs_test(void *p_arg);
void Data_display(short *time);
int referfile_time(unsigned int start_fptr,unsigned int end_fptr,short time[]);
void log_display(void);
#endif
