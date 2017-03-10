/** @file
 *  @brief 连接工厂定义
 *  @author 
 *  @date 2011/12/06
 *  @version 0.1
 */
#ifndef __NW_CONNECT_FACTORY_H__
#define __NW_CONNECT_FACTORY_H__
#include "connect.h"

class C_ConnFact
{
public:
	static C_CONNECT* CreateConnection(S_CHANNEL& channel);
	static void DestroyConnection(C_CONNECT *pConnect);
};


#endif//__NW_CONNECT_FACTORY_H__

