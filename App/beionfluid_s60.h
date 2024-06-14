#ifndef BEIONFLUID_S60_H
#define BEIONFLUID_S60_H



typedef struct
{
	short command_status;//指令状态  0就绪 1执行中
//	short value_num;//阀的序号
//	short value_status;//阀状态 0停止  1运行中
//	short motor_status;//电机状态 0停止  1运行中
//	short push_switch_status;//推限位状态 0停止  1运行中
//	short pull_switch_status;//拉限位状态 0停止  1运行中
//	short inner;//内径
	short New_command_flag;
}LEVI2043E;

extern LEVI2043E levi_2043e;


extern void Levi2043e(void *p_arg);
extern void Goto_coordinate(void);
#endif
