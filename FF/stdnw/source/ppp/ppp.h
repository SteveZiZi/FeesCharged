/** @file
 *  @brief PPP拨号服务功能定义
 *  @author 
 *  @date 2011/11/30
 *  @version 0.1
 */
#ifndef __PPP_SERVER_H__
#define __PPP_SERVER_H__
#include "pppbase.h"
#include "sysbase.h"

class C_PppServer:public C_PppBase
{
public:
	C_PppServer(void){m_pPppInfo = NULL, m_pPppStatus = NULL;}
	virtual ~C_PppServer(){}

	virtual int BeforeService(void);

protected:
	virtual bool PppEnable(void);
	virtual int CheckNotify(void);
	virtual int CheckModemType(void);
	virtual int AdaptModem(void);
	
	virtual int OnPPPOffLine(void);		//拨号失败后调用，统计拨号失败次数
	virtual int OnPPPOnLine(void);	//拨号成功后调用，复位拨号失败次数计数器
	virtual bool NeedPpp(void);
	virtual bool NeedOnLine(void);		//拨号前调用，判断是否进行连接
	virtual bool NeedOffLine(void);
	virtual bool InitSignalVal(void);
	virtual bool SetSignalVal(int signal);

	virtual int GetModemPowerOffDelayS(void);
	virtual int GetModemPowerOnDelayS(void);
	virtual S_WORD GetRefreshSignalInterS(void);

	int LcdBottomInfo(void);
	int SavePppRunInfo(void);

protected:
	S_PppInfo *m_pPppInfo;
	S_PppStatus *m_pPppStatus;
};

#endif//__PPP_SERVER_H__

