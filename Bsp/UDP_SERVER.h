#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

/* MAC ADcontinue;continue;DRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
#define MAC_ADDR0   0x42
#define MAC_ADDR1   0x8e
#define MAC_ADDR2   0x8f
#define MAC_ADDR3   0x3d
#define MAC_ADDR4   0x8a
#define MAC_ADDR5   0x53
 
/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   1
#define IP_ADDR3   253
   
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   1
#define GW_ADDR3   1  
#define UDP_LOCAL_PORT     		1030 

void UDP_server_init(void);


#endif

