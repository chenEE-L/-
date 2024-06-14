#include "CRC.h"
#include <stdio.h>



int crc(unsigned char *puchMsg,unsigned int usDataLen)
{
    //unsigned char *puchMsg ; /* ???CRC?????*/
    //unsigned short usDataLen ; /* ??????*/
    {
        unsigned char uchCRCHi = 0xFF ; /* ?CRC?????*/
        unsigned char uchCRCLo = 0xFF ; /* ?CRC ?????*/
        unsigned int uIndex ; /* CRC??????*/
        while (usDataLen--) /* ???????*/
        {
            uIndex = uchCRCHi ^ *puchMsg++ ; /* ??CRC */
            uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
            uchCRCLo = auchCRCLo[uIndex] ;
        }
				
        return ((uchCRCHi << 8) | uchCRCLo) ;
    }
}

void Get_crc(unsigned char *puchMsg,unsigned int usDataLen)
{
	int CRC_data=crc( puchMsg,usDataLen);
	puchMsg[usDataLen] = ((CRC_data&0xff00)>>8);
	puchMsg[usDataLen+1] = CRC_data&0xff;
}

bool check_crc(unsigned char *puchMsg,unsigned int usDataLen)
{
	int CRC_data=crc( puchMsg,usDataLen);
	if(puchMsg[usDataLen] == CRC_data/256&&puchMsg[usDataLen+1] == CRC_data%256)	
	return true;
	else return false;
}
