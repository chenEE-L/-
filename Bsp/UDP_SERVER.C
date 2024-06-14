#include "main.h"
#include "app.h"

/***********************************************************************
函数名称：udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,struct ip_addr *addr, u16_t port)
功    能：udp数据接收和发送
注    意：这是一个回调函数，当一个udp段到达这个连接时会被调用
***********************************************************************/
struct ip_addr destAddr ; /* 获取远程主机 IP地址 */
u16_t  destPORT;
void udp_server_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,struct ip_addr *addr, u16_t port)
{
	//struct ip_addr destAddr = *addr; /* 获取远程主机 IP地址 */
	char *commd=NULL;
	destAddr = *addr;
	destPORT = port;
	struct pbuf *p_temp = p;
	//while(p_temp != NULL)
	{
		/******将数据原样返回*******************/
		//udp_sendto(pcb,p_temp,&destAddr,port); /* 将收到的数据再发送出去 */	
		
		commd = (char*)(p_temp->payload);
			if(commd[0] == 'c')
			{
				char num;
				num = commd[14]-'0';
				if(commd[15]!='\r')
				{
					num *= 10;
					num += commd[15]-'0';
				}
				num-=1;//将数据减一方便取余数1~30改为0~29
				num%=8;//对8取余数得到0~7，八个通道号
				if(num>0)
					Set_relay_onoff(0x01,1<<(num-1));//0号通道设零。
				else
					Set_relay_onoff(0x01,0);//0号通道设零。
				//comSendBuf(COM1,p_temp->payload,p_temp->len);//发送指令
			}
			else
			comSendBuf(COM1,p_temp->payload,p_temp->len);//发送指令
		p_temp = p_temp->next;
	}
	pbuf_free(p); 						/* 释放该UDP段 */
}


/***********************************************************************
函数名称：UDP_server_init(void)
功    能：完成UDP服务器的初始化，主要是使得UDP通讯快进入监听状态
***********************************************************************/
void UDP_server_init(void)
{
	struct udp_pcb *pcb;
	pcb = udp_new();								//申请udp控制块
	udp_bind(pcb,IP_ADDR_ANY,UDP_LOCAL_PORT); 	    /* 绑定本地IP地址和端口号（作为udp服务器） */
	udp_recv(pcb,udp_server_recv,NULL); 				/* 设置UDP段到时的回调函数 */

}
