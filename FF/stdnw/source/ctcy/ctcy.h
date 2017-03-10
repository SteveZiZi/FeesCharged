/** @file
 *  @brief CT采样进程数据采集类
 *  @author 
 *  @date 2013/07/11
 *  @version 0.1
 */
#ifndef __CTCY_SERVER_H__
#define __CTCY_SERVER_H__
#include "server.h"

class C_CtcyServer:public C_SERVER
{
public:
	virtual int BeforeService(void);
	virtual int AfterService(void);
	virtual int OnTickService(void);
};

#endif//__FHKZ_SERVER_H__

