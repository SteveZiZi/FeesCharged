/**
 *  @file
 *  @brief 流量控制
 *  @author 
 *  @date 2011-07-28
 *  @version v1.0
 *  @note
 *  流量控制配置和实现
 */
#ifndef __BASEB_FLOWCTRL_H__
#define __BASEB_FLOWCTRL_H__
#include "mptype.h"

class C_IniFlow
{
public:
	static int LoadFlowCtrlInfo(S_FlowInfo &FlowInfo, const char *fname);
	static int SaveFlowCtrlInfo(const S_FlowInfo &FlowInfo, const char *fname);
};

class C_FlowCtrl
{
public:
	~C_FlowCtrl(){}
	
	static C_FlowCtrl &Instance(void);
	void SetFlowCtrlInfo(S_FlowInfo *pFlowInfo){/*m_pFlowInfo = pFlowInfo*/m_PrivateData = *pFlowInfo;} // 用局部变量，httx会互相影响

	bool FlowCtrlEnable(void){return m_pFlowInfo->m_FlowCtrlEnable;}
	bool ConnectDate(int day){return m_pFlowInfo->m_MSConnectDate.test(day);}
	bool ConnectTime(int hour){return m_pFlowInfo->m_MSConnectTime.test(hour);}

	S_WORD GetLogonInterSecond(void){return m_pFlowInfo->m_LogonInterS;}
	S_WORD GetLogonRetryTimes(void){return m_pFlowInfo->m_LogonRetryTimes;}
	S_WORD GetLogonFailedTimes(void){return m_pFlowInfo->m_LogonFailedTimes;}
	S_WORD GetSocketConnectInterSecond(void){return m_pFlowInfo->m_SocketConnectInterS;}
	S_WORD GetSocketConnectRetryTimes(void){return m_pFlowInfo->m_SocketConnectRetryTimes;}
	S_WORD GetSocketConnectFailedTimes(void){return m_pFlowInfo->m_SocketConnectFailedTimes;}
	S_WORD GetDialInterSecond(void){return m_pFlowInfo->m_DialInterS;}
	S_WORD GetDialRetryTimes(void){return m_pFlowInfo->m_DialRetryTimes;}
	S_WORD GetDialFailedTimes(void){return m_pFlowInfo->m_DialFailedTimes;}
	S_WORD GetDialFrequencyMinite(void){return m_pFlowInfo->m_DialFrequencyM;}
	S_WORD GetHeartBeatFailedTimes(void){return m_pFlowInfo->m_HeartBeatFailedTimes;}
	S_DWORD GetDayFlowMax(void){return m_pFlowInfo->m_DayFlowMax;} 
	S_DWORD GetMonFlowMax(void){return m_pFlowInfo->m_MonFlowMax;}

	void ClearFlowCtrlInfo(void){m_pFlowInfo->ClearFailedInfo();}
	void LogonRecord(bool fSuccess);
	void HeartBeatRecord(bool fSuccess);
	void SocketConnectRecord(bool fSuccess);
	void PppDialRecord(bool fSuccess);

private:
	C_FlowCtrl(void){m_pFlowInfo = &m_PrivateData;}

private:
	S_FlowInfo *m_pFlowInfo;
	S_FlowInfo m_PrivateData;
};

#define FLOWCTRL C_FlowCtrl::Instance()

#endif//__BASEB_FLOWCTRL_H__

