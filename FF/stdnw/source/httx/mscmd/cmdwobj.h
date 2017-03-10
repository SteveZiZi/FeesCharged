/** @file
 *  @brief 后台通讯命令写参数对象定义及查找
 *  @author  zhongguangxiong
 *  @date 2012/02/18
 *  @version 0.1
 */
#ifndef __NW_CMDWOBJ_H__
#define __NW_CMDWOBJ_H__
#include "nwmscmd.h"

//
//
//
class C_AFNCMD_WPARAM:public C_AFNCMD
{
public:
	virtual bool NeedPwdCheck(void){return true;}
	virtual int Execute(const S_CmdObjs &CmdObjs, strings &strResponseInfos);
	
protected:
	bool UpdateMeterInfo(void);
	virtual C_CmdReceiver *GetCmdReceiver(INT32U DI);
};


#endif //__NW_CMDWOBJ_H__


