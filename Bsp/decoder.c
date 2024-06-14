#include "bsp.h"
#include "app.h"
#include "string.h"

char *commend[16];

Antenna antenna[8];

char abc[10][10];

void Antenna_init()
{
	antenna[1].ANTID = 1;
	antenna[1].SATID = 1;
	antenna[1].HV = 1;
	antenna[1].k22 = 1;
	antenna[1].LNB1 = 10600;
	antenna[1].LNB2 = 11300;
	antenna[1].PORT = 1;
	antenna[1].sat.fre = 12721-11300;
	antenna[1].sat.sym = 43200;
	
	commend[0] = "set_tp";
	commend[1] = "get_tp_strength";
	commend[2] = "get_tp_quality";
	commend[3] = "get_tp_info";
	commend[4] = "stop_get_tp_info";
	commend[5] = "get_sat_list";
	commend[6] = "set_sat_params";
	commend[7] = "blind_search_start";
	commend[8] = "search_get_process";
	commend[9] = "get_search_program_list";
	commend[10] = "get_all_program_list";
	commend[11] = "play_program_id";
	commend[12] = "get_program_epg";
	commend[13] = "set_lnb_params";
	commend[14] = "set_agc_step";
	commend[15] = "get_agc_value";
	
	
}


void send_commend(Antenna antenna,char *p[],char *ccmd[])
{
	int i;
	int commend;
	int pccmdtotal=sizeof(commend)/sizeof(char *);
	for(i=0;i<pccmdtotal;i++)
	{
		//Printf("p");
		if(0==strcmp(p[0],ccmd[i]))
		{
			break;
		}
	}
	commend = i;
	switch(commend)
	{
		case set_tp:
		{
			//comSendBuf(COM1, "set_tp fre=%d sym=%d HV=%d 22K=0 PORT=1\n", sizeof("set_tp fre=1150000 sym=27500000 HV=1 22K=0 PORT=1\n"));
			printf("set_tp fre=%d sym=%d HV=%d 22K=%d PORT=%d\n",antenna.sat.fre,antenna.sat.sym,antenna.HV,antenna.k22,antenna.PORT);
		}
		break;
		case get_tp_strength:
		{
			//comSendBuf(COM1, "get_tp_strength\n", sizeof("get_tp_strength\n"));
			printf("get_tp_strength\n");
		}
		break;
		case get_tp_quality:
		{
			//comSendBuf(COM1, "get_tp_quality\n", sizeof("get_tp_quality\n"));
			printf("get_tp_quality\n");
		}
		break;
		case get_tp_info:
		{
			//comSendBuf(COM1, "get_tp_info\n", sizeof("get_tp_info\n"));
			printf( "get_tp_info\n");
		}
		break;
		case stop_get_tp_info:
		{
			//comSendBuf(COM1, "stop_get_tp_info\n", sizeof("stop_get_tp_info\n"));
			printf("stop_get_tp_info\n");
		}
		break;
		case get_sat_list:
		{
			//comSendBuf(COM1, "get_sat_list\n", sizeof("get_sat_list\n"));
			printf("get_sat_list\n");
		}
		break;
		case set_sat_params:
		{
			printf("set_sat_params SATID=%d LNB1=%d LNB2=%d HV=%d 22K=%d PORT=%d\n",
			antenna.SATID,antenna.LNB1,antenna.LNB2,antenna.HV,antenna.k22,antenna.PORT);
		}
		break;
		case blind_search_start:
		{
			printf("blind_search_start SATID=%d\n",antenna.SATID);
		}
		break;
		case search_get_process:
		{
			//comSendBuf(COM1, "search_get_process\n", sizeof("search_get_process\n"));
			printf("search_get_process\n");
		}
		break;
		case get_search_program_list:
		{
			printf("get_search_program_list SATID=%d\n",antenna.SATID);
		}
		break;
		case get_all_program_list:
		{
			//comSendBuf(COM1, "get_all_program_list\n", sizeof("get_all_program_list\n"));
			printf("get_all_program_list\n");
		}
		break;
		case play_program_id:
		{
			printf("play_program_id PROGID=994\n");
		}
		break;
		case get_program_epg:
		{
			printf("get_program_epg PROGID=994\n");
		}
		break;
		case set_lnb_params:
		{
			printf("");
		}
		break;
		
		case set_agc_step:
		{
			
			//comSendBuf(COM1, "set_agc_step STEP=4\n", sizeof("set_agc_step STEP=4\n"));
			printf("set_agc_step STEP=4\n");
		}
		break;
		case get_agc_value:
		{
			//comSendBuf(COM1, "get_agc_value\n", sizeof("get_agc_value\n"));
			printf("get_agc_value\n");
		}
		break;
		default:break;
	}
}
