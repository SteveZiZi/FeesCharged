/** @file
 *  @brief 后台通讯命令工厂类定义
 *  @author 
 *  @date 2011/12/22
 *  @version 0.1
 */
#ifndef __NW_CMDFACT_H__
#define __NW_CMDFACT_H__

#include "nwmscmd.h"

class C_CmdFact
{
public:
	static C_AFNCMD *GetAfnCmd(INT8U AFN);
};

#endif //__NW_CMDFACT_H__

