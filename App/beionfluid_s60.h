#ifndef BEIONFLUID_S60_H
#define BEIONFLUID_S60_H



typedef struct
{
	short command_status;//ָ��״̬  0���� 1ִ����
//	short value_num;//�������
//	short value_status;//��״̬ 0ֹͣ  1������
//	short motor_status;//���״̬ 0ֹͣ  1������
//	short push_switch_status;//����λ״̬ 0ֹͣ  1������
//	short pull_switch_status;//����λ״̬ 0ֹͣ  1������
//	short inner;//�ھ�
	short New_command_flag;
}LEVI2043E;

extern LEVI2043E levi_2043e;


extern void Levi2043e(void *p_arg);
extern void Goto_coordinate(void);
#endif
