
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "gps.h"
#include "serial_command.h"


char GPS_serial_command_word[GPS_COMMAND_MAX_LENGTH];

extern int   counter_times;
uint8_t  utc_valid=0;
uint8_t  utc_valid_old= 0;
extern int idle_menu_display;

GPS_time gps_time;
uint8_t gps_time_changed=0;

GPS_GEO_POS gps_position;
char *nmea_field[MAX_GPS_FIELD_NUMBER_ONE_COMMAND]; /////
int gps_1pps_lock_time=0;
int gps_1pps_unlock_time=0;

int display_satellite_cn=0;
short sat_vis_cn[GPS_CHANNEL_NUMBER_MAX]; ////各个接收通道的信噪比
short sat_vis_id[GPS_CHANNEL_NUMBER_MAX]; ////各个接收通道对应的星号

int gps_satellite_in_view;
extern int gps_lock_status;
extern uint8_t data_to_cpld;
extern void delay_1ms(int ms);




/*
int  gps_getOneCommandString(int uart_no,char *cmd_line, unsigned char n)  
{
  unsigned char c;
  int cnt = 0;
  SERIAL_BUFFER *pbuff;
  	if(uart_no == UART_0)
  	{
  		pbuff = &UART0_rcv_buffer;
  	}
  	else if(uart_no == UART_1)
  	{
  		pbuff = &UART1_rcv_buffer;
  	}
  	else  return 0;
  
  c = readBuff(pbuff);
  
  //while((c != '$')&&(c!=' '))
  while((c != '$')&&(c!='\0'))
  {
  		c = readBuff(pbuff);
  }   
  ////to find command start character or rcvbuff is empty 
  
  if(c == '$')
  { 
   	do  
   	{ 
   		cmd_line[cnt] = readBuff(pbuff);
	   	
   		c=cmd_line[cnt++];
   		if( c == '$') cnt =0;///如果命令没结束前发现新的命令起始符，从新开始接收新的命令
	   	
   		if(cnt == n-1) break;      
	 }while ( ( c != '\0') && (pbuff->flag!=EMPTY)); 
  
    IRQDisable();
  	
  	if(c == '\0') ////发现一个命令结束符
  	{
  		if ( pbuff->rcv_command_num >0)  pbuff->rcv_command_num--;
  	} 
  
  	else   ////如果接收缓冲区空但是没有发现命令结束符
  	{
  		 pbuff->rcv_command_num = 0;
  		 cnt = 0;
  	}
  	
  	if(cnt == n-1) cmd_line[cnt] = '\0';
  	IRQEnable();
  	
  }
  else  ////接收缓冲区空但是没有发现命令起始符
  	{
  		IRQDisable();	
  		pbuff->rcv_command_num = 0;////indicate that not any command is received
  		cnt = 0;
  		UART_rcv_buffer_init(uart_no);
  		IRQEnable();
  	}	
  
  return cnt;
 }     
 */
    
int gps_extract_field(char *buff,char *field[],char delimiter,short field_no_max)
{
   int i,field_no,current_field_pos,len;
   
   len = strlen(buff);
   if(len <= 0) return(0);             // no chars in string
   field_no=0;                                // field index 0..n-1
   current_field_pos=0;                                // char index     
   
   for(i=0; i<len; i++)
   {   
   
      if(current_field_pos == 0)
      {
         field[field_no] = buff+i;           // point to start of string    
      }      
     
      if((i==len-1) || (buff[i]=='*'))
      {        
          // last field found 
          buff[i] = '\0';               // terminate field with null 
          return(field_no+1);
      }
      else if(buff[i]=='\0')
      {        
          // last field found 
          return(field_no);
      }
      else if(buff[i]== delimiter)
      { 
         // end of current field found  
         buff[i] = '\0';               // terminate field with null
         current_field_pos = 0;                     // first char
         field_no++;                       // of next field        
      }
      else
      {
        current_field_pos++;                        // move to next character  
      } 
      
      if(field_no == field_no_max)
      {
         return(field_no);            // out of pointers
      }
   }
   return(field_no);
}    



void get_nmea_date(char *msgtmp[1],GPS_time *date)
{ 
  
   
   // read date  
   msgtmp[0][6]='\0';
   date->year  = atoi(msgtmp[0]+4); 
	 //date->year += 2000;
   msgtmp[0][4] = '\0'; 
   date->month = atoi(msgtmp[0]+2);
   msgtmp[0][2] = '\0';  
   if(date->month == 0) return;         // don't output non-data 
   date->date   = atoi(msgtmp[0]); 
  
} 
 

void get_nmea_utc(char *msgtmp[1],GPS_time *date)
{    
  

   //utc_secs = (float) atof(msgtmp[0]+4); 
    
    msgtmp[0][6]='\0';
    date->second = atoi(msgtmp[0]+4);
   //date.second = (char)utc_secs;
   msgtmp[0][4] = '\0';
   date->minute = atoi(msgtmp[0]+2); 
   msgtmp[0][2] = '\0';
   date->hour = atoi(msgtmp[0]);
   date->hour += 8;
   if(date->hour > 23)
   {   		
    	////////日期加一
  		int month[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};  	
    	int year = date->year;
    	if( ((year%4==0)&&(year%100 !=0))||(year%400==0))  month[2]++;
     	date->hour -= 24;
    	if(date->date < month[date->month])
    	{
    		date->date++;
   		 }
    	else  
    	{
    		 ////////月份加一
    		date->date = 1;
    		if(date->month<12)	date->month++;
    		else  ////////年加一
    		{
    			date->month = 1;
    			date->year++;
    		}
    	}
    	/*
    	if((gps_time.minute ==0)&&( gps_time.second == 0))   
   		{
  			send_debug_info("-----------------");
   			sprintf(serial_send_buff,"current time is[%02dy%02dm%02dd] [%02d:%02d:%02d]",gps_time.year,gps_time.month,gps_time.date, gps_time.hour,gps_time.minute,gps_time.second);
			send_debug_info(serial_send_buff);  
   		}
		*/
   	}
   

   if(utc_valid_old != utc_valid)
   {
   		utc_valid_old = utc_valid;
   		if(utc_valid == 1)
   		{
   			printf("GPS LOCKED  *%02d:%02d:%02d\r\n",date->hour,date->minute,date->second);
			  
			
		}
		else 
		{
			printf("GPS LOST     %02d:%02d:%02d\r\n",date->hour,date->minute,date->second);
			
		}
	
	}
	
} 


uint8_t get_nmea_valid_status(char *msgtmp[1])
{          
   char  time_valid;  
   short gps_qual = 0;
   
   // read gps postion status 
  
   time_valid = msgtmp[0][0];
   if(time_valid == '\0'){
      gps_qual = 0;
      return gps_qual;       // don't output non-data
   }
   else if(time_valid == 'V'){   
      gps_qual = 0;
     
   }
   else{    
      gps_qual = 1;    ///utc time is availabe
      
   }
   return gps_qual;
}
/*
RMC - Recommended Minimum Navigation Information

                                                            12

        1         2 3       4 5        6 7   8   9    10  11|

        |         | |       | |        | |   |   |    |   | |

 $--RMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,xxxx,x.x,a*hh<CR><LF>



 
1    = UTC of position fix
2    = Data status (V=navigation receiver warning)
3    = Latitude of fix
4    = N or S
5    = Longitude of fix
6    = E or W
7    = Speed over ground in knots
8    = Track made good in degrees True
9    = UT date
10   = Magnetic variation degrees (Easterly var. subtracts from true course)
11   = E or W
12   = Checksum
$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50

*/
void proc_rmc(char *msgtmp[30],int field_no)
{

	float latitude;
   int lat_dir;
   float longitude;
   int long_dir;
//   float alitude;
   // utc must be shown as valid or invalid

  
    
   
   if(field_no >=2) utc_valid = get_nmea_valid_status(msgtmp+2);
  //if(utc_valid == 1)
   {
		if(field_no >=6)
		{
			latitude = atof(&msgtmp[3][0]);//////维度
			latitude /= 100;
			longitude = atof(msgtmp[5]);
			longitude /= 100;


			if(msgtmp[4][0] == 'N') lat_dir = GPS_LATITUDE_NORTH;
			else if(msgtmp[4][0] == 'S') lat_dir = GPS_LATITUDE_SOUTH;
			else lat_dir = 0;           //lzl long_dir->lat_dir


			if(msgtmp[6][0] == 'E') long_dir = GPS_LONGITUDE_EAST;
			else if(msgtmp[6][0] == 'W') long_dir = GPS_LONGITUDE_WEST;
			//else long_dir = 0; 

			//if(lat_dir!=0)//GPS丢失就默认上次的值
			{
				gps_position.gps_latitude = latitude;   //维度
				gps_position.gps_latitude_dir= lat_dir;
			}
			//if(long_dir!=0)//GPS丢失就默认上次的值
			{
				gps_position.gps_longitude= longitude;   //经度
				gps_position.gps_longitude_dir = long_dir;
			}
			// utc must be shown as valid or invalid

			//printf("lat: %c %f long:%c %f\r\n",msgtmp[4][0],latitude,msgtmp[6][0],longitude);
			
			
			
			//if(lat_dir == GPS_LATITUDE_NORTH) latitude +=100;         
			//Serial_MasterCommandSend(0x0A,(int)(latitude*10+0.5));   //发送到舱内 维度
			//if(long_dir == GPS_LONGITUDE_EAST) longitude +=180;
			//Serial_MasterCommandSend(0x09,(int)(longitude*10+0.5));   //发送到舱内经度
			
			
			
		}
		if(field_no >=8)
		{
			gps_position.speed = atof(msgtmp[7]);
			gps_position.gps_heading = atof(msgtmp[8]);
		}
	  if(field_no >=9)  get_nmea_date(msgtmp+9,&gps_time);
    if(field_no >=1) get_nmea_utc(msgtmp+1,&gps_time); 
   }

   //printf("current time is[%02dy%02dm%02dd] [%02d:%02d:%02d]\r\n",gps_time.year,gps_time.month,gps_time.date, gps_time.hour,gps_time.minute,gps_time.second);

/*	if((gps_time.minute ==0)&&( gps_time.second == 0))   
   {
   		sprintf(serial_send_buff,"current time is[%02dy%02dm%02dd] [%02d:%02d:%02d]",gps_time.year,gps_time.month,gps_time.date, gps_time.hour,gps_time.minute,gps_time.second);
		send_debug_info(serial_send_buff);  
   }
*/  
   //if(field_no >=3) show_nmea_lla(msgtmp+3,msgtmp+5,msgalt);
   //if(field_no >=7) show_nmea_sog(msgtmp+7);
   //if(field_no >=8) show_nmea_cog(msgtmp+8);
   
  	//if(field_no >=10) show_nmea_mag(msgtmp+10);  
  
	gps_time_changed = 1;

   return;
}



/*
  通常一个gsv电文对应最多4个通道的信息，12个通道可用3个GSV电文来表示
  
*/
void get_nmea_vis_sats(short num_msg, char *msgtmp[30],int num_field)
{   
   int i = 0;

   int channel_number;
     
   num_field -= 3;
   channel_number = (num_msg -1) * 4;
   while(num_field>=4)
   {
      sat_vis_id[channel_number] = atoi(msgtmp[i*4]);
      //sat_vis_el[i+k]  = (float) atof(msgtmp[j+1]);
      //sat_vis_az[i+k]  = (float) atof(msgtmp[j+2]); 
      sat_vis_cn[channel_number]  = atoi(msgtmp[i*4 + 3]);
       i++; 
      num_field -= 4;
      channel_number++;
   }
   while(channel_number < num_msg * 4)
   {
   		sat_vis_cn[channel_number]  = 0;
   		channel_number++; 
   }
   
   channel_number = (num_msg -1) * 4;
   
}  


////gps satellites in view
/*
GSV Satellites in view
1 2 3 4 5 6 7 n
| | | | | | | |
$--GSV,x,x,x,x,x,x,x,...*hh
1) total number of messages
2) message number
3) satellites in view
4) satellite number
5) elevation in degrees
6) azimuth in degrees to true
7) SNR in dB
more satellite infos like 4)-7)
n) Checksum
*/

//$GPGSV,2,2,08,21,71264,23,23,65055,,26,12039,,31,31307,11*7E 
//$GPGSV,3,2,12,22,41163,,05,27050,,11,09288,,18,07151,*7F

void proc_gsv(char *msgtmp[30], int num_field)
{  
//   short max_msg = 0; ////GSV电文的总个数
   short num_msg = 0; ////当前GSV电文号
  // short num_vis = 0;  ////当前可视的星的个数
 
     
   //max_msg = atoi(msgtmp[1]);  
   num_msg = atoi(msgtmp[2]);
   gps_satellite_in_view = atoi(msgtmp[3]);
   
   get_nmea_vis_sats(num_msg, msgtmp+4,num_field);
   /*
   if(display_satellite_cn > 0)  
   {
       display_satellite_cn--;
   }
   */	
   
   return;
}
/*
void show_nmea_lla(char *msglat[2],char *msglon[2],char *msgalt[2])
{

 
   // read latitude,
   gps_position.lat_dir = msglat[1][0];
   
   strcpy(gps_position.lat_m,msglat[0]+2);  ////读取纬度值的分数
  // lat = atof(msglon[0]+2); 
   msglat[0][2] = '\0';
    
   gps_position.lat_d = atoi(msglat[0]); ////读取纬度值的度数
   
  
   
   // read longitude
   strcpy(gps_position.lon_m,msglon[0]+3);  ////读取经度值的分数
   //lon = atof(msglon[0]+3); 
   msglon[0][3] = '\0';
   gps_position.lon_d = atoi(msglon[0]);   
   gps_position.lon_dir = msglon[1][0];        // read direction  
  
  //////read altitude 
  strcpy(gps_position.alt,msgalt[0]);
  strcpy(gps_position.alt_uint,msgalt[1]);
  
  
}
 */
 
void proc_gga(char *msgtmp[20],int num_field)
{   

   float latitude;
   int lat_dir;
   float longitude;
   int long_dir;
//   float alitude;
   // utc must be shown as valid or invalid
   if(0 == atoi(msgtmp[6])) return ;
   /*
   Fix Quality:
	- 0 = Invalid
	- 1 = GPS fix
	- 2 = DGPS fix
   */      
   
   //show_nmea_lla(msgtmp+2,msgtmp+4,msgtmp+9);
   latitude = atof(msgtmp[2]);
   longitude = atof(msgtmp[4]);
   //alitude = atof(msgtmp[9]);

   if(msgtmp[3][0] == 'N') lat_dir = GPS_LATITUDE_NORTH;
   else if(msgtmp[3][0] == 'S') lat_dir = GPS_LATITUDE_SOUTH;
   else long_dir = 0; 
  

   if(msgtmp[5][0] == 'E') long_dir = GPS_LONGITUDE_EAST;
   else if(msgtmp[5][0] == 'W') long_dir = GPS_LONGITUDE_WEST;
   else long_dir = 0; 

   
  gps_position.gps_latitude = latitude;
  gps_position.gps_latitude_dir= lat_dir;
  
  gps_position.gps_longitude= longitude;
  gps_position.gps_longitude_dir = long_dir;
}

/*
$HCHDG, x.x,x.x,a,x.x,a*hh<cr><lf>
　　Deviation ( 偏向角 )是罗盘正方向和平台的正方向的夹角
　　Variation (磁偏角)是磁北与地理北之间的夹角
　　如果偏向角或磁偏角未被编入,相应的位置将为空白(根据NMEA 0183版本2.1,第5.2.2.3节).如果参数的绝对值大于180度,将不被编入.偏向角和磁偏角如果为正,指示为a=E；如果为负,指示为a=W。如果航向不能被计算出来，相应的位置将为空白（参见HPR专用的句子）
　　NMEA要求航向测量的单位为度。
　　例：
　　$HCHDG,85.8,0.0,E,0.0,E*77
　　$HCHDG,271.2,0.0,E,0.0,E*44
　　$HCHDG,271.1,10.7,E,12.2,W*52
　　$HCHDG,0.0,10.7,E,12.2,W*57
　　HDT      Heading True
　　航向真值
　　$HCHDT, x.x,T*hh<cr><lf>
　　如果磁偏角未被编入,或者航向不能被计算出来,则航向的部分将为空白(见HDG和定义).如果偏向角未被编入,则假设其为零,否则将被加入到航向和磁偏角的测量中来，表示罗盘指示的真值
　　例：
　　$HCHDT,86.2,T*15
　　$HCHDT,271.1,T*2C
　　$HCHDT,0.9,T*20
*/
void proc_HDG(char *msgtmp[20],int num_field)
{   
    
   // utc must be shown as valid or invalid
   float magnetic_sensor;
   float magnetic_deviation;
   int	 magnetic_deviation_dir;
   float magnetic_vatition;
   int	 magnetic_vatition_dir;

	
   magnetic_sensor = atof(msgtmp[1]);
   magnetic_deviation = atof(msgtmp[2]);
   magnetic_vatition = atof(msgtmp[4]);

   if(msgtmp[3][0] == 'E') magnetic_deviation_dir = GPS_LONGITUDE_EAST;
   else if(msgtmp[3][0] == 'W') magnetic_deviation_dir = GPS_LONGITUDE_WEST;
   else magnetic_deviation_dir = 0; 

   if(msgtmp[5][0] == 'E') magnetic_vatition_dir= GPS_LONGITUDE_EAST;
   else if(msgtmp[5][0] == 'W') magnetic_vatition_dir = GPS_LONGITUDE_WEST;
   else magnetic_vatition_dir = 0; 
   gps_position.compass_available= 1;
   gps_position.magnetic_heading = magnetic_sensor + magnetic_deviation*magnetic_deviation_dir;
   gps_position.true_heading = magnetic_sensor + gps_position.magnetic_heading + magnetic_vatition*magnetic_vatition_dir;
   printf("sensor:%f dev:%f var:%f\r\n",magnetic_sensor,magnetic_deviation,magnetic_vatition);
   return;
}

void process_nmea(char gps_command[])
{
  
  // char *field[30];
   int numfield;  
   char delimiter = ',';

   // extract the fields and process by type
   numfield = gps_extract_field(gps_command,nmea_field,delimiter,MAX_GPS_FIELD_NUMBER_ONE_COMMAND);   

            
   if(strcmp(nmea_field[0],"GPALM") == 0){
      //proc_alm(nmea_field,numfield);
   }   
   else if(strcmp(nmea_field[0],"GPGGA") == 0){
      //proc_gga(nmea_field,numfield);
   }
   else if(strcmp(nmea_field[0],"GPGLL") == 0){
     // proc_gll(nmea_field,numfield);
   }   
   else if(strcmp(nmea_field[0],"GPGSA") == 0){ 
     // proc_gsa(nmea_field,numfield);
   }
   else if(strcmp(nmea_field[0],"GPGSV") == 0){
   	  
      //proc_gsv(nmea_field, numfield);
   }   
   else if(strcmp(nmea_field[0],"GNRMC") == 0){
     proc_rmc(nmea_field,numfield);
     //send_debug_info("RMC");  //Guan
   }
	 else if(strcmp(nmea_field[0],"GPRMC") == 0){
     proc_rmc(nmea_field,numfield);
     //send_debug_info("RMC");  //Guan
   }
   else if(strcmp(nmea_field[0],"GPVTG") == 0){
     // proc_vtg(nmea_field,numfield);;
   }  
   else if(strcmp(nmea_field[0],"GPZDA") == 0){
     // proc_zda(nmea_field,numfield);
   }   
   else if(strcmp(nmea_field[0],"PRWIALT") == 0){
     // proc_alt(nmea_field,numfield);
   }
   else if(strcmp(nmea_field[0],"PRWIBIT") == 0){
     // proc_bit(nmea_field,numfield);
   }
   else if(strcmp(nmea_field[0],"PRWICOM") == 0){
     // proc_com(nmea_field,numfield);
   }
   else if(strcmp(nmea_field[0],"PRWIDGP") == 0){
      //proc_dgp(nmea_field,numfield);
   }
   else if(strcmp(nmea_field[0],"PRWIZCH") == 0){
      //proc_zch(nmea_field,numfield);
   } 
   else if(strcmp(nmea_field[0],"PRWIERR") == 0){
     // proc_err(nmea_field,numfield);
   } 
   else if(strcmp(nmea_field[0],"PRWIRID") == 0){
     // proc_rid(nmea_field,numfield);
   } 
   else if(strcmp(nmea_field[0],"HDG") == 0){
      //proc_HDG(nmea_field,numfield);
	 }
	else if(strcmp(&nmea_field[0][2],"HDG") == 0){
      //proc_HDG(nmea_field,numfield);
	 }
	 
   
}



void process_Gps_command()
{
	char command_word[GPS_COMMAND_MAX_LENGTH];
	int i=1;
	int get_command = 0;
	if(GPS_serial_command_word[0] ==1)
	{		
		get_command = 1;
		GPS_serial_command_word[0] = 0;
		for(i=1;(i<GPS_COMMAND_MAX_LENGTH)&&(GPS_serial_command_word[i]!= 0);i++)
		{
			command_word[i-1] = GPS_serial_command_word[i];
		}
		
		
		
	}
	if (get_command == 1)	
	{
		process_nmea(command_word);
	}
}


void gps_init()
{	
	 
/*
	 $PRWIILOG,RMC,A,T,1,0  ///设置允许RMC命令每秒发送一次 

	where:
   $PRWIILOG
   GGA        type of sentence
   A          A=activate（发送）, V=deactivate（不发送）
   T          cyclic
   1          every 1 second
   0          ??
*/
	int i;
	/*
	IRQDisable();
	
	
		UART_send_string(UART_0,"$PRWIILOG,???,V,,,\r\n"); ////disable all output message
		for(i=0;i<1000000;i++);
	 	UART_send_string(UART_0,"$PRWIILOG,RMC,A,T,1,0\r\n");////enable rmc message output every 1 second
		for(i=0;i<1000000;i++);
 		UART_send_string(UART_0,"$PRWIILOG,GSV,A,T,1,0\r\n");////enable GSV message output every 1 second
		for(i=0;i<1000000;i++);
		UART_send_string(UART_0,"$PRWIILOG,GGA,A,T,1,0\r\n");////enable GGA message output every 1 second
		for(i=0;i<1000000;i++);
		UART_send_string(UART_0,"$PSRF100,1,9600,8,1,0*0D\r\n");////9600
		for(i=0;i<10000000;i++);
	
	
	IRQEnable();
	
*/
	gps_position.compass_available = 1;//可用
	gps_position.gps_latitude= 0;
	 
	gps_position.gps_latitude_dir =0; 
	
	gps_position.gps_longitude=0; 
	gps_position.gps_longitude_dir = 0;
	gps_position.magnetic_heading = 0;
	gps_position.true_heading = 0;
	gps_position.speed = 0;
	gps_position.gps_heading = 0;
	gps_position.gps_heading_TTL = 0;
	gps_position.gps_latitude_old = 0;
	gps_position.gps_longitude_old = 0;
	//Satellite_search.ship_compass_angle = 0;
	
	for(i=0;i<GPS_CHANNEL_NUMBER_MAX;i++)
	{
		sat_vis_cn[i] = 0; ////各个接收通道的信噪比
		sat_vis_id[i] = 0; ////各个接收通道对应的星号
	}
}

