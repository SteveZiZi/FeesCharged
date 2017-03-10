/** @file
 *  @brief 交流采集进程数据采集类
 *  @author 万青松
 *  @date 2012/03/01
 *  @version 0.1
 */
#ifndef __JLCY_SERVER_H__
#define __JLCY_SERVER_H__

#include "server.h"

class C_JlcyServer:public C_SERVER
{
public:
	virtual int BeforeService(void);
	virtual int AfterService(void);
	virtual int OnTickService(void);
	
private:


};

#endif//__JLCY_SERVER_H__

