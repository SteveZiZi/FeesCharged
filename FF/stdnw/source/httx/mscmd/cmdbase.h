/** @file
 *  @brief 后台通讯命令基础服务定义
 *  @author 
 *  @date 2011/12/22
 *  @version 0.1
 */
#ifndef __NW_CMDBASE_H__
#define __NW_CMDBASE_H__

#include "nwmscmd.h"

class C_CR_E0000000:public C_CmdReceiver
{
public:
	virtual bool ParseCmdInfo(string::const_iterator &iterBegin, string::const_iterator &iterEnd, S_CmdObj &CmdObj);
	virtual bool Action(const S_CmdObj &CmdObj, strings &strResponse);
};

class C_AFNCMD_CONFIRM:public C_AFNCMD
{
public:
	virtual int Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos);
protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};


#endif //__NW_CMDBASE_H__

