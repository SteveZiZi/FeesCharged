#ifndef _IFDATACOMM_H_
#define _IFDATACOMM_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sharemem/ipcSharemem.h"
#include "sysipc/sysipc.h"


class CIfDatacomm
{
public:
	static bool getCurZxygData(unsigned int mp,char *pData);
	static T_emCARDSTATE getCardState();
	static T_emCARDTYPE getCardType();
	static bool geInSevenDaysData(unsigned int mp,char dataarry[7][4]);
	static bool getInSixMonthsData(unsigned int mp,char dataarry[6][4]);
	static SIpcTerminalInfo GetTerminalInfo();
};
#endif


