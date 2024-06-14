#ifndef _DECODER_H_
#define _DECODER_H_

#include "bsp.h"
#include "app.h"

typedef enum
{
	set_tp=1,
	get_tp_strength,
	get_tp_quality,
	get_tp_info,
	stop_get_tp_info,
	get_sat_list,
	set_sat_params,
	blind_search_start,
	search_get_process,
	get_search_program_list,
	get_all_program_list,
	play_program_id,
	get_program_epg,
	set_lnb_params,
	set_agc_step,
	get_agc_value
}
Decoder_Commend;

typedef struct
{
	u32 fre;
	u32 sym;
}SAT;


typedef struct
{
	SAT sat;
	u8 SATID;
	u8 ANTID;
	u16 LNB1;
	u16 LNB2;
	u8 HV;
	u8 k22;
	u8 PORT;
}Antenna;


#endif 
