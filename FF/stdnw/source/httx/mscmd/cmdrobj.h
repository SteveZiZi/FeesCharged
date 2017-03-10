/** @file
 *  @brief 后台通讯命令读参数对象定义及查找
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#ifndef __NW_CMDROBJ_H__
#define __NW_CMDROBJ_H__
#include "nwmscmd.h"

//
//
//
class C_AFNCMD_RPARAM:public C_AFNCMD
{
public:
	virtual int Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos);

protected:
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};


#endif//__NW_CMDROBJ_H__


